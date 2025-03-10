#include "DataProcessorThread.h"
#include <QDebug>

DataProcessorThread::DataProcessorThread( QObject *parent)
    : QThread(parent)
{
    // 构造函数
    m_waitCondition = new QWaitCondition();
}
DataProcessorThread::~DataProcessorThread()
{
    // 构造函数
    delete m_waitCondition;
}

void  DataProcessorThread::receiveUIData(const QString name, const QByteArray &data)
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
        emit uiDataReadyToSend(byte_data);
    }
}

//processRawData 槽函数
void DataProcessorThread::processRawData(const QByteArray &data) {
    QMutexLocker locker(&m_mutex);
    proBuffer.enqueue(data);
    m_waitCondition->wakeOne(); // 取消注释并激活等待
}
void DataProcessorThread::run() {
    QMutexLocker locker(&m_mutex);
    QQueue<QByteArray> localBuffer;
    
    while (!isInterruptionRequested()) {
        // 等待直到有数据或超时
        m_waitCondition->wait(&m_mutex, 50);
        
        // 原子交换缓冲队列
        proBuffer.swap(localBuffer);
        
        if (!localBuffer.isEmpty()) {
            locker.unlock(); // 处理期间释放锁
            
            QElapsedTimer timer;
            timer.start();
            
            // 批量处理数据
            while (!localBuffer.isEmpty()) {
                QByteArray data = localBuffer.dequeue();
                
                ProtocolHandler::Command cmd;
                QByteArray parsedData;
                if (protocolHandler.parseFrame(data, cmd, parsedData)) {
                    // 命令处理逻辑
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
                        emit heartSignal();
                        break;
                    default:
                        qDebug() << "Unknown command 0x" << QString::number(cmd, 16);
                        break;                    
                    }
                }
                
                // 超时保护（单帧处理不超过10ms）
                if (timer.elapsed() > 10) break;
            }
            
            locker.relock(); // 重新加锁
            
            // 将未处理完的数据放回队列头部
            while (!localBuffer.isEmpty()) {
                proBuffer.prepend(localBuffer.takeLast());
            }
        }
    }
}

