/********************************************************************************
** Form generated from reading UI file 'serialwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALWINDOW_H
#define UI_SERIALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialWindow
{
public:

    void setupUi(QWidget *SerialWindow)
    {
        if (SerialWindow->objectName().isEmpty())
            SerialWindow->setObjectName(QString::fromUtf8("SerialWindow"));
        SerialWindow->resize(400, 300);

        retranslateUi(SerialWindow);

        QMetaObject::connectSlotsByName(SerialWindow);
    } // setupUi

    void retranslateUi(QWidget *SerialWindow)
    {
        SerialWindow->setWindowTitle(QCoreApplication::translate("SerialWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialWindow: public Ui_SerialWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALWINDOW_H
