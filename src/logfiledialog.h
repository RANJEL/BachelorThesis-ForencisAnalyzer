/// Header for class LogFileDialog
/** \file logfiledialog.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef ADDLOGFILEDIALOG_H
#define ADDLOGFILEDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "mainwindow.h"

class MainWindow;

namespace Ui {
    class LogFileDialog;
}

/// Class representing the dialog that handles Add or Modify input log file
class LogFileDialog : public QDialog {
    Q_OBJECT

public:
    /** \param parent parent that created this QDialog
      * \param fileName name of input log file that should be opened to modify, or "" if you want to add new */
    explicit LogFileDialog(QWidget* parent = 0, QString fileName = QString::fromStdString("")); ///< Constructor
    ~LogFileDialog(); ///< Destructor

private slots:
    void on_btn_saveAndExit_clicked(); ///< Only way to save changes that were made in the dialog

private:
    QString fileName;

    Ui::LogFileDialog* ui;
    MainWindow* mainWindow;
    void setInvalidFields(); ///< If rejecting QDialog won't close the dialog, set INVALID fields at least
    bool isValidFile(); ///< Checks if the file has valid name
    void addLogFile(); ///< Method called if the dialog was created in order to add new input log file
    void fillFields(); ///< Fill known dialog field
};

#endif // ADDLOGFILEDIALOG_H
