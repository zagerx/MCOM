#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QtDebug>



serialwindow::serialwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::serialwindow)
{
    ui->setupUi(this);
    SerialPortInit();
}

serialwindow::~serialwindow()
{
    delete ui;
}

void serialwindow::SerialPortInit()
{
    serial = new(QSerialPort);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        serial->setPort(info);
        if(serial->open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(info.portName());
            serial->close();
            qDebug() << "串口打开成功";
        }else{
            qDebug() << "串口打开失败，请重试";
        }
    }
}

void serialwindow::on_pushButton_clicked()
{
    QPushButton *openbutton;
    openbutton = ui->pushButton;
    openbutton->setText("xxxx");
    /*打开串口button*/
    /*读取下拉列表当前的串口号*/

}

