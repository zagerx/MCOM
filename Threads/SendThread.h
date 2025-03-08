#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QQueue>

class SendThread : public QThread {
    Q_OBJECT
public:
    explicit SendThread(QObject *parent = nullptr);
    
public slots:
    void sendData(const QByteArray &data);  // 接收发送数据的槽函数

signals:
    void dataReadyToSend(const QByteArray &data);  // 数据准备好发送的信号

protected:
    void run() override;

private:
    QQueue<QByteArray> SendBuffer;  // 发送缓冲区
    QMutex m_mutex;  // 互斥锁
};

#endif // SENDTHREAD_H