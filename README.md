# GalileoModbus
Probe tool for finding Modbus registers and generating CSV data to access them


## Data Types

| Name | Value |
| ----------- | ------------- |
| Bool | 0 |
| Float | 1 |
| Uint16 | 2 |
| Int16 |  3 |
| Uint32 | 4 |
| Int32 |  5 |
| Uint64 | 6 |
| Int64 |  7 |
| Double | 8 |
| String |  9 |
| DateMDY | 10 |
| DateYDM | 11 |
| DateDMY | 12 |
| TimeHMS | 13 |


## Byte Order Flag
This flag determines the endianess the data is expected to be stored.
Imagine two 16-bit registers at address 521 and 522

In big endian the data will be read ->  [521]:[522]

In little endian the data will be read ->  [522]:[521]

| Order | Flag |
| -------- | -------- |
| Low -> High (Little) | 0 |
|  High -> Low (Big) | 1 |

## Note about Modbus Addressing
Modbus ASCII Addressing
5-Digit Addressing vs. 6-Digit Addressing
In Modbus addressing, the first digit of the address specifies the primary table. The remaining digits represent
the device's data item. The maximum value is a two byte unsigned integer (65,535). Six digits are required to
represent the entire address table and item. As such, addresses that are specified in the device's manual as
0xxxx, 1xxxx, 3xxxx, or 4xxxx will be padded with an extra zero once applied to the Address field of a Modbus
tag.

## All addresses stored under maps/device/****.csv are stored as decimal representation. The application parsing the CSV file will need to convert from their decimal representation into a 5 or 6 digit address where needed.



##  How to decode data based on the data type and endian flag
- Bool is read as a 16 bit unsigned integer and then turned into a false if zero, otherwise true.
- Float is converted from 2 16 bit unsigned registers into a single 32 bit float value respective of the endianess.

- Unsigned 16-bit Integer is read as raw value.

- Strings are read all 16-bit registers in, breaking each registers into 2 8-bit ascii values. They are appended to the string based on the endianess described.



