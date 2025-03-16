#include "waveformwindow.h"

WaveformWindow::WaveformWindow(QWidget *parent) : QMainWindow(parent) {
    m_waveform = new WaveformWidget(this);
    setCentralWidget(m_waveform);

    // 设置窗口标题和大小
    setWindowTitle("Waveform Window");
    resize(800, 600);
}

WaveformWindow::~WaveformWindow() {
    delete m_waveform;
}

void WaveformWindow::appendData(int channel, float value) {
    m_waveform->appendData(channel, value);
}