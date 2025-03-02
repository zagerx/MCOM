#include "RecvThread.h"
#include <QDebug>

RecvThread::RecvThread(QObject *parent)
    : QThread(parent) {
    // 构造函数
}

void RecvThread::run() {
    // 线程的主循环
    while (!isInterruptionRequested()) {
        QThread::msleep(1); // 避免 CPU 占用过高
    }
}

void RecvThread::processData(const QByteArray &data) {
    // 将数据放入缓冲区
    QMutexLocker locker(&m_mutex); // 加锁
    ReciveBuffer.enqueue(data); // 将数据加入队列
}

QByteArray RecvThread::readData() {
    QMutexLocker locker(&m_mutex); // 加锁
    if (!ReciveBuffer.isEmpty()) {
        return ReciveBuffer.dequeue(); // 从队列中取出数据
    }
    return QByteArray(); // 如果缓冲区为空，返回空的 QByteArray
}