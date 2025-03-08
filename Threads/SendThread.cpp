#include "SendThread.h"
#include <QDebug>

SendThread::SendThread(QObject *parent)
    : QThread(parent) {
    // 构造函数
}

void SendThread::run() {
    // 线程的主循环
    while (!isInterruptionRequested()) {
        QMutexLocker locker(&m_mutex);
        if (!SendBuffer.isEmpty()) {
            QByteArray data = SendBuffer.dequeue();
            locker.unlock();  // 提前释放锁
            emit dataReadyToSend(data);  // 发出实际发送信号
            qDebug() << "Data sent from SendThread:" << data.toHex();
        } else {
            QThread::msleep(1);  // 避免CPU空转
        }
    }
}

void SendThread::sendData(const QByteArray &data) {
    // 将数据加入发送队列
    QMutexLocker locker(&m_mutex);
    SendBuffer.enqueue(data);
}
