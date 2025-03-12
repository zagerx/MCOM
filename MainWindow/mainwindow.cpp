#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include "Threads/IOThread.h"
#include "Threads/DataProcessorThread.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    // 初始化模块
    SerialDev = new SerialManager(this);
    mSendThread = new IOThread(this); 
    mDataProThread = new DataProcessorThread(this); // 初始化数据处理线程
    //接收数据流  串口-->IOThead-->DPThread-->UI
    connect(SerialDev,&SerialManager::dataReceived,mSendThread, &IOThread::handleReceivedData);
    connect(mSendThread, &IOThread::dataReadyToProcess,mDataProThread, &DataProcessorThread::processRawData);
    connect(mDataProThread, &DataProcessorThread::heartSignal,this, &MainWindow::toggleLed);
    //发送数据流  UI-->DPThread-->IOThead-->串口
    connect(this, &MainWindow::uiDataReady,mDataProThread, &DataProcessorThread::receiveUIData);
    connect(mDataProThread, &DataProcessorThread::uiDataReadyToSend,mSendThread, &IOThread::sendData);
    connect(mSendThread, &IOThread::dataReadyToSend,SerialDev,&SerialManager::writeData);

    // 连接按钮点击事件
    connect(ui->OpBt, &QPushButton::clicked,this, &MainWindow::onConnectButtonClicked);
    // 连接 verticalLayout 中的单选按钮
    connectVLayout(ui->verticalLayout);
    // 连接 verticalLayout_2 中的单选按钮
    connectVLayout(ui->verticalLayout_2);
    // 连接 verticalLayout_3 中的单选按钮
    ui->checkBox->setChecked(true); // 设置为选中状态
    connectVLayout(ui->verticalLayout_3);
    //连接 vertialcalLayout_4 
    connectVLayout(ui->verticalLayout_4);
    //连接 vertialcalLayout_5
    connectVLayout(ui->verticalLayout_5);
    // 初始化时更新可用串口号
    updateAvailablePorts();
    // 启动接收线程
    mDataProThread->start();
    mSendThread->start();
}

MainWindow::~MainWindow() {
    mDataProThread->requestInterruption(); // 请求中断线程
    mDataProThread->wait(); // 等待线程结束        
    delete ui;
}

void MainWindow::updateAvailablePorts() {
    // 清空comboBox_2
    ui->comboBox_2->clear();
    // 从SerialManager获取可用串口号
    QStringList ports = SerialDev->getAvailablePorts();
    if (ports.isEmpty()) {
        // 如果没有可用的串口号，禁用按钮
        ui->OpBt->setEnabled(false);
        ui->OpBt->setText("无可用串口");
    } else {
        // 如果有可用的串口号，添加到comboBox_2
        ui->comboBox_2->addItems(ports);
        ui->OpBt->setEnabled(true);
        ui->OpBt->setText("打开串口");
    }
}
/*==========================================================================================
 * @brief 
 * @FuncName     打开串口按钮回调 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void MainWindow::onConnectButtonClicked() {
    if (SerialDev->isOpen()) {
        // 如果串口已打开，则关闭串口
        SerialDev->closePort();
        ui->OpBt->setText("打开串口");
        qDebug() << "串口已关闭";
    } else {
        qDebug()<<"串口未打开";
        // 如果串口未打开，则获取对应的串口号及波特率并打开串口
        QString portName = ui->comboBox_2->currentText(); // 从comboBox_2获取串口号
        QString baudRateStr = ui->comboBox->currentText(); // 从comboBox获取波特率
        bool ok;
        int baudRate = baudRateStr.toInt(&ok);
        if (!ok) {
            qDebug() << "波特率转换失败";
            return;
        }

        SerialDev->setBaudRate(baudRate); // 设置波特率
        if (SerialDev->openPort(portName)) {
            ui->OpBt->setText("关闭串口");
            qDebug() << "串口已打开";
        } else {
            qDebug() << "串口打开失败";
        }
    }
}
/*==========================================================================================
 * @brief 
 * @FuncName     单选按钮回调 
 * @param        checked 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void MainWindow::onRadioButtonToggled(bool checked) {
    if (checked) {
        // 获取触发信号的 QRadioButton
        QRadioButton *radioButton = qobject_cast<QRadioButton*>(sender());
        if (radioButton) {
            // 获取 QRadioButton 的父布局
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(radioButton->parentWidget()->layout());
            if (layout) {
                if (layout == ui->verticalLayout) {
                    const QByteArray data;
                    emit uiDataReady(radioButton->text(),data);
                } else if (layout == ui->verticalLayout_2) {
                    const QByteArray data;
                    emit uiDataReady(radioButton->text(),data);
                }
            }
        }
    }
}
// 修改 connectVLayout 函数
void MainWindow::connectVLayout(QLayout *layout) {
    if (!layout) return; // 检查布局是否为空

    // 使用队列来模拟递归处理
    QQueue<QLayout*> layoutQueue;
    layoutQueue.enqueue(layout);

    while (!layoutQueue.isEmpty()) {
        QLayout *currentLayout = layoutQueue.dequeue();

        for (int i = 0; i < currentLayout->count(); ++i) {
            QWidget *widget = nullptr;
            QLayout *subLayout = nullptr;

            // 获取布局项中的控件或子布局
            if (currentLayout->itemAt(i)->widget()) {
                widget = currentLayout->itemAt(i)->widget();
            } else if (currentLayout->itemAt(i)->layout()) {
                subLayout = currentLayout->itemAt(i)->layout();
            }

            // 处理控件
            if (widget) {
                // 连接单选按钮
                if (auto radio = qobject_cast<QRadioButton*>(widget)) {
                    connect(radio, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
                }
                // 连接复选框
                else if (auto check = qobject_cast<QCheckBox*>(widget)) {
                    connect(check, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxStateChanged);
                }
                // 连接按钮
                else if (auto button = qobject_cast<QPushButton*>(widget)) {
                    connect(button, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);
                }
            }
            // 处理子布局
            else if (subLayout) {
                layoutQueue.enqueue(subLayout); // 将子布局加入队列
            }
        }
    }
}
void MainWindow::onPushButtonClicked() {
    // 获取发送信号的按钮
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    qDebug()<<button->text();
    // 获取按钮所在的布局
    QWidget *buttonParent = button->parentWidget();
    if (!buttonParent) return;

    // 获取父控件的布局
    QLayout *layout = buttonParent->layout();
    if (!layout) return;

    // 遍历布局中的控件
    for (int i = 0; i < layout->count(); ++i) {
        QWidget *widget = layout->itemAt(i)->widget();
        if (auto lineEdit = qobject_cast<QLineEdit*>(widget)) {
            // 找到文本框并获取内容
            QString text = lineEdit->text();
            qDebug() << "文本框内容:" << text;
            return;
        }
    }
}
void MainWindow::onCheckBoxStateChanged(int state) {
    QCheckBox *check = qobject_cast<QCheckBox*>(sender());
    if (check) {
        qDebug() << "verticalLayout_3 中的控件被选中:" << check->text() << "状态:" << state;
    }
}
void MainWindow::toggleLed() {
    qDebug()<<"main hear";
}
