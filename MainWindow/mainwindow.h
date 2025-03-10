#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include "HardwareManager/SerialManager.h"
#include "Threads/DataProcessorThread.h"
#include "Threads/IOThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void uiDataReady(const QString name, const QByteArray &data);
private slots:
    void onConnectButtonClicked();
    void onRadioButtonToggled(bool checked); // 单选按钮回调函数
    void toggleLed(void);
private:
    void updateAvailablePorts(); // 更新可用串口号
    void connectVLayout(QVBoxLayout *layout);
    Ui::MainWindow *ui;
    SerialManager *SerialDev;
    DataProcessorThread *mDataProThread; // 数据处理线程
    IOThread *mSendThread; // 新增接收线程
};

#endif // MAINWINDOW_H
