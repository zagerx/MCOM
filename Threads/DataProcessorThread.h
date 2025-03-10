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
    ~DataProcessorThread();
public slots:
    void receiveUIData(const QString name,const QByteArray &data);
    void processRawData(const QByteArray &data);
protected:
    void run() override;
signals:
    void heartSignal(void);
    void uiDataReadyToSend(const QByteArray &data);
private:
    QMutex m_mutex;
    QWaitCondition* m_waitCondition;
    QQueue<QByteArray> proBuffer;
    ProtocolHandler protocolHandler;
};

#endif // DATAPROCESSORTHREAD_H
