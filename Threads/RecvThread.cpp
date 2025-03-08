#include "RecvThread.h"
#include <QDebug>

RecvThread::RecvThread(QObject *parent)
    : QThread(parent) {
    // 构造函数
}

void RecvThread::run() {
    QMutexLocker locker(&m_mutex);
    while (!isInterruptionRequested()) {
        // 等待直到有数据或中断请求
        while (ReciveBuffer.isEmpty() && !isInterruptionRequested()) {
            m_waitCondition->wait(&m_mutex, 50); // 带超时的等待
        }

        // 处理所有待处理数据
        while (!ReciveBuffer.isEmpty())
        {
            QByteArray data = ReciveBuffer.dequeue();
            locker.unlock();
            // 直接转发原始数据（不处理）
            emit rawDataReceived(data); // 新增信号
            locker.relock(); // 重新加锁继续处理
        }
    }
}

void RecvThread::processData(const QByteArray &data) {
    QMutexLocker locker(&m_mutex);
    ReciveBuffer.enqueue(data);
    m_waitCondition->wakeOne(); // 取消注释并激活等待
}
