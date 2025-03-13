#include "WaveformWidget.h"
#include <QTimer>

WaveformWidget::WaveformWidget(QWidget *parent) 
    : QCustomPlot(parent), 
      m_dataBuffer(2000), // 存储2000个数据点
      m_currentIndex(0)
{
    // 初始化绘图
    addGraph();
    graph(0)->setPen(QPen(Qt::blue));
    
    // 设置坐标轴
    xAxis->setLabel("Time");
    yAxis->setLabel("Value");
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    
    // 定时刷新界面（30FPS）
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &WaveformWidget::refreshPlot);
    refreshTimer->start(33);
    
    // 初始化数据缓冲区
    m_dataBuffer.fill(0);
}

void WaveformWidget::appendData(int channel, float value)
{
    Q_UNUSED(channel); // 目前只处理第一个通道
    
    // 环形缓冲区存储数据
    m_dataBuffer[m_currentIndex] = value;
    m_currentIndex = (m_currentIndex + 1) % m_dataBuffer.size();
}

void WaveformWidget::refreshPlot()
{
    static QVector<double> x, y;
    x.clear();
    y.clear();
    
    // 准备最新500个数据点
    const int displayPoints = qMin(500, m_dataBuffer.size());
    int startIndex = (m_currentIndex - displayPoints + m_dataBuffer.size()) % m_dataBuffer.size();
    
    for(int i=0; i<displayPoints; ++i){
        x.append(i);
        y.append(m_dataBuffer[(startIndex + i) % m_dataBuffer.size()]);
    }
    
    graph(0)->setData(x, y);
    
    // 自动调整Y轴范围
    auto [min, max] = std::minmax_element(y.begin(), y.end());
    yAxis->setRange(*min - 0.1, *max + 0.1);
    xAxis->setRange(0, displayPoints);
    
    replot();
}