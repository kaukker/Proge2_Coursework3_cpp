#ifndef READERTHREAD_H
#define READERTHREAD_H

#include <QThread>
#include <QString>
#include "coursework2dll.h"

using namespace std;

class ReaderThread : public QThread
{
    Q_OBJECT

public:
    explicit ReaderThread(Control_GUI* control, QObject* parent = nullptr);
    ~ReaderThread() = default;

signals:
    void textReady(QString text);

protected:
    void run() override;

private:
    Control_GUI* control;
};

#endif
