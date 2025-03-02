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
            qDebug() << "Processing data in DataProcessorThread:" << data;
        } else {
            QThread::msleep(1); // 如果缓冲区为空，短暂休眠以避免忙等待
        }
    }
}
