/// Implementation of configfiledialog.h
/** \file configfiledialog.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "configfiledialog.h"
#include "ui_configfiledialog.h"

ConfigFileDialog::ConfigFileDialog(QWidget* parent, QString fileName) :
QDialog(parent),
ui(new Ui::ConfigFileDialog),
m_ruleGenerator(new CRuleGenerator()) {
    ui->setupUi(this);
    this->mainWindow = (MainWindow*) parent;
    this->fileName = fileName;

    QPalette palette;
    palette.setColor(QPalette::Base, Qt::lightGray); // QColor(240, 240, 240) didn't work out of QtCreator
    ui->txt_confName->setReadOnly(true);
    ui->txt_confName->setPalette(palette);

    if (this->fileName.isNull()) { // add config file was pressed
        addConfigFile();
    }

    if (isValidFile()) {
        fillFields();
    }
}

ConfigFileDialog::~ConfigFileDialog() {
    delete m_ruleGenerator;
    delete ui;
}

void ConfigFileDialog::on_btn_saveAndExit_clicked() {
    if (!isValidFile()) {
        this->reject();
    } else {
        this->mainWindow->getModel()->updateConfigDialogAlises(this->ui->txt_aliases->toPlainText().toUtf8().constData());
        writeToFile(this->fileName);
        this->accept();
    }
}

void ConfigFileDialog::setInvalidFields() {
    ui->txt_confName->setPlainText("INVALID");
    ui->txt_configFile->setPlainText("INVALID");
    ui->txt_aliases->setPlainText("INVALID");

    this->fileName = "INVALID";
}

bool ConfigFileDialog::isValidFile() {
    return string(this->fileName.toUtf8().constData()) != "INVALID";
}

void ConfigFileDialog::addConfigFile() {
    this->fileName = QFileDialog::getOpenFileName(this, "Select configuration file to open", QDir::homePath()); // or "."
    if (this->fileName.isNull()) {
        QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, "Error", "Error when opening a new file - no file selected, please exit the dialog, nothing will be saved!", QMessageBox::Ok);
        messageBox->exec();
        delete messageBox;

        this->setInvalidFields();
        this->reject();
    } else {
        bool valid;
        valid = this->mainWindow->getModel()->addConfigFile(this->fileName.toUtf8().constData());
        if (valid == false) {
            QMessageBox* messageBox = new QMessageBox(QMessageBox::Critical, "Error", "You selected file that was already added, please exit the dialog and use Modify option!", QMessageBox::Ok);
            messageBox->exec();
            delete messageBox;

            this->setInvalidFields();
            this->reject();
        } else {
            this->mainWindow->getConfigFilesController()->addFileName(this->fileName, true);
        }
    }
}

void ConfigFileDialog::fillFields() {
    ui->txt_confName->setPlainText(this->fileName);
    readFromFile(this->fileName);
    ui->txt_aliases->setPlainText(this->mainWindow->getModel()->getConfigDialogAliases().c_str());
}

bool ConfigFileDialog::readFromFile(QString fileName) {
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    this->ui->txt_configFile->setPlainText(fileContent);

    file.close();
    return true;
}

bool ConfigFileDialog::writeToFile(QString fileName) {
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QTextStream out(&file);

    QString fileContent = this->ui->txt_configFile->toPlainText();
    QString evaluatedFileContent = this->mainWindow->getModel()->evaluateInputAccordingToConfigAliases(fileContent.toUtf8().constData()).c_str();

    out << evaluatedFileContent;

    file.close();
    return true;
}

void ConfigFileDialog::on_btn_generate_clicked() {
    QString ruleType = this->ui->cmb_ruleType->currentText();
    this->ui->txt_configFile->appendPlainText(this->m_ruleGenerator->generateRule(ruleType));
}
