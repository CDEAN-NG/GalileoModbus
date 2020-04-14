#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QDebug>
#include <QDate>
#include <QMetaEnum>
#include <QLoggingCategory>

#ifndef QT_STATIC
 QT_BEGIN_NAMESPACE
 Q_LOGGING_CATEGORY(QT_MODBUS, "qt.modbus")
 Q_LOGGING_CATEGORY(QT_MODBUS_LOW, "qt.modbus.lowlevel")
 QT_END_NAMESPACE
#endif

QT_USE_NAMESPACE

MainWindow *s_instance = nullptr;

union ConvertToFloat{
    float f;
    quint16 i[2];
};

union ConvertToDouble{
    double d;
    quint16 i[4];
};

static void HandlerFunction(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    if (auto instance = MainWindow::instance())
        instance->appendToLog(msg);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow), mClient(nullptr), mActiveRequest(nullptr),
    mDebugHandler(HandlerFunction)
{
    mUi->setupUi(this);
    s_instance = this;
    connect(mUi->actionExit, &QAction::triggered, []() {
        QGuiApplication::quit();
    });
    auto meta = metaObject();
    int enumerator = 0;
    for(int i(0); i < meta->enumeratorCount(); i++){
        auto enumType = meta->enumerator(i);
        QString namestr(enumType.name());
        if(namestr.compare(QStringLiteral("ModbusDataType"))== 0){
            enumerator = i;
            break;
        }
    }
    auto types = meta->enumerator(enumerator);
    auto count = types.keyCount();
    for(int index(0); index < count; index++ ) {
        auto key = types.key(index);
        mUi->dataType->addItem(key, types.keyToValue(key));
    }
    connect(mUi->connectButton, &QPushButton::clicked, this, &MainWindow::onConnect);
    connect(mUi->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnect);
    connect(mUi->sendButton, &QPushButton::clicked, this, &MainWindow::onSend);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
}

void MainWindow::appendOperationOutput()
{
    QString out;
    out.append(QStringLiteral("(Name), "));

    auto function = mUi->functionDrop->currentText().left(4).toShort(nullptr, 16);
    auto size = mUi->blockCount->text();
    out.append(QString::number(function) + ", ");
    out.append(mUi->startAddress->text() + ", ");
    out.append(size + ", ");
    auto dataType = mUi->dataType->currentData().toInt();
    out.append(QString::number(dataType) + ", ");
    out.append(QString::number(mUi->byteOrder->currentIndex()));
    mUi->exportData->appendPlainText(out);
}


void MainWindow::connectRequest()
{
    connect(mActiveRequest, &QModbusReply::finished, this, &MainWindow::receivedData);
    connect(mActiveRequest, &QModbusReply::errorOccurred, this, &MainWindow::handleError);
}

void MainWindow::disconnectRequest()
{
    disconnect(mActiveRequest, &QModbusReply::finished, this, &MainWindow::receivedData);
    disconnect(mActiveRequest, &QModbusReply::errorOccurred, this, &MainWindow::handleError);
    delete mActiveRequest;
    mActiveRequest = nullptr;
}

int MainWindow::generateEndAddress()
{
    int offset(mUi->startAddress->value());
    offset +=  mUi->blockCount->value();
    return offset;
}

void MainWindow::onConnect()
{
    if(mClient) {
        delete mClient;
    }
    mClient = new QModbusTcpClient(this);
    connect(mClient, &QModbusDevice::errorOccurred, this, [this](QModbusDevice::Error) {
        qDebug().noquote() << QStringLiteral("Error: %1").arg(mClient->errorString());
        onDisconnect();
    }, Qt::QueuedConnection);

    mClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter,
        mUi->tcpAddressEdit->text());
    mClient->setConnectionParameter(QModbusDevice::NetworkPortParameter,
        mUi->tcpPortEdit->text());

    mClient->setTimeout(mUi->timeoutSpin->value());
    mClient->setNumberOfRetries(mUi->retriesSpin->value());

    connect(mClient, &QModbusDevice::stateChanged, [this](QModbusDevice::State state) {
        switch (state) {
        case QModbusDevice::UnconnectedState:
            qDebug().noquote() << QStringLiteral("State: Entered unconnected state.");
            onDisconnect();
            break;
        case QModbusDevice::ConnectingState:
            qDebug().noquote() << QStringLiteral("State: Entered connecting state.");
            break;
        case QModbusDevice::ConnectedState:
            qDebug().noquote() << QStringLiteral("State: Entered connected state.");
            mUi->sendButton->setDisabled(false);
            break;
        case QModbusDevice::ClosingState:
            qDebug().noquote() << QStringLiteral("State: Entered closing state.");
            onDisconnect();
            break;
        }
    });
    mClient->connectDevice();
}

void MainWindow::onDisconnect()
{
    if(mClient->state() == QModbusDevice::ConnectedState){
        mClient->disconnectDevice();
    }
    mUi->sendButton->setDisabled(true);
    mUi->disconnectButton->setDisabled(true);
    mUi->connectButton->setDisabled(false);
    mUi->settingsWidget->setEnabled(true);
}

void MainWindow::onSend()
{
    appendOperationOutput();
    mUi->sendButton->setDisabled(true);
    if(mActiveRequest) {
        if(!mActiveRequest->isFinished()) {
            disconnectRequest();
        }
        delete mActiveRequest;
    }
    auto startAddress = mUi->startAddress->value();
    auto endAddress = mUi->endAddress->value();
    if(endAddress < startAddress || endAddress == 0){
        startOperation(startAddress, generateEndAddress());
    } else {
        startOperation(startAddress,endAddress);
    }
}

void MainWindow::receivedData()
{
    qDebug() << "Received data";
    auto result = mActiveRequest->result();
    qDebug() << "Got " << result.valueCount() << " values";
    auto values = result.values();
    qDebug() << values;
    setDataOutput(values);
    disconnectRequest();
    mUi->sendButton->setDisabled(false);
}

void MainWindow::handleError(QModbusDevice::Error error)
{
    qDebug() << "Error occured on read";
    qDebug() << error;
    qDebug() << mActiveRequest->errorString();
    disconnectRequest();
}

MainWindow *MainWindow::instance()
{
    return s_instance;
}

void MainWindow::startOperation(int start, int end)
{
    if(start > end) {
        qDebug() << "Start Address is less than end address";
        return;
    }
    auto function = mUi->functionDrop->currentText().left(4).toShort(nullptr, 16);
    auto size = mUi->blockCount->text().toInt();
    qDebug() << "Reading from " << start;
    qDebug() << "Function type: " << function;
    QModbusDataUnit unit(QModbusDataUnit::RegisterType(function), start, size);
    mActiveRequest = mClient->sendReadRequest(unit, 1);
    connectRequest();
}

void MainWindow::setDataOutput(QVector<quint16> &data)
{
    auto leftToRight = !bool(mUi->byteOrder->currentIndex());
    auto dataType = mUi->dataType->currentIndex();
    switch(dataType) {
        case ModbusDataType::Bool:{
            quint32 accumulator(0);
            for( auto value: data) {
                accumulator += value;
            }
            auto output = accumulator == 0 ? "False": "True";
            mUi->dataRecieved->setText(output);
            break;
        }
        case ModbusDataType::Float:{
            if(data.size() == 2){
                ConvertToFloat ctf;
                if(leftToRight) {
                    ctf.i[0] = data[0];
                    ctf.i[1] = data[1];
                } else {
                    ctf.i[1] = data[0];
                    ctf.i[0] = data[1];
                }
                qDebug() << "Got Float " << ctf.f;
                mUi->dataRecieved->setText(QString::number(ctf.f));
            }
            break;
        }
        case ModbusDataType::Uint16:{
                QString output;
                for( auto value: data) {
                    output.append(QString::number(value));
                    output.append(", ");
                }
                mUi->dataRecieved->setText(output);
            break;
        }
        case ModbusDataType::Int16:{
                QString output;
                for( auto value: data) {
                    output.append(QString::number(qint16(value)));
                    output.append(", ");
                }
                mUi->dataRecieved->setText(output);
            break;
        }
        case ModbusDataType::Uint32:{
            if(data.size() == 2){
                quint32 accumulator(0);
                accumulator += data[leftToRight ? 1:0];
                accumulator = accumulator << 16;
                accumulator += data[leftToRight ? 0:1];
                mUi->dataRecieved->setText(QString::number(accumulator));
            }
            break;
        }
        case ModbusDataType::Int32:{
            if(data.size() == 2){
                qint32 accumulator(0);
                accumulator += data[leftToRight ? 1:0];
                accumulator = accumulator << 16;
                accumulator += data[leftToRight ? 0:1];
                mUi->dataRecieved->setText(QString::number(accumulator));
            }
            break;
        }
        case ModbusDataType::Uint64:{
            if(data.size() == 4) {
                quint64 accumulator(0);
                if(leftToRight) {
                    accumulator += data[3];
                    accumulator = accumulator << 16;
                    accumulator += data[2];
                    accumulator = accumulator << 16;
                    accumulator += data[1];
                    accumulator = accumulator << 16;
                    accumulator += data[0];
                } else {
                    accumulator += data[0];
                    accumulator = accumulator << 16;
                    accumulator += data[1];
                    accumulator = accumulator << 16;
                    accumulator += data[2];
                    accumulator = accumulator << 16;
                    accumulator += data[3];
                }
                mUi->dataRecieved->setText(QString::number(accumulator));
            }
            break;
        }
        case ModbusDataType::Int64:{
            if(data.size() == 4) {
                qint64 accumulator(0);
                if(leftToRight) {
                    accumulator += data[3];
                    accumulator = accumulator << 16;
                    accumulator += data[2];
                    accumulator = accumulator << 16;
                    accumulator += data[1];
                    accumulator = accumulator << 16;
                    accumulator += data[0];
                } else {
                    accumulator += data[0];
                    accumulator = accumulator << 16;
                    accumulator += data[1];
                    accumulator = accumulator << 16;
                    accumulator += data[2];
                    accumulator = accumulator << 16;
                    accumulator += data[3];
                }
                mUi->dataRecieved->setText(QString::number(accumulator));
            }
            break;
        }
        case ModbusDataType::Double:{
            if(data.size() == 4) {
                ConvertToDouble d;
                if(leftToRight) {
                    d.i[3] = data[3];
                    d.i[2] = data[2];
                    d.i[1] = data[1];
                    d.i[0] = data[0];
                } else {
                    d.i[3] = data[0];
                    d.i[2] = data[1];
                    d.i[1] = data[2];
                    d.i[0] = data[3];
                }
                mUi->dataRecieved->setText(QString::number(d.d));
            }
            break;
        }
        case ModbusDataType::String:{
            QString output;
            for( auto value : data ) {
                quint8 low = (value & 0b11111111);
                value = value >> 8;
                quint8 high = (value & 0b11111111);
                if(leftToRight) {
                    output.append(QChar(low));
                    output.append(QChar(high));
                } else {
                    output.append(QChar(high));
                    output.append(QChar(low));
                }
            }
            mUi->dataRecieved->setText(output);
            break;
        }
        case ModbusDataType::DateDMY:{
            if(data.size() >= 3){ // date
                QDate d(data[2], data[1], data[0]);
                qDebug() << "Got Date: " << d.toString();
                mUi->dataRecieved->setText(d.toString());
            }
            break;
        }
        case ModbusDataType::DateMDY:{
            if(data.size() >= 3){ // date
                QDate d(data[2], data[0], data[1]);
                qDebug() << "Got Date: " << d.toString();
                mUi->dataRecieved->setText(d.toString());
            }
            break;
        }
        case ModbusDataType::DateYDM:{
            if(data.size() >= 3){ // date
                QDate d(data[0], data[2], data[1]);
                qDebug() << "Got Date: " << d.toString();
                mUi->dataRecieved->setText(d.toString());
            }
            break;
        }
        case ModbusDataType::DateYMD:{
            if(data.size() >= 3){ // date
                QDate d(data[0], data[1], data[2]);
                qDebug() << "Got Date: " << d.toString();
                mUi->dataRecieved->setText(d.toString());
            }
            break;
        }
        case ModbusDataType::TimeHMS:{
            if(data.size() >= 3){
                QTime  t(data[0],data[1],data[2]);
                qDebug() << "Got Time: " << t.toString();
                mUi->dataRecieved->setText(t.toString());
            }
            break;
        }
    }

}

MainWindow::~MainWindow()
{
    delete mUi;
    delete mClient;
    s_instance = nullptr;
}

