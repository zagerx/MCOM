#include "IOThread.h"
#include <QDebug>

IOThread::IOThread(QObject *parent)
    : QThread(parent) {
    // 构造函数
}

void IOThread::run() {
    // 线程的主循环
    while (!isInterruptionRequested()) {
        QMutexLocker locker(&m_mutex);
        while (SendBuffer.isEmpty() && !isInterruptionRequested()) {
            m_dataCondition.wait(&m_mutex, 50); // 超时50ms
        }        
        if (!SendBuffer.isEmpty()) {
            QByteArray data = SendBuffer.dequeue();
            locker.unlock();  // 提前释放锁
            emit dataReadyToSend(data);  // 发出实际发送信号
            qDebug() << "Data sent from IOThread:" << data.toHex();
        } else {
            QThread::msleep(2);  // 避免CPU空转
        }
    }
}

void IOThread::sendData(const QByteArray &data) {
    // 将数据加入发送队列
    QMutexLocker locker(&m_mutex);
    SendBuffer.enqueue(data);
    m_dataCondition.wakeAll(); // 唤醒线程
}
void IOThread::handleReceivedData(const QByteArray &data) {
    QMutexLocker locker(&m_mutex);
    
    // 缓冲区管理
    if (m_recvBuffer.size() + data.size() > MAX_BUFFER_SIZE) {
        qWarning() << "接收缓冲区溢出，清空数据";
        m_recvBuffer.clear();
    }
    m_recvBuffer.append(data);

    // 协议帧分割
    int pos = 0;
    while ((pos = m_recvBuffer.indexOf("\xA5\xA5", pos)) != -1) {
        int endPos = m_recvBuffer.indexOf("\x5A\x5A", pos + 2);
        if (endPos == -1) break;

        QByteArray frame = m_recvBuffer.mid(pos, endPos - pos + 2);
        m_recvBuffer.remove(0, endPos + 2); // 移除已处理数据
        emit dataReadyToProcess(frame);
        pos = 0; // 重置查找位置
    }
}
