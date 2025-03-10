#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
class IOThread : public QThread {
    Q_OBJECT
public:
    explicit IOThread(QObject *parent = nullptr);
    
public slots:
    void sendData(const QByteArray &data);  // 接收发送数据的槽函数
    void handleReceivedData(const QByteArray &data);
signals:
    void dataReadyToSend(const QByteArray &data);  // 数据准备好发送的信号
    void dataReadyToProcess(const QByteArray &data);
protected:
    void run() override;

private:
    QWaitCondition m_dataCondition;
    QQueue<QByteArray> SendBuffer;  // 发送缓冲区
    QMutex m_mutex;  // 互斥锁
    QByteArray m_recvBuffer; // 新增接收缓冲区
    const int MAX_BUFFER_SIZE = 4096; // 4KB缓冲    
};

#endif // SENDTHREAD_H
