#include "waveformwindow.h"
WaveformWindow::WaveformWindow(QWidget *parent) : QMainWindow(parent) {
    m_waveform = new WaveformWidget(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 不将波形部件添加到布局中，而是直接设置其位置和大小
    m_waveform->setGeometry(0, 0, 700, 400);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_channelCheckboxes.resize(8);
    for (int i = 0; i < 8; ++i) {
        m_channelCheckboxes[i] = new QCheckBox(QString("ch%1").arg(i + 1), centralWidget);
        m_channelCheckboxes[i]->setChecked(true);

        layout->addWidget(m_channelCheckboxes[i]);

        connect(m_channelCheckboxes[i], &QCheckBox::stateChanged, [this, i](int state) {
            bool visible = (state == Qt::Checked);
            m_waveform->setChannelVisible(i, visible);
        });
    }

    // 获取波形部件的宽度
    int x = m_waveform->width();

    // 设置布局的起始位置为 (x, 0)
    layout->setContentsMargins(40, 10, 10, 10);
    layout->setSpacing(5);
    layout->setContentsMargins(750, 0, 0, 0);
    layout->addStretch();
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
