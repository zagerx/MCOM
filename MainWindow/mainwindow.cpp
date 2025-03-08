#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include "Threads/RecvThread.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    // 初始化模块
    SerialDev = new SerialManager(this);
    mRecvThread = new RecvThread(this); // 初始化接收线程
    mDataProThread = new DataProcessorThread(mRecvThread, this); // 初始化数据处理线程

    connect(SerialDev,&SerialManager::dataReceived,this,&MainWindow::handleDataReceived);
    // 连接按钮点击事件
    connect(ui->OpBt, &QPushButton::clicked,this, &MainWindow::onConnectButtonClicked);
    // 连接 verticalLayout 中的单选按钮
    connectVLayout(ui->verticalLayout);
    // 连接 verticalLayout_2 中的单选按钮
    connectVLayout(ui->verticalLayout_2);
    // 连接 verticalLayout_3 中的单选按钮
    ui->checkBox->setChecked(true); // 设置为选中状态
    connectVLayout(ui->verticalLayout_3);
    // 连接 MainWindow 的信号到 RecvThread 的槽函数
    connect(this, &MainWindow::dataToProcess, mRecvThread, &RecvThread::processData);
    //连接
    connect(mDataProThread, &DataProcessorThread::heart_sigle,this, &MainWindow::toggle_led);

    // 初始化时更新可用串口号
    updateAvailablePorts();
    // 启动接收线程
    mRecvThread->start();  
    mDataProThread->start();    
}

MainWindow::~MainWindow() {
    mRecvThread->requestInterruption(); // 请求中断线程
    mRecvThread->wait(); // 等待线程结束 
    mDataProThread->requestInterruption(); // 请求中断线程
    mDataProThread->wait(); // 等待线程结束        
    delete ui;
}
void MainWindow::handleDataReceived(const QByteArray &data) {
    emit dataToProcess(data);
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
void MainWindow::onRadioButtonToggled(bool checked) {
    if (checked) {
        // 获取触发信号的 QRadioButton
        QRadioButton *radioButton = qobject_cast<QRadioButton*>(sender());
        if (radioButton) {
            // 获取 QRadioButton 的父布局
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(radioButton->parentWidget()->layout());
            if (layout) {
                if (layout == ui->verticalLayout) {
                    qDebug() << "verticalLayout 中的按钮被选中:" << radioButton->text();
                    // 构建要发送的数据
                } else if (layout == ui->verticalLayout_2) {
                    qDebug() << "verticalLayout_2 中的按钮被选中:" << radioButton->text();
                }
            }
        }
    }
}
// 修改 connectVLayout 函数
void MainWindow::connectVLayout(QVBoxLayout *layout) {
    for (int i = 0; i < layout->count(); ++i) {
        QWidget *widget = layout->itemAt(i)->widget();
        // 同时连接单选按钮和复选框
        if (auto radio = qobject_cast<QRadioButton*>(widget)) {
            connect(radio, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
        } else if (auto check = qobject_cast<QCheckBox*>(widget)) 
        {
            connect(check, &QCheckBox::stateChanged, this, [=](int state){
                QVBoxLayout *layout = ui->verticalLayout_3;
                qDebug() << "verticalLayout_3 中的控件被选中:" << check->text() << "状态:" << state;
            });
        }
    }
}void MainWindow::toggle_led() {
    qDebug()<<"main hear";
}
