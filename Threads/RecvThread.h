#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QQueue>

class RecvThread : public QThread {
    Q_OBJECT
public:
    explicit RecvThread(QObject *parent = nullptr);
    QByteArray readData(); // 读取缓冲区中的数据

protected:
    void run() override;

public slots:
    void processData(const QByteArray &data); // 槽函数：处理数据

private:
    QQueue<QByteArray> ReciveBuffer; // 数据缓冲区
    QMutex m_mutex; // 互斥锁，用于保护缓冲区
};

#endif // RECVTHREAD_H