#include "SerialManager.h"
#include <QDebug>
#include <QSerialPortInfo>

SerialManager::SerialManager(QObject *parent) : QObject(parent) {
    _Serial = new QSerialPort(this);
    configureSerialPort(); // 配置串口参数

    // 连接信号槽
    connect(_Serial, &QSerialPort::readyRead, this, &SerialManager::handleReadyRead);
    connect(_Serial, &QSerialPort::errorOccurred, this, &SerialManager::handleError);
}

SerialManager::~SerialManager() {
    if (_Serial->isOpen()) {
        _Serial->close();
    }
    delete _Serial;
}
bool SerialManager::openPort(const QString &portName) {
    _Serial->setPortName(portName);
    if (_Serial->open(QIODevice::ReadWrite)) {
        _Serial->clear(QSerialPort::Input); // 清空输入缓冲区
        return true;
    } else {
        logError("Failed to open port: " + _Serial->errorString());
        emit errorOccurred(_Serial->errorString());
        return false;
    }
}

void SerialManager::closePort() {
    if (_Serial->isOpen()) {
        _Serial->clear(QSerialPort::Input); // 清空输入缓冲区
        _Serial->close();
    }
}

bool SerialManager::isOpen() const {
    return _Serial->isOpen();
}

void SerialManager::writeData(const QByteArray &data) {
    if (_Serial->isOpen()) {
        if (_Serial->write(data) == -1) {
            logError("Failed to write data: " + _Serial->errorString());
            emit errorOccurred(_Serial->errorString());
        }
    } else {
        logError("Port is not open, cannot write data.");
    }
}

void SerialManager::handleReadyRead() {
    QByteArray data = _Serial->readAll();
    emit dataReceived(data); // 发送数据接收信号
}

void SerialManager::handleError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
        QString errorMessage = "Serial port error: " + _Serial->errorString();
        logError(errorMessage);
        emit errorOccurred(errorMessage);
    }
}

void SerialManager::setBaudRate(int baudRate) {
    _Serial->setBaudRate(baudRate);
}

QStringList SerialManager::getAvailablePorts() const {
    QStringList ports;
    for (const QSerialPortInfo &port : QSerialPortInfo::availablePorts()) {
        ports.append(port.portName());
    }
    return ports;
}

void SerialManager::configureSerialPort() {
    _Serial->setParity(QSerialPort::NoParity);
    _Serial->setDataBits(QSerialPort::Data8);
    _Serial->setStopBits(QSerialPort::OneStop);
    _Serial->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialManager::logError(const QString &errorMessage) {
    qDebug() << errorMessage; // 记录错误信息到调试输出
}
