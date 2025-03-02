#ifndef DATAPROCESSORTHREAD_H
#define DATAPROCESSORTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include "Threads/RecvThread.h"

class DataProcessorThread : public QThread {
    Q_OBJECT
public:
    explicit DataProcessorThread(RecvThread *recvThread, QObject *parent = nullptr);

protected:
    void run() override;

private:
    RecvThread *data_soure; // 接收线程，用于读取数据
};

#endif // DATAPROCESSORTHREAD_H