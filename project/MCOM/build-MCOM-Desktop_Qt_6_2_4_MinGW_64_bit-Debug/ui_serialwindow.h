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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serialwindow
{
public:
    QComboBox *comboBox;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QComboBox *portBox;

    void setupUi(QWidget *serialwindow)
    {
        if (serialwindow->objectName().isEmpty())
            serialwindow->setObjectName(QString::fromUtf8("serialwindow"));
        serialwindow->resize(400, 300);
        comboBox = new QComboBox(serialwindow);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(300, 10, 71, 21));
        pushButton = new QPushButton(serialwindow);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(300, 120, 75, 23));
        textEdit = new QTextEdit(serialwindow);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(0, 0, 281, 141));
        portBox = new QComboBox(serialwindow);
        portBox->setObjectName(QString::fromUtf8("portBox"));
        portBox->setGeometry(QRect(300, 40, 71, 21));

        retranslateUi(serialwindow);

        QMetaObject::connectSlotsByName(serialwindow);
    } // setupUi

    void retranslateUi(QWidget *serialwindow)
    {
        serialwindow->setWindowTitle(QCoreApplication::translate("serialwindow", "\344\270\262\345\217\243\350\260\203\350\257\225\347\225\214\351\235\242", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("serialwindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("serialwindow", "9600", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("serialwindow", "115200", nullptr));

        pushButton->setText(QCoreApplication::translate("serialwindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serialwindow: public Ui_serialwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALWINDOW_H
