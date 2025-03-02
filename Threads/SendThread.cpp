#include "SendThread.h"
#include <QThread>

SendThread::SendThread(SerialManager *serial, ProtocolHandler *protocol, QObject *parent)
    : QThread(parent), SerialDev(serial), m_protocol(protocol) {}

void SendThread::run() {
    while (!isInterruptionRequested()) {
        // 示例：周期性发送心跳包
        QByteArray frame = m_protocol->buildFrame(ProtocolHandler::CMD_HEARTBEAT, QByteArray());
        SerialDev->writeData(frame);
        QThread::msleep(1000); // 1秒发送一次
    }
}