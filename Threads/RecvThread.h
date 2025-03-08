#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
class RecvThread : public QThread {
    Q_OBJECT
public:
    explicit RecvThread(QObject *parent = nullptr);

protected:
    void run() override;
signals:
    void rawDataReceived(const QByteArray &data);
public slots:
    void processData(const QByteArray &data); // 槽函数：处理数据

private:
    QQueue<QByteArray> ReciveBuffer; // 数据缓冲区
    QMutex m_mutex; // 互斥锁，用于保护缓冲区
    QWaitCondition *m_waitCondition = new QWaitCondition(); // 新增等待条件
};

#endif // RECVTHREAD_H