#ifndef WAVEFORMWINDOW_H
#define WAVEFORMWINDOW_H

#include <QMainWindow>
#include "waveformwidget.h"

class WaveformWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit WaveformWindow(QWidget *parent = nullptr);
    ~WaveformWindow();

public slots:
    void appendData(int channel, float value);

private:
    WaveformWidget *m_waveform;
};

#endif // WAVEFORMWINDOW_H