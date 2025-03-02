#include "WaveformWidget.h"

// WaveformWidget::WaveformWidget(QWidget *parent) : QCustomPlot(parent), m_currentX(0) {
//     // 初始化绘图
//     addGraph();
//     xAxis->setLabel("时间");
//     yAxis->setLabel("角度");
    
//     // 配置定时刷新
//     m_refreshTimer = new QTimer(this);
//     connect(m_refreshTimer, &QTimer::timeout, [this](){
//         if(m_data.size() > 0) {
//             QVector<double> xData(m_data.size());
//             std::iota(xData.begin(), xData.end(), m_currentX - m_data.size());
//             graph(0)->addData(xData, m_data);
//             m_data.clear();
//             rescaleAxes();
//             replot();
//         }
//     });
//     m_refreshTimer->start(50); // 20Hz刷新
// }

// void WaveformWidget::addDataPoint(double value) {
//     m_data.append(value);
//     m_currentX++;
// }
