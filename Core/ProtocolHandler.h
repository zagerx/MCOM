#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QByteArray>
#include <QDebug>

class ProtocolHandler : public QObject {
    Q_OBJECT
public:
    // 命令枚举
    enum Command {
        CMD_SET_SPEED = 0x0101,     // 设置速度
        CMD_DISABLE_MOTOR = 0x0103, // 电机失能
        CMD_NORMAL_MODE = 0x0105,   // 电机正常模式
        CMD_SPEED_MODE = 0x0106,    // 电机调试模式
        CMD_HEARTBEAT = 0x0200      // 心跳
    };
    using byte = unsigned char;  // 增加类型别名提高可读性
    explicit ProtocolHandler(QObject *parent = nullptr);

    // 序列化：将命令和数据打包成帧
    QByteArray buildFrame(Command cmd, const QByteArray &data);

    // 反序列化：从帧中解析出命令和数据
    bool parseFrame(const QByteArray &frame, Command &cmd, QByteArray &data);

private:
    // 计算 CRC16
    quint16 calculateCRC(const QByteArray &data);
};

#endif // PROTOCOLHANDLER_H
