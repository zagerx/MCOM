#ifndef WAVEFORMWINDOW_H
#define WAVEFORMWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QVector>
#include "waveformwidget.h"

class WaveformWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit WaveformWindow(QWidget *parent = nullptr);
    ~WaveformWindow();

public slots:
    void appendData(int channel, float value);
    void setChannelVisible(int channel, bool visible);

private:
    WaveformWidget *m_waveform;
    QVector<QCheckBox*> m_channelCheckboxes;
};

#endif // WAVEFORMWINDOW_H