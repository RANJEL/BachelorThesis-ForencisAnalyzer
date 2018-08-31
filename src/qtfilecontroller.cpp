/// Implementation of qtfilecontroller.h
/** \file qtfilecontroller.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "qtfilecontroller.h"

QtFileController::QtFileController(QMainWindow* mainWindow, QMenu* listMenu, QMenu* modifyMenu, QMenu* deleteMenu, bool isLogFileController) {
    this->mainWindow = mainWindow;
    this->listMenu = listMenu;
    this->modifyMenu = modifyMenu;
    this->deleteMenu = deleteMenu;
    this->isLogFileController = isLogFileController;
    signalMapperModify = new QSignalMapper(this->mainWindow);
    signalMapperDelete = new QSignalMapper(this->mainWindow);

    if (isLogFileController) {
        QObject::connect(signalMapperModify, SIGNAL(mapped(QString)), this->mainWindow, SLOT(actionModify_Log_File_triggered(QString)));
        QObject::connect(signalMapperDelete, SIGNAL(mapped(QString)), this->mainWindow, SLOT(actionDelete_Log_File_triggered(QString)));
    } else {
        QObject::connect(signalMapperModify, SIGNAL(mapped(QString)), this->mainWindow, SLOT(actionModify_Config_File_triggered(QString)));
        QObject::connect(signalMapperDelete, SIGNAL(mapped(QString)), this->mainWindow, SLOT(actionDelete_Config_File_triggered(QString)));
    }

}

QtFileController::~QtFileController() {
    for (auto it = file2ActionsMap.begin(); it != file2ActionsMap.end(); it++) { // requires -std=c++11
        delete it->second[0];
        delete it->second[1];
        delete it->second[2];
    }

    delete this->signalMapperModify;
    delete this->signalMapperDelete;
}

void QtFileController::addToActionMenu(const QString& fileName, bool setChecked) {
    QAction* actionMenuInput = this->listMenu->addAction(fileName);
    actionMenuInput->setCheckable(true);
    actionMenuInput->setChecked(setChecked);

    this->file2ActionsMap[fileName][0] = actionMenuInput;

    if (isLogFileController) {
        QObject::connect(actionMenuInput, SIGNAL(triggered()), this->mainWindow, SLOT(actionMenu_Log_File_triggered()));
    } else {
        QObject::connect(actionMenuInput, SIGNAL(triggered()), this->mainWindow, SLOT(actionMenu_Config_File_triggered()));
    }
}

void QtFileController::addToModifyMenu(const QString& fileName) {
    QAction* actionModify = this->modifyMenu->addAction(fileName);

    this->file2ActionsMap[fileName][1] = actionModify;

    QObject::connect(actionModify, SIGNAL(triggered()), signalMapperModify, SLOT(map()));
    signalMapperModify->setMapping(actionModify, actionModify->text());
}

void QtFileController::addToDeleteMenu(const QString& fileName) {
    QAction* actionDelete = this->deleteMenu->addAction(fileName);

    this->file2ActionsMap[fileName][2] = actionDelete;

    QObject::connect(actionDelete, SIGNAL(triggered()), signalMapperDelete, SLOT(map()));
    signalMapperDelete->setMapping(actionDelete, actionDelete->text());
}

void QtFileController::addFileName(const QString& fileName, bool setChecked) {
    addToActionMenu(fileName, setChecked);
    addToModifyMenu(fileName);
    addToDeleteMenu(fileName);
}

void QtFileController::disconnectListeners(QAction* action) {
    QObject::disconnect(action, SIGNAL(triggered()), signalMapperModify, SLOT(map()));
}

void QtFileController::deleteFromActionMenu(QAction* action) {
    if (isLogFileController) {
        QObject::disconnect(action, SIGNAL(triggered()), this->mainWindow, SLOT(actionMenu_Log_File_triggered()));
    } else {
        QObject::disconnect(action, SIGNAL(triggered()), this->mainWindow, SLOT(actionMenu_Config_File_triggered()));
    }
    this->listMenu->removeAction(action);
}

void QtFileController::deleteFromModifyMenu(QAction* action) {
    this->disconnectListeners(action);
    this->modifyMenu->removeAction(action);
}

void QtFileController::deleteFromDeleteMenu(QAction* action) {
    this->disconnectListeners(action);
    this->deleteMenu->removeAction(action);
}

bool QtFileController::deleteFileName(const QString& fileName) {
    auto it = file2ActionsMap.find(fileName);
    if (it != file2ActionsMap.end()) {
        deleteFromActionMenu(it->second[0]);
        deleteFromModifyMenu(it->second[1]);
        deleteFromDeleteMenu(it->second[2]);

        return true;
    }
    return false;
}
