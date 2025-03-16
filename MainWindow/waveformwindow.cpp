#include "waveformwindow.h"

WaveformWindow::WaveformWindow(QWidget *parent) : QMainWindow(parent) {

    m_waveform = new WaveformWidget(this);
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    m_channelCheckboxes.resize(8);
    for (int i = 0; i < 8; ++i) {
        m_channelCheckboxes[i] = new QCheckBox(QString("ch%1").arg(i + 1), this);
        m_channelCheckboxes[i]->setChecked(true); //
        layout->addWidget(m_channelCheckboxes[i]);
        
        connect(m_channelCheckboxes[i], &QCheckBox::stateChanged, [this, i](int state) {
            bool visible = (state == Qt::Checked);
            m_waveform->setChannelVisible(i, visible);
        });
    }
    
    layout->addWidget(m_waveform);
    setCentralWidget(centralWidget);
    
    setWindowTitle("Waveform Window");
    resize(800, 600);
}

WaveformWindow::~WaveformWindow() {
    delete m_waveform;
    for (auto checkbox : m_channelCheckboxes) {
        delete checkbox;
    }
}

void WaveformWindow::appendData(int channel, float value) {
    m_waveform->appendData(channel, value);
}

void WaveformWindow::setChannelVisible(int channel, bool visible) {
    m_waveform->setChannelVisible(channel, visible);
}
