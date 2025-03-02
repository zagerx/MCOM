#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include "HardwareManager/SerialManager.h"
#include "Core/ProtocolHandler.h"

class SendThread : public QThread {
    Q_OBJECT
public:
    SendThread(SerialManager *serial, ProtocolHandler *protocol, QObject *parent = nullptr);
    void run() override;

private:
    SerialManager *SerialDev;
    ProtocolHandler *m_protocol;
};

#endif // SENDTHREAD_H
