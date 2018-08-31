/// Header for class QtFileController
/** \file qtfilecontroller.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef QTFILECONTROLLER_H
#define QTFILECONTROLLER_H

#include <map>
#include <QMainWindow>
#include <QSignalMapper>
#include <QAction>
#include <QMenu>
#include <QString>

using namespace std;

/// Class that has primary task to create & delete dynamically QActions representing CRUID operations with files. It contains QSignalMapper in order to redirect signals to proper slots
class QtFileController {
public:
    /** \param mainWindow pointer to the parent mainWindow where are QMenus 
     *  \param listMenu menu list of checkable files 
     *  \param modifyMenu modify menu where new QAction will be added 
     *  \param deleteMenu delete menu where new QAction will be added 
     *  \param isLogFileController true if QtFileController handles input log files, false if it handles configuration files
     */
    QtFileController(QMainWindow* mainWindow, QMenu* listMenu, QMenu* modifyMenu, QMenu* deleteMenu, bool isLogFileController); ///< Constructor
    ~QtFileController(); ///< Destructor

    /** \param fileName name of the file you want to add 
        \param setChecked true if this file should be checked in checkable menu
     */
    void addFileName(const QString& fileName, bool setChecked); ///< Add file name to checkable menu, modifyMenu and deleteMenu
    bool deleteFileName(const QString& fileName);

private:
    QMainWindow* mainWindow;
    QMenu* listMenu;
    QMenu* modifyMenu;
    QMenu* deleteMenu;

    bool isLogFileController;
    QSignalMapper* signalMapperModify;
    QSignalMapper* signalMapperDelete;
    map<QString, QAction*[3] > file2ActionsMap; ///< For each file there are 3 different QActions defined (1. in checkable menu, 2. in modify menu, 3. in delete menu)

    void addToActionMenu(const QString& fileName, bool setChecked);
    void addToModifyMenu(const QString& fileName);
    void addToDeleteMenu(const QString& fileName);

    void disconnectListeners(QAction* action);
    void deleteFromActionMenu(QAction* action);
    void deleteFromModifyMenu(QAction* action);
    void deleteFromDeleteMenu(QAction* action);

};

#endif /* QTFILECONTROLLER_H */
