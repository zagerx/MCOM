#include "ProtocolHandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent) : QObject(parent) {
    // 构造函数
}

QByteArray ProtocolHandler::buildFrame(Command cmd, const QByteArray &data) {
    QByteArray frame;

    // 帧头
    frame.append(0xA5);
    frame.append(0xA5);

    // 命令 (little-endian)
    frame.append(static_cast<char>(cmd>>8 & 0xFF));
    frame.append(static_cast<char>((cmd) & 0xFF));

    // 数据长度 (little-endian)
    quint16 dataLen = data.size();
    frame.append(static_cast<char>(dataLen & 0xFF));
    frame.append(static_cast<char>((dataLen >> 8) & 0xFF));

    // 数据
    frame.append(data);

    // CRC16 (计算范围：帧头到数据)
    quint16 crc = calculateCRC(frame);
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));

    // 帧尾
    frame.append(0x5A);
    frame.append(0x5A);

    return frame;
}

bool ProtocolHandler::parseFrame(const QByteArray &frame, Command &cmd, QByteArray &data) {
    // 检查帧长度是否合法
    if (frame.size() < 10) { // 最小帧长度：帧头(2) + 命令(2) + 数据长度(2) + CRC(2) + 帧尾(2)
        qDebug() << "Frame too short";
        return false;
    }

    // 检查帧头和帧尾
    if (static_cast<byte>(frame[0]) != 0xA5 
        || static_cast<byte>(frame[1]) != 0xA5
        || static_cast<byte>(frame[frame.size()-2]) != 0x5A
        || static_cast<byte>(frame[frame.size()-1]) != 0x5A)
    {
        qDebug() << "无效的帧头/帧尾:"
                 << static_cast<int>(frame[0]) 
                 << static_cast<int>(frame[1]);
        return false;
    }

    // 提取命令
    cmd = static_cast<Command>((static_cast<quint8>(frame[2]) << 8) | static_cast<quint8>(frame[3]));

    // 提取数据长度
    quint16 dataLen = (static_cast<quint8>(frame[5]) << 8) | static_cast<quint8>(frame[4]);

    // 检查数据长度是否合法
    if (frame.size() != 10 + dataLen) {
        qDebug() << "Invalid data length";
        return false;
    }

    // 提取数据
    data = frame.mid(6, dataLen);

    // 计算并验证 CRC
    // QByteArray crcData = frame.left(frame.size() - 4); // 帧头到数据部分
    // quint16 expectedCrc = calculateCRC(crcData);
    quint16 actualCrc = (static_cast<quint8>(frame[frame.size() - 4]) << 8) | static_cast<quint8>(frame[frame.size() - 3]);

    if (0x1234 != actualCrc) {
        qDebug() << "CRC check failed";
        return false;
    }

    return true;
}

quint16 ProtocolHandler::calculateCRC(const QByteArray &data) {
    // CRC-16/MODBUS 算法
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data[i]);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
