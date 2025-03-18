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

        for (int i = 0; i < 8; ++i) {
            connect(m_channelCheckboxes[i], &QCheckBox::stateChanged, this, [this, i](int state) {
                bool visible = (state == Qt::Checked);
                setChannelVisible(i, visible); // 调用 WaveformWindow 的方法
            });
        }
    }
    // 添加按钮
    m_plotControlButton = new QPushButton("开始绘制", centralWidget);
    layout->addWidget(m_plotControlButton);

    // 连接按钮的点击信号到槽函数
    connect(m_plotControlButton, &QPushButton::clicked, this, &WaveformWindow::togglePlot);

    // 初始化布尔变量
    m_isPlotting = false;

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
    delete m_plotControlButton; // 删除按钮
}

void WaveformWindow::appendData(int channel, float value) {
    m_waveform->appendData(channel, value);
}

void WaveformWindow::setChannelVisible(int channel, bool visible) {
    m_waveform->setChannelVisible(channel, visible);
}
void WaveformWindow::togglePlot() {
    if (!m_isPlotting) {
        // 如果没有在绘制，开始绘制
        m_isPlotting = true;
        m_plotControlButton->setText("停止绘制");
        m_waveform->clearData(); // 清空波形数据
        m_waveform->startPlotting();
    } else {
        // 如果正在绘制，停止绘制并清空波形
        m_isPlotting = false;
        m_waveform->stopPlotting(); // 停止绘制
        m_plotControlButton->setText("开始绘制");
    }
}
