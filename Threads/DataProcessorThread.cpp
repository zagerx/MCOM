#include "DataProcessorThread.h"
#include <QDebug>

DataProcessorThread::DataProcessorThread(RecvThread *recvThread, QObject *parent)
    : QThread(parent), data_soure(recvThread) {
    // 构造函数
}

void  DataProcessorThread::getUIData(const QString name, const QByteArray &data)
{
    if(name == "正常模式")
    {
        //序列化数据
        QByteArray byte_data;
        byte_data = protocolHandler.buildFrame(ProtocolHandler::CMD_NORMAL_MODE,data);
        qDebug() << "buildFrame" << byte_data.toHex();
    }else if(name == "速度模式"){
        //序列化数据
        QByteArray byte_data;
        byte_data = protocolHandler.buildFrame(ProtocolHandler::CMD_SPEED_MODE,data);
        qDebug() << "buildFrame" << byte_data.toHex();
        emit ui_dataReady_sigle(byte_data);
    }
}

//handleRawData 槽函数
void DataProcessorThread::handleRawData(const QByteArray &data) {
    QMutexLocker locker(&m_mutex);
    proBuffer.enqueue(data);
    m_waitCondition->wakeOne(); // 取消注释并激活等待
}
void DataProcessorThread::run() {
    QMutexLocker locker(&m_mutex);
    while (!isInterruptionRequested()) {
        // 等待直到有数据或中断请求
        while (proBuffer.isEmpty() && !isInterruptionRequested()) {
            m_waitCondition->wait(&m_mutex, 50); // 带超时的等待
        }

        // 处理所有待处理数据
        while (!proBuffer.isEmpty()) {
            QByteArray data = proBuffer.dequeue();
            locker.unlock(); // 处理数据时释放锁
            
            qDebug() << "Processing data in DataProcessorThread:" << data.toHex();

            ProtocolHandler::Command cmd;
            QByteArray parsedData;
            if (protocolHandler.parseFrame(data, cmd, parsedData)) {
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
                    case ProtocolHandler::CMD_SPEED_MODE:  // 修正命令描述
                        qDebug() << "Command: Speed Mode";
                        break;
                    case ProtocolHandler::CMD_HEARTBEAT:
                        emit heart_sigle();
                        break;
                    default:
                        qDebug() << "Unknown command 0x" << QString::number(cmd, 16);
                        break;
                }
            } else {
                qDebug() << "Failed to deserialize frame:" << data.toHex();
            }
            
            locker.relock(); // 重新加锁继续处理
        }
    }
}