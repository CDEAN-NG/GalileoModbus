#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QModbusTcpClient;
class QModbusReply;
#include <QModbusDataUnit>
#include <QModbusDevice>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DebugHandler
{
public:
    explicit DebugHandler(QtMessageHandler newMessageHandler)
        : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
    {}
    ~DebugHandler() { qInstallMessageHandler(oldMessageHandler); }

private:
    QtMessageHandler oldMessageHandler;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    enum ModbusDataType { Bool, Float, Uint16, Int16, Uint32, Int32, Uint64, Int64, Double, String, DateMDY, DateYMD, DateYDM, DateDMY, TimeHMS};
    Q_ENUM(ModbusDataType)
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow *instance();
    void appendToLog(const QString &msg) {
        mUi->logTextEdit->appendPlainText(msg);
    }

signals:
    void recievedAllData();

public slots:
    void onConnect();
    void onDisconnect();
    void onSend();


protected slots:
    void receivedData();
    void handleError(QModbusDevice::Error error);

private:
    Ui::MainWindow*    mUi;
    QModbusTcpClient*  mClient;
    QModbusReply*      mActiveRequest;
    DebugHandler       mDebugHandler;
    void appendOperationOutput();
    int  generateEndAddress();
    void connectRequest();
    void disconnectRequest();
    void setDataOutput(QVector<quint16>& data);
    void startOperation(int start, int end);
};
#endif // MAINWINDOW_H
