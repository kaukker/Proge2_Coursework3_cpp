#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "coursework2dll.h"
#include "readerthread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class mainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_btnExit_clicked();

    void on_btnFiles_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void handleTextReady(QString text);

private:
    Ui::mainWindow *ui;
    Control_GUI guiControl;
    ReaderThread* readerThread = nullptr;
    QFile outputFile;
    void stopGenerator();
};
#endif
