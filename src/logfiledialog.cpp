/// Implementation of logfiledialog.h
/** \file logfiledialog.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "logfiledialog.h"
#include "ui_logfiledialog.h"

LogFileDialog::LogFileDialog(QWidget* parent, QString fileName) :
QDialog(parent),
ui(new Ui::LogFileDialog) {
    ui->setupUi(this);
    this->mainWindow = (MainWindow*) parent;
    this->fileName = fileName;

    QPalette palette;
    palette.setColor(QPalette::Base, Qt::lightGray); // QColor(240, 240, 240) didn't work out of QtCreator
    ui->txt_fileName->setReadOnly(true);
    ui->txt_fileName->setPalette(palette);
    ui->txt_formatName->setReadOnly(true);
    ui->txt_formatName->setPalette(palette);

    if (this->fileName.isNull()) { // add log file was pressed
        addLogFile();
    }

    if (isValidFile()) {
        fillFields();
    }
}

LogFileDialog::~LogFileDialog() {
    delete ui;
}

void LogFileDialog::on_btn_saveAndExit_clicked() {
    if (!isValidFile()) {
        this->reject();
    } else {
        this->mainWindow->getModel()->updateLogDialogAliases(ui->txt_dateAliases->toPlainText().toUtf8().constData());
        this->mainWindow->getModel()->modifyInputFile(ui->txt_fileName->toPlainText().toUtf8().constData(), ui->txt_formatName->toPlainText().toUtf8().constData(), ui->lnE_dateRegex->text().toUtf8().constData());

        string missingElement;
        if (!this->mainWindow->getModel()->validateDateParsingRegex(ui->lnE_dateRegex->text().toUtf8().constData(), missingElement)) {
            QString message = "Mandatory field ";
            message = message.append(missingElement.c_str());
            message = message.append(" is missing in date parsing regex, please fix it in order to be able to parse event time from this log.");

            QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, "Error", message, QMessageBox::Ok);
            messageBox->exec();
            delete messageBox;
            this->reject();
        } else {
            QString assigment = "${";
            assigment = assigment.append(ui->txt_formatName->toPlainText());
            assigment = assigment.append("}=");
            assigment = assigment.append(ui->lnE_dateRegex->text());
            this->mainWindow->getModel()->addLogDialogAlias(assigment.toUtf8().constData());
            this->accept();
        }
    }

}

void LogFileDialog::setInvalidFields() {
    ui->txt_fileName->setPlainText("INVALID");
    ui->txt_formatName->setPlainText("INVALID");
    ui->lnE_dateRegex->setText("INVALID");
    ui->txt_dateAliases->setPlainText("INVALID");

    this->fileName = "INVALID";
}

bool LogFileDialog::isValidFile() {
    return string(this->fileName.toUtf8().constData()) != "INVALID";
}

void LogFileDialog::addLogFile() {
    this->fileName = QFileDialog::getOpenFileName(this, "Select log file to open", QDir::homePath()); // or "."
    if (this->fileName.isNull()) {
        QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, "Error", "Error when opening a new file - no file selected, please exit the dialog, nothing will be saved!", QMessageBox::Ok);
        messageBox->exec();
        delete messageBox;

        this->setInvalidFields();
        this->reject();
    } else {
        bool valid;
        valid = this->mainWindow->getModel()->addInputFile(this->fileName.toUtf8().constData(), CFsFunctions::generateFormatNameFromFileName(fileName.toUtf8().constData()).c_str(), "");
        if (valid == false) {
            QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, "Error", "You selected file that was already added, please exit the dialog and use Modify option!", QMessageBox::Ok);
            messageBox->exec();
            delete messageBox;

            this->setInvalidFields();
            this->reject();
        } else {
            this->mainWindow->getLogFilesController()->addFileName(this->fileName, true);
        }
    }
}

void LogFileDialog::fillFields() {
    ui->txt_fileName->setPlainText(this->fileName);
    string formatName = this->mainWindow->getModel()->getFormatName(this->fileName.toUtf8().constData());
    ui->txt_formatName->setPlainText(formatName.c_str());
    ui->lnE_dateRegex->setText(this->mainWindow->getModel()->getFormatRegex(formatName).c_str());
    ui->txt_dateAliases->setPlainText(this->mainWindow->getModel()->getLogDialogAliases().c_str());
}
