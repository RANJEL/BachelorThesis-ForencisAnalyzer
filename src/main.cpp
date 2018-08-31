/// Entry point of Forensic Analyzer
/** \file main.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
