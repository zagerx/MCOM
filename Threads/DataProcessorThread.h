#ifndef DATAPROCESSORTHREAD_H
#define DATAPROCESSORTHREAD_H

#include <QThread>
#include <QObject>
#include <QByteArray>
#include "Threads/RecvThread.h"
#include "Core/ProtocolHandler.h"  // Include the ProtocolHandler header

class DataProcessorThread : public QThread {
    Q_OBJECT
public:
    explicit DataProcessorThread(RecvThread *recvThread, QObject *parent = nullptr);

protected:
    void run() override;
signals:
    void heart_sigle(void);
private:
    RecvThread *data_soure;
    ProtocolHandler protocolHandler;  // Instance of ProtocolHandler
};

#endif // DATAPROCESSORTHREAD_H