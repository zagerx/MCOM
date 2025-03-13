#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <QVector>
#include "qcustomplot.h"

class WaveformWidget : public QCustomPlot {
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget *parent = nullptr);
    
public slots:
    void appendData(int channel, float value);

private slots:
    void refreshPlot();

private:
    QVector<float> m_dataBuffer; // 环形缓冲区
    int m_currentIndex = 0;
    QTimer* m_refreshTimer = nullptr;
};

#endif // WAVEFORMWIDGET_H