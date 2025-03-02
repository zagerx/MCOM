#include "ProtocolHandler.h"
#include <QDebug>

ProtocolHandler::ProtocolHandler(QObject *parent) : QObject(parent) {}

QByteArray ProtocolHandler::buildFrame(CommandType cmd, const QByteArray &data) {
    QByteArray frame;
    frame.append(0xA5).append(0xA5); // Header
    
    // Command (little-endian)
    frame.append(static_cast<char>(cmd & 0xFF));
    frame.append(static_cast<char>((cmd >> 8) & 0xFF));
    
    // Data length
    quint16 len = data.length();
    frame.append(static_cast<char>(len & 0xFF));
    frame.append(static_cast<char>((len >> 8) & 0xFF));
    
    frame.append(data);
    
    // CRC
    quint16 crc = calculateCRC(frame);
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));
    
    // Footer
    frame.append(0x5A).append(0x5A);
    return frame;
}

void ProtocolHandler::processRawData(const QByteArray &rawData) {
    ReciveBuffer.append(rawData);
    return;
    // while(ReciveBuffer.size() >= 8) { // Minimum frame size
    //     int startIdx = ReciveBuffer.indexOf("\xA5\xA5");
    //     if(startIdx == -1) {
    //         ReciveBuffer.clear();
    //         return;
    //     }
    //     if(startIdx > 0) {
    //         ReciveBuffer.remove(0, startIdx);
    //     }
        
    //     if(ReciveBuffer.size() < 8) return;
        
    //     quint16 dataLen = static_cast<quint8>(ReciveBuffer[4]) |
    //                      (static_cast<quint8>(ReciveBuffer[5]) << 8);
    //     int totalLen = 8 + dataLen;
        
    //     if(ReciveBuffer.size() < totalLen) return;
        
    //     QByteArray frame = ReciveBuffer.left(totalLen);
    //     ReciveBuffer.remove(0, totalLen);
        
    //     // Verify CRC
    //     QByteArray crcData = frame.left(frame.size()-4);
    //     quint16 expectedCrc = calculateCRC(crcData);
    //     quint16 actualCrc = static_cast<quint8>(frame[frame.size()-4]) |
    //                         (static_cast<quint8>(frame[frame.size()-3]) << 8);
        
    //     if(expectedCrc != actualCrc) {
    //         emit protocolError("CRC校验失败");
    //         continue;
    //     }
        
    //     // Parse command
    //     CommandType cmd = static_cast<CommandType>(static_cast<quint8>(frame[2])) | (static_cast<quint8>(frame[3]) << 8);
        
    //     QByteArray payload = frame.mid(6, dataLen);
        
    //     switch(cmd) {
    //     case CMD_HEARTBEAT:
    //         emit heartbeatReceived();
    //         break;
    //     case CMD_ANGLE_DATA:
    //         if(payload.size() == 8) {
    //             double angle;
    //             memcpy(&angle, payload.constData(), 8);
    //             emit angleDataReceived(angle);
    //         }
    //         break;
    //     }
    // }
}
void ProtocolHandler::sendSpeedModeCommand(const QByteArray &data) {
    QByteArray frame = buildFrame(CMD_SET_SPEED_MODE, data);
    emit frameReadyToSend(frame); // 发送信号，通知外部发送数据
}
quint16 ProtocolHandler::calculateCRC(const QByteArray &data) {
    // Implement CRC-16/MODBUS
    quint16 crc = 0xFFFF;
    for(int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data[i]);
        for(int j = 0; j < 8; j++) {
            if(crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}
