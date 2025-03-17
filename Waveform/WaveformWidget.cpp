#include "WaveformWidget.h"
#include <QTimer>

WaveformWidget::WaveformWidget(QWidget *parent) 
    : QCustomPlot(parent)
{
    // 初始化支持8个通道
    m_dataBuffers.resize(8);
    m_currentIndices.resize(8);
    m_channelVisible.resize(8, true); // 默认所有通道可见

    for (int i = 0; i < 8; ++i) {
        m_dataBuffers[i].resize(2000); // 每个通道2000个数据点
        m_dataBuffers[i].fill(0);
        m_currentIndices[i] = 0;
    }

    // 初始化绘图
    for (int i = 0; i < 8; ++i) {
        addGraph();
        graph(i)->setPen(QPen(QColor::fromHsv((i * 360 / 8) % 360, 255, 255)));
    }
    
    // 设置坐标轴
    xAxis->setLabel("Time");
    yAxis->setLabel("Value");
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    
    // 定时刷新界面（30FPS）
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &WaveformWidget::refreshPlot);
    refreshTimer->start(33);
}

void WaveformWidget::appendData(int channel, float value) {
    if (channel < 0 || channel >= 8) return; // 通道超出范围
    
    // 环形缓冲区存储数据
    m_dataBuffers[channel][m_currentIndices[channel]] = value;
    m_currentIndices[channel] = (m_currentIndices[channel] + 1) % m_dataBuffers[channel].size();
}

void WaveformWidget::setChannelVisible(int channel, bool visible) {
    if (channel < 0 || channel >= 8) return; // 通道超出范围
    m_channelVisible[channel] = visible;
}

void WaveformWidget::refreshPlot() {
    for (int i = 0; i < 8; ++i) {
        if (!m_channelVisible[i]) {
            graph(i)->data()->clear();
            continue;
        }

        static QVector<double> x, y;
        x.clear();
        y.clear();
        
        // 准备最新500个数据点
        const int displayPoints = qMin(500, m_dataBuffers[i].size());
        int startIndex = (m_currentIndices[i] - displayPoints + m_dataBuffers[i].size()) % m_dataBuffers[i].size();
        
        for(int j = 0; j < displayPoints; ++j){
            x.append(j);
            y.append(m_dataBuffers[i][(startIndex + j) % m_dataBuffers[i].size()]);
        }
        
        graph(i)->setData(x, y);
    }
    
    // 自动调整Y轴范围
    QVector<float> allData;
    for (int i = 0; i < 8; ++i) {
        if (m_channelVisible[i]) {
            allData += m_dataBuffers[i].mid(m_currentIndices[i], 500);
        }
    }
    
    if (!allData.isEmpty()) {
        auto [min, max] = std::minmax_element(allData.begin(), allData.end());
        yAxis->setRange(*min - 0.1, *max + 0.1);
    }
    yAxis->setRange(1.0,-1.0);
    xAxis->setRange(0, 500);
    
    replot();
}
