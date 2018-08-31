/// Implementation of cfsfunctions.h
/** \file cfsfunctions.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "cfsfunctions.h"

string CFsFunctions::getFileNameFromAbsolutePath(const string& fileName) {
    char delim = '/';
    size_t lastSlashIndex = fileName.rfind(delim);
    string relativeName = fileName.substr(lastSlashIndex + 1);

    return relativeName;
}

string CFsFunctions::getRelativePath(const string& folder, const string& file) {
    return "./" + folder + "/" + file;
}

void CFsFunctions::createPathIfDoesntExist(const string& folder, const string& file) {
    string relativeFilePath = CFsFunctions::getRelativePath(folder, file);
    string createBackupFolderCommand = "mkdir " + folder + " 2>/dev/null";
    system(createBackupFolderCommand.c_str());
    string createBackupFileCommand = "touch " + relativeFilePath + " 2>/dev/null";
    system(createBackupFileCommand.c_str());
}

string CFsFunctions::generateFormatNameFromFileName(const string& fileName) {
    string s = getFileNameFromAbsolutePath(fileName);
    char dot = '.';
    size_t firstDotIndex = s.find(dot);
    if (firstDotIndex != string::npos) {
        s = s.substr(0, firstDotIndex);
    }

    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

void CFsFunctions::trimString(string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}
