#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QByteArray>

class ProtocolHandler : public QObject {
    Q_OBJECT
public:
    enum CommandType {
        CMD_HEARTBEAT = 0x0000,
        CMD_ANGLE_DATA = 0x0001,
        CMD_SET_SPEED_MODE = 0x0106 // 新增速度模式指令
    };

    explicit ProtocolHandler(QObject *parent = nullptr);
    
    QByteArray buildFrame(CommandType cmd, const QByteArray &data);
    void processRawData(const QByteArray &rawData);
    void sendSpeedModeCommand(const QByteArray &data); // 新增发送速度模式指令的函数

signals:
    void heartbeatReceived();
    void angleDataReceived(double value);
    void protocolError(const QString &error);
    void frameReadyToSend(const QByteArray &frame); // 新增信号，通知外部发送数据

private:
    quint16 calculateCRC(const QByteArray &data);
    
    QByteArray ReciveBuffer;
};

#endif // PROTOCOLHANDLER_H
