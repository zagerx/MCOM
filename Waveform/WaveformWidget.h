#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <QVector>
#include "qcustomplot.h"

class WaveformWidget : public QCustomPlot {
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget *parent = nullptr);
    void appendData(int channel, float value);
    void setChannelVisible(int channel, bool visible);
    void clearData();
    void stopPlotting(); // 添加停止绘制的接口
    void startPlotting();
private slots:
    void refreshPlot();

private:
    QVector<QVector<float>> m_dataBuffers; // 每个通道一个缓冲区
    QVector<int> m_currentIndices; // 每个通道的当前索引
    QTimer* m_refreshTimer = nullptr;
    QVector<bool> m_channelVisible; // 每个通道的可见性状态
    bool m_isPlotting; // 添加布尔变量，用于记录是否在绘制
};

#endif // WAVEFORMWIDGET_H