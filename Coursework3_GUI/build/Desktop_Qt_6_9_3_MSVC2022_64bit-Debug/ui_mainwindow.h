/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *textBox;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnFiles;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnExit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName("mainWindow");
        mainWindow->resize(631, 657);
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName("gridLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        textBox = new QPlainTextEdit(groupBox);
        textBox->setObjectName("textBox");
        textBox->setReadOnly(true);

        verticalLayout_2->addWidget(textBox);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName("gridLayout_4");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(30);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        btnFiles = new QPushButton(groupBox_2);
        btnFiles->setObjectName("btnFiles");

        horizontalLayout_3->addWidget(btnFiles);

        btnStart = new QPushButton(groupBox_2);
        btnStart->setObjectName("btnStart");

        horizontalLayout_3->addWidget(btnStart);

        btnStop = new QPushButton(groupBox_2);
        btnStop->setObjectName("btnStop");

        horizontalLayout_3->addWidget(btnStop);

        btnExit = new QPushButton(groupBox_2);
        btnExit->setObjectName("btnExit");

        horizontalLayout_3->addWidget(btnExit);


        gridLayout_4->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 1, 1, 1);

        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 631, 21));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName("statusbar");
        mainWindow->setStatusBar(statusbar);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("mainWindow", "Observation results", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("mainWindow", "Operations", nullptr));
        btnFiles->setText(QCoreApplication::translate("mainWindow", "Files", nullptr));
        btnStart->setText(QCoreApplication::translate("mainWindow", "Start", nullptr));
        btnStop->setText(QCoreApplication::translate("mainWindow", "Stop", nullptr));
        btnExit->setText(QCoreApplication::translate("mainWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
