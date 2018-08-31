/// Header for class MainWindow
/** \file mainwindow.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QTime>

#include "cmodel.h"
#include "qtfilecontroller.h"
#include "logfiledialog.h"
#include "configfiledialog.h"

namespace Ui {
    class MainWindow;
}

/// The main Qt window of Forensic Analyzer
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /** Constructor creates QtFileControllers for input log files and configuration files. It also creates the model of the application.*/
    explicit MainWindow(QWidget* parent = 0); ///< Constructor
    ~MainWindow(); ///< Destructor

    CModel* getModel(); ///< Getter to the model of Forensic Analyzer
    QtFileController* getLogFilesController(); ///< Getter to QtFileController that handles input log files
    QtFileController* getConfigFilesController(); ///< Getter to QtFileController that handles configuration files

private slots:
    void on_btn_startAnalyzing_clicked();

    void on_actionAdd_Log_File_triggered();

    void on_actionAdd_Config_File_triggered();

    void on_actionSet_Time_From_triggered();

    void on_actionSet_Time_To_triggered();
    
    void on_action1_Critical_triggered();

    void on_action2_Error_triggered();

    void on_action3_Warning_triggered();

    void on_action4_Notification_triggered();

    void on_action5_Informative_triggered();

    void on_action6_Debug_triggered();

    void on_actionClear_All_Filters_triggered();

    void on_actionUsage_triggered();

    void on_actionAbout_triggered();

    /** custom slots - they are not fixed, QtFileController redirect various signals to these slots */
    void actionModify_Log_File_triggered(QString fileName);

    void actionDelete_Log_File_triggered(QString fileName);

    void actionMenu_Log_File_triggered();

    void actionModify_Config_File_triggered(QString fileName);

    void actionDelete_Config_File_triggered(QString fileName);

    void actionMenu_Config_File_triggered();

private:
    Ui::MainWindow* ui;
    CModel* model;
    QtFileController* logFilesController;
    QtFileController* configFilesController;

    void loadBackup(); ///< This method fills QtFileControllers according to the model

    void updateWhichLogFilesAreChecked(); ///< Go through Qt QMenu and update the model
    void updateWhichConfigFilesAreChecked(); ///< Go through Qt QMenu and update the model
    void updateLogFilesParams(); ///< Fills the Qt line edit according to the model
    void updateConfigFilesParams(); ///< Fills the Qt line edit according to the model 
    void updateFixedAndTimeParams(); ///< Fills the Qt line edit according to the model 

    QString showSetDateAndTimeMessageBox(); ///< Method that shows MessageBox triggered by setting time filter restriction
    void showElapsedTime(int msec); ///< Method that shows MessageBox informing a user about the time that SEC+ needed to go through all input log files
};

#endif // MAINWINDOW_H
