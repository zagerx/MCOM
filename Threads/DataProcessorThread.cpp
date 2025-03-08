#include "DataProcessorThread.h"
#include <QDebug>

DataProcessorThread::DataProcessorThread(RecvThread *recvThread, QObject *parent)
    : QThread(parent), data_soure(recvThread) {
    // 构造函数
}

void DataProcessorThread::run() {
    // 线程的主循环
    while (!isInterruptionRequested()) {
        // 从接收线程的缓冲区中读取数据
        QByteArray data = data_soure->readData();
        if (!data.isEmpty()) {
            qDebug() << "Processing data in DataProcessorThread:" << data.toHex();

            // 调用反序列化函数
            ProtocolHandler::Command cmd;
            QByteArray parsedData;
            if (protocolHandler.parseFrame(data, cmd, parsedData)) {
                // 在这里处理反序列化后的命令和数据
                switch (cmd) {
                    case ProtocolHandler::CMD_SET_SPEED:
                        qDebug() << "Command: Set Speed";
                        break;
                    case ProtocolHandler::CMD_DISABLE_MOTOR:
                        qDebug() << "Command: Disable Motor";
                        break;
                    case ProtocolHandler::CMD_NORMAL_MODE:
                        qDebug() << "Command: Normal Mode";
                        break;
                    case ProtocolHandler::CMD_DEBUG_MODE:
                        qDebug() << "Command: Debug Mode";
                        break;
                    case ProtocolHandler::CMD_HEARTBEAT:
                        emit heart_sigle();
                        break;
                    default:
                        qDebug() << "Unknown command";
                        break;
                }
            } else {
                qDebug() << "Failed to deserialize frame";
            }
        } else {
            QThread::msleep(1); 
        }
    }
}
