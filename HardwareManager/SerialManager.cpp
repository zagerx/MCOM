#include "SerialManager.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QThread>

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
    QMutexLocker locker(&m_writeMutex);
    if (!_Serial->isOpen()) return;

    int retries = 3;
    qint64 totalWritten = 0;
    
    while (retries-- > 0 && totalWritten < data.size()) {
        qint64 written = _Serial->write(data.constData() + totalWritten, 
                                      data.size() - totalWritten);
        if (written == -1) {
            logError("写入失败: " + _Serial->errorString());
            continue;
        }
        
        if (!_Serial->waitForBytesWritten(1000)) {
            logError("等待写入超时");
            continue;
        }
        
        totalWritten += written;
        if (totalWritten < data.size()) {
            qDebug() << "部分写入，已写:" << totalWritten << "/" << data.size();
        }
    }
    
    if (totalWritten != data.size()) {
        emit errorOccurred("数据发送不完整");
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
