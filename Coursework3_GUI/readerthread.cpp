#include "readerthread.h"

// Konstruktor
ReaderThread::ReaderThread(Control_GUI* control, QObject* parent)
                           : QThread(parent), control(control){}


// Loeb andmeid puhvrist ja saadab need GUI-le.
void ReaderThread::run()
{
    while (true)
    {
        // Loob luku ja lukustab mutexi
        unique_lock<mutex> lock(control->mx);

        // Ootab, kuni puhvris on tekst või stop-lipp muutub tõeseks
        control->cv.wait(lock, [&]()
            {
                return !control->buffer.str().empty() || control->stop;
            });

        // Loeb teksti puhvrist
        string text = control->buffer.str();

        // Puhastab puhvri
        control->buffer.str("");
        control->buffer.clear();

        // Kui stop on aktiveeritud ja puhver on tühi väljub tsükklist
        if (text.empty() && control->stop)
            break;

        lock.unlock();

        // Saadab loetud teksti GUI-le
        emit textReady(QString::fromLatin1(text.c_str()));
    }
}
