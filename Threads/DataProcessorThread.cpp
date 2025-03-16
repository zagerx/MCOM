#include "DataProcessorThread.h"
#include <QDebug>

DataProcessorThread::DataProcessorThread( QObject *parent)
    : QThread(parent)
{
    // 构造函数
    m_waitCondition = new QWaitCondition();
     proBuffer.reserve(1000);
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
void DataProcessorThread::run()
{
    QElapsedTimer batchTimer;
    const int MAX_BATCH_SIZE = 200; // 每批处理200帧
    while (!isInterruptionRequested()) {
        QVector<QByteArray> processingBatch;
        
        { // 临界区开始
            QMutexLocker locker(&m_mutex);
            m_waitCondition->wait(&m_mutex, 50);
            
            // 批量转移数据
            processingBatch.reserve(qMin(proBuffer.size(), MAX_BATCH_SIZE));
            while (!proBuffer.isEmpty() && processingBatch.size() < MAX_BATCH_SIZE) {
                processingBatch.append(proBuffer.dequeue());
            }
        } // 临界区结束
        if (!processingBatch.isEmpty()) {
            batchTimer.start();
            
            foreach (const QByteArray &frame, processingBatch) {
                ProtocolHandler::Command cmd;
                QByteArray parsedData;
                if (protocolHandler.parseFrame(frame, cmd, parsedData)) {
                    switch (cmd) {
                    case ProtocolHandler::CMD_SPEED_MODE:
                        processSpeedModeData(parsedData);
                        break;
                    case ProtocolHandler::CMD_HEARTBEAT:
                    emit heartSignal();
                        static unsigned int cnt;
                        cnt++;
                        channelDataReady(cnt%8, sinf(cnt*0.1f));
                        break;
                    default:
                        qDebug() << "Unknown command 0x" << QString::number(cmd, 16);
                        break; 
                    }
                }
                if (batchTimer.elapsed() > 15) // 每批处理不超过15ms
                    break;
            }
        }
    }
}

// 新增批量数据处理方法
void DataProcessorThread::processSpeedModeData(const QByteArray& parsedData)
{
    const float* floatData = reinterpret_cast<const float*>(parsedData.constData());
    size_t count = parsedData.size() / sizeof(float);
    
    // 仅处理第一个通道数据
    if(count >= 1) {
        emit channelDataReady(0, floatData[0]); // 通道索引从0开始
    }
}
