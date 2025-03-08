#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QStringList>
#include <QMutex>

class SerialManager : public QObject {
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    bool openPort(const QString &portName);
    void closePort();
    bool isOpen() const;
    void setBaudRate(int baudRate);
    QStringList getAvailablePorts() const; // 获取可用串口号

public slots:
    void writeData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    void configureSerialPort(); // 配置串口参数
    void logError(const QString &errorMessage); // 记录错误信息
    QMutex m_writeMutex; 
    QSerialPort *_Serial;
};

#endif // SERIALMANAGER_H
