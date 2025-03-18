#ifndef WAVEFORMWINDOW_H
#define WAVEFORMWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QVector>
#include "waveformwidget.h"
#include <QPushButton> // 添加按钮头文件

class WaveformWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit WaveformWindow(QWidget *parent = nullptr);
    ~WaveformWindow();

public slots:
    void appendData(int channel, float value);
    void setChannelVisible(int channel, bool visible);
    void togglePlot(); // 添加新的槽函数

private:
    WaveformWidget *m_waveform;
    QVector<QCheckBox*> m_channelCheckboxes;
    QPushButton *m_plotControlButton; // 添加按钮成员变量
    bool m_isPlotting; // 添加布尔变量，用于记录是否在绘制
};

#endif // WAVEFORMWINDOW_H