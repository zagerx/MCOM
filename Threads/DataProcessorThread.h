#ifndef DATAPROCESSORTHREAD_H
#define DATAPROCESSORTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QWaitCondition>
#include <QMutex>
#include <QQueue>

#include "Core/ProtocolHandler.h"  // Include the ProtocolHandler header

class DataProcessorThread : public QThread {
    Q_OBJECT
public:
    explicit DataProcessorThread(QObject *parent = nullptr);
public slots:
    void getUIData(const QString name,const QByteArray &data);
    void handleRawData(const QByteArray &data);
protected:
    void run() override;
signals:
    void heart_sigle(void);
    void ui_dataReady_sigle(const QByteArray &data);
private:
    ProtocolHandler protocolHandler;  // Instance of ProtocolHandler
    QWaitCondition *m_waitCondition = new QWaitCondition();   
    QMutex m_mutex; // 互斥锁，用于保护缓冲区
    QQueue<QByteArray> proBuffer;
};

#endif // DATAPROCESSORTHREAD_H
