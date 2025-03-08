#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include "HardwareManager/SerialManager.h"
#include "Threads/RecvThread.h"
#include "Threads/DataProcessorThread.h"
#include "Threads/SendThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void UIdataReadysig(const QString name, const QByteArray &data);
private slots:
    void onConnectButtonClicked();
    void onRadioButtonToggled(bool checked); // 单选按钮回调函数
    void toggle_led(void);
private:
    void updateAvailablePorts(); // 更新可用串口号
    void connectVLayout(QVBoxLayout *layout);
    Ui::MainWindow *ui;
    SerialManager *SerialDev;
    RecvThread *mRecvThread; // 新增接收线程
    DataProcessorThread *mDataProThread; // 数据处理线程
    SendThread *mSendThread; // 新增接收线程
};

#endif // MAINWINDOW_H
