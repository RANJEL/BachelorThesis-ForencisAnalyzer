/// Header for class ConfigFileDialog
/** \file configfiledialog.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef CONFIGFILEDIALOG_H
#define CONFIGFILEDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "crulegenerator.h"

class MainWindow;

namespace Ui {
    class ConfigFileDialog;
}

/// Class representing the dialog that handles Add or Modify configuration file
class ConfigFileDialog : public QDialog {
    Q_OBJECT

public:
    /** \param parent parent that created this QDialog
     * \param fileName name of configuration file that should be opened to modify, or "" if you want to add new */
    explicit ConfigFileDialog(QWidget* parent = 0, QString fileName = QString::fromStdString("")); ///< Constructor
    ~ConfigFileDialog(); ///< Destructor

private slots:
    void on_btn_saveAndExit_clicked(); ///< Only way to save changes that were made in the dialog

    void on_btn_generate_clicked(); ///< Button that provides user ability to generate mandatory fields of the SEC+ rule

private:
    Ui::ConfigFileDialog* ui;

    QString fileName;
    CRuleGenerator* m_ruleGenerator;

    MainWindow* mainWindow;
    void setInvalidFields(); ///< If rejecting QDialog won't close the dialog, set INVALID fields at least
    bool isValidFile(); ///< Checks if the file has valid name
    void addConfigFile(); ///< Method called if the dialog was created in order to add new input log file
    void fillFields(); ///< Fill known dialog field
    bool readFromFile(QString fileName); ///< Read configuration file and display it in the dialog
    bool writeToFile(QString fileName); ///< Write / Update displayed configuration file
};

#endif // CONFIGFILEDIALOG_H
