#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Files nupu vajutus
void MainWindow::on_btnFiles_clicked()
{
    // Loob faili valimise akna
    QFileDialog dialog(this);

    dialog.setWindowTitle("Select output file"); // Määrab akna nime
    dialog.setAcceptMode(QFileDialog::AcceptSave); // määrab salvestusrežiimi
    dialog.setOption(QFileDialog::DontConfirmOverwrite, true); // Eemaldab ülekirjutamis teate
    dialog.setNameFilter("Text files (*.txt)"); // kuvab ja salvestab .txt faile

    // Kontroll kas aken sulgeti (cancel või X)
    if (!dialog.exec())
    {
        return;
    }

    // Salvestab kasutaja valitud failid
    QStringList selectedFiles = dialog.selectedFiles();

    // Kontroll kas fail vailiti
    if (selectedFiles.isEmpty())
    {
        return;
    }

    // Võtab listist esimese faili
    QString fileName = selectedFiles.first();

    outputFile.setFileName(fileName);

    // Valitakse faili abamise režiimid
    QIODevice::OpenMode openMode = QIODevice::WriteOnly | QIODevice::Text;

    // Kontroll kas fail on juba olemas
    if (QFile::exists(fileName))
    {
        // Kuvatakse sõnumi kast, kas fail üle kirjutada või jätkata
        QMessageBox::StandardButton reply =
            QMessageBox::question(
                this,
                "File Exists",
                "Append to existing file?",
                QMessageBox::Yes |
                QMessageBox::No |
                QMessageBox::Cancel);

        // Vastavalt valikule lisatakse režiimi juurde tingimus
        if (reply == QMessageBox::Yes)
        {
            openMode |= QIODevice::Append;
        }
        else if (reply == QMessageBox::No)
        {
            openMode |= QIODevice::Truncate;
        }
        else
        {
            return;
        }
    }

    // Fail avatakse, kui sellist faili pole siis luuakse
    if (!outputFile.open(openMode))
    {
        QMessageBox::critical(this, "File Error", "Could not open selected file.");
        return;
    }

    // Kutsutakse välja DLL funktsioon DataProduceri initsialiseerimiseks
    if (!CW2_Initialize())
    {
        ui->textBox->appendPlainText("DLL initialization failed.");
        outputFile.close();
        return;
    }

    ui->textBox->appendPlainText("DLL initialized.");

    ui->btnFiles->setEnabled(false);
    ui->btnStart->setEnabled(true);
}

// Start nupu vajutus
void MainWindow::on_btnStart_clicked()
{
    // Loob thread'i andmete lugemiseks
    readerThread = new ReaderThread(&guiControl, this);

    // Ühendatakse textReady signaal funktsiooniga handleTextReady
    connect(readerThread,
            &ReaderThread::textReady,
            this,
            &MainWindow::handleTextReady);

    // Käivitatakse andmete genereerimine
    if (!CW2_Start(&guiControl))
    {
        ui->textBox->appendPlainText("Generation start failed.");

        delete readerThread;
        readerThread = nullptr;
        return;
    }

    ui->textBox->appendPlainText("Generation started.");

    readerThread->start(); // Käivit andmete lugemis threadi ja run() funktsiooni

    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

// Stop nupu vajutus
void MainWindow::on_btnStop_clicked()
{
    stopGenerator();
}

// Exit nupu vajutus
void MainWindow::on_btnExit_clicked()
{
    close();
}

// Kuvab ja salvestab DLL saadud andmed
void MainWindow::handleTextReady(QString text)
{
    ui->textBox->appendPlainText(text); // andmed kuvatakse

    // Kui fail on avatud andmed salvestatakse
    if (outputFile.isOpen())
    {
        QTextStream out(&outputFile);
        out << text << "\n";
        out.flush();
    }
}

// Peatab andmete genereerimise
void MainWindow::stopGenerator()
{
    CW2_Stop(); // Generaator seisatatakse

    // lugemis thread'i töö lõpetatakse
    if (readerThread)
    {
        readerThread->wait(); // Ootab kuni thread lõpetab töö
        delete readerThread;; // Thread kustutatakse
        readerThread = nullptr;
    }

    ui->textBox->appendPlainText("Generation stopped.");

    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Andmete Genereerimine peatatakse
    if (readerThread)
    {
        stopGenerator();
    }

    CW2_Shutdown(); // DLL'd vabastatakse

    // Kui fail on lahti sulgetakse see
    if (outputFile.isOpen())
    {
        outputFile.close();
    }

    // sulgeb programmi
    event->accept();
}
