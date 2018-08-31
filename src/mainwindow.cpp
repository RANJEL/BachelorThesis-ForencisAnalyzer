/// Implementation of mainwindow.h
/** \file mainwindow.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent),
ui(new Ui::MainWindow) {
    ui->setupUi(this);
    model = new CModel();
    logFilesController = new QtFileController(this, this->ui->menuInput_Log_Files, this->ui->menuModify_Log, this->ui->menuDelete_Log, true);
    configFilesController = new QtFileController(this, this->ui->menuConfiguration_Files, this->ui->menuModify_Config, this->ui->menuDelete_Config, false);
    loadBackup();
    updateFixedAndTimeParams();

    QPalette palette;
    palette.setColor(QPalette::Base, Qt::lightGray); // QColor(240, 240, 240) didn't work out of QtCreator
    ui->txt_input->setReadOnly(true);
    ui->txt_input->setPalette(palette);
    ui->txt_config->setReadOnly(true);
    ui->txt_config->setPalette(palette);
    ui->txt_fixed->setReadOnly(true);
    ui->txt_fixed->setPalette(palette);
}

MainWindow::~MainWindow() {
    delete model;
    delete logFilesController;
    delete configFilesController;
    delete ui;
}

void MainWindow::loadBackup() {
    set<string> allInputFiles = this->model->getAllInputFiles();
    for (auto it = allInputFiles.begin(); it != allInputFiles.end(); it++) {
        QString inputFileName = it->c_str();
        this->logFilesController->addFileName(inputFileName, false);
    }

    set<string> allConfigFiles = this->model->getAllConfigFiles();
    for (auto it = allConfigFiles.begin(); it != allConfigFiles.end(); it++) {
        QString configFileName = it->c_str();
        this->configFilesController->addFileName(configFileName, false);
    }
}

CModel* MainWindow::getModel() {
    return this->model;
}

QtFileController* MainWindow::getLogFilesController() {
    return this->logFilesController;
}

QtFileController* MainWindow::getConfigFilesController() {
    return this->configFilesController;
}

void MainWindow::updateWhichLogFilesAreChecked() {
    QList<QAction*> actionList = ui->menuInput_Log_Files->actions();
    for (int i = 3; i < actionList.size(); i++) {
        if (actionList.at(i)->isChecked()) {
            this->model->activateInputFile(actionList.at(i)->text().toUtf8().constData());
        } else {
            this->model->deactivateInputFile(actionList.at(i)->text().toUtf8().constData());
        }
    }
}

void MainWindow::updateWhichConfigFilesAreChecked() {
    QList<QAction*> actionList = ui->menuConfiguration_Files->actions();
    for (int i = 3; i < actionList.size(); i++) {
        if (actionList.at(i)->isChecked()) {
            this->model->activateConfigFile(actionList.at(i)->text().toUtf8().constData());
        } else {
            this->model->deactivateConfigFile(actionList.at(i)->text().toUtf8().constData());
        }
    }
}

void MainWindow::updateLogFilesParams() {
    ui->txt_input->setText(this->model->getInputParams(false).c_str());
}

void MainWindow::updateConfigFilesParams() {
    ui->txt_config->setText(this->model->getConfigParams(false).c_str());
}

void MainWindow::updateFixedAndTimeParams() {
    ui->txt_fixed->setText(this->model->getFixedAndTimeParams().c_str());
}

void MainWindow::on_btn_startAnalyzing_clicked() {
    ui->txt_sec->clear();

    QTime timer;
    timer.start();
    this->model->startSec(ui->txt_sec, ui->lnE_optional->text().toUtf8().constData());
    int elapsedTimeInMiliseconds = timer.elapsed();
    showElapsedTime(elapsedTimeInMiliseconds);
}

void MainWindow::showElapsedTime(int msecs) {
    int hours = msecs / (1000 * 60 * 60);
    msecs %= (1000 * 60 * 60);
    int minutes = msecs / (1000 * 60);
    msecs %= (1000 * 60);
    int seconds = msecs / 1000;
    msecs %= 1000;
    int miliseconds = msecs;

    ostringstream os;
    os << hours << " hours, " << minutes << " minutes, " << seconds << " seconds, " << miliseconds << " miliseconds.";
    QString message = "Analysis finished in ";
    message = message.append(os.str().c_str());

    QMessageBox* messageBox = new QMessageBox(QMessageBox::Information, "Finished", message, QMessageBox::Ok);
    messageBox->exec();
    delete messageBox;
}

void MainWindow::on_actionAdd_Log_File_triggered() {
    LogFileDialog logFileDialog(this, NULL); // NULL => Add new log file
    logFileDialog.exec();
    updateLogFilesParams();
}

void MainWindow::actionModify_Log_File_triggered(QString fileName) {
    LogFileDialog logFileDialog(this, fileName);
    logFileDialog.exec();
}

void MainWindow::actionDelete_Log_File_triggered(QString fileName) {
    this->model->deleteInputFile(fileName.toUtf8().constData(), CFsFunctions::generateFormatNameFromFileName(fileName.toUtf8().constData()));
    this->logFilesController->deleteFileName(fileName);
    updateLogFilesParams();
}

void MainWindow::actionMenu_Log_File_triggered() {
    updateWhichLogFilesAreChecked();
    updateLogFilesParams();
}

void MainWindow::on_actionAdd_Config_File_triggered() {
    ConfigFileDialog configFileDialog(this, NULL); // NULL => Add new log file
    configFileDialog.exec();
    updateConfigFilesParams();
}

void MainWindow::actionModify_Config_File_triggered(QString fileName) {
    ConfigFileDialog configFileDialog(this, fileName);
    configFileDialog.exec();
}

void MainWindow::actionDelete_Config_File_triggered(QString fileName) {
    this->model->deleteConfigFile(fileName.toUtf8().constData());
    this->configFilesController->deleteFileName(fileName);
    updateConfigFilesParams();
}

void MainWindow::actionMenu_Config_File_triggered() {
    updateWhichConfigFilesAreChecked();
    updateConfigFilesParams();
}

QString MainWindow::showSetDateAndTimeMessageBox() {
    QMessageBox* setDateAndTimeFilter = new QMessageBox(QMessageBox::Question, "Set Date and Time", "Set Date and Time:", QMessageBox::Ok | QMessageBox::Cancel);

    QDateTime now = QDateTime::currentDateTime();
    QDateTimeEdit* dateTimeEdit = new QDateTimeEdit(now.date(), setDateAndTimeFilter);
    dateTimeEdit->setFixedHeight(58);
    dateTimeEdit->setFixedWidth(208);
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("MMM/dd/yyyy:HH:mm:ss");

    QString result = "";

    if (setDateAndTimeFilter->exec() == QMessageBox::Ok) {
        result = dateTimeEdit->date().toString("M/d/yyyy").append(':').append(dateTimeEdit->time().toString("H:m:s"));
    }

    delete dateTimeEdit;
    delete setDateAndTimeFilter;

    return result;
}

void MainWindow::on_actionSet_Time_From_triggered() {
    QString dateAndTime = showSetDateAndTimeMessageBox();
    if (!dateAndTime.isEmpty()) {
        this->model->setTimeFilterFrom(dateAndTime.toUtf8().constData());
        updateFixedAndTimeParams();
    }
}

void MainWindow::on_actionSet_Time_To_triggered() {
    QString dateAndTime = showSetDateAndTimeMessageBox();
    if (!dateAndTime.isEmpty()) {
        this->model->setTimeFilterTo(dateAndTime.toUtf8().constData());
        updateFixedAndTimeParams();
    }
}

void MainWindow::on_action1_Critical_triggered() {
    this->model->setDebugLevel(1);
    updateFixedAndTimeParams();
}

void MainWindow::on_action2_Error_triggered() {
    this->model->setDebugLevel(2);
    updateFixedAndTimeParams();
}

void MainWindow::on_action3_Warning_triggered() {
    this->model->setDebugLevel(3);
    updateFixedAndTimeParams();
}

void MainWindow::on_action4_Notification_triggered() {
    this->model->setDebugLevel(4);
    updateFixedAndTimeParams();
}

void MainWindow::on_action5_Informative_triggered() {
    this->model->setDebugLevel(5);
    updateFixedAndTimeParams();
}

void MainWindow::on_action6_Debug_triggered() {
    this->model->setDebugLevel(6);
    updateFixedAndTimeParams();
}

void MainWindow::on_actionClear_All_Filters_triggered() {
    this->model->setTimeFilterFrom("");
    this->model->setTimeFilterTo("");
    updateFixedAndTimeParams();
}

void MainWindow::on_actionUsage_triggered() {
    ui->txt_sec->clear();

    this->model->startSec(ui->txt_sec, "--help");
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox* messageBox = new QMessageBox(QMessageBox::Information, "About Forensic Analyzer v1.0.0", "Forensic Analyzer is graphical interface of SEC+. \n"
            "It allows you to search for event correlations across various log files. \nSpecify suspicious activities by adding rules into configuration files and start the offline forensic analysis. \n\n Copyright (C) 2017 Jan Lejnar", QMessageBox::Ok);
    messageBox->exec();
    delete messageBox;
}
