/// Implementation of caliasesmanager.h
/** \file caliasesmanager.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "caliasesmanager.h"

CAliasesManager::CAliasesManager(const string& backupFileName) {
    m_backupFolderName = "backup";
    m_backupFileName = backupFileName;
}

bool CAliasesManager::addAlias(const string& assignment) {
    size_t aliasStart, aliasEnd;
    if (!constainsAlias(assignment, aliasStart, aliasEnd)) {
        return false;
    } else {
        string aliasName, aliasValue;
        if (!parseAliasNameAndValue(assignment, aliasStart, aliasEnd, aliasName, aliasValue)) {
            return false; // empty name or value
        }

        auto it = m_aliasesMap.find(aliasName);

        if (it != m_aliasesMap.end()) {
            m_aliasesMap[aliasName] = aliasValue;
            return false; // was modified
        } else {
            m_aliasesMap[aliasName] = aliasValue;
            return true;
        }
    }
}

void CAliasesManager::clearAllAliases() {
    m_aliasesMap.clear();
}

bool CAliasesManager::constainsAlias(const string& input, size_t& aliasStart, size_t& aliasEnd) const {
    aliasStart = input.find("${");
    if (aliasStart == string::npos) {
        return false;
    }

    aliasEnd = input.find('}', aliasStart + 2);
    if (aliasEnd == string::npos) {
        return false;
    }

    return true;
}

bool CAliasesManager::parseAliasName(const string& input, const size_t& aliasStart, const size_t& aliasEnd, string& aliasName) const {
    aliasName = input.substr(aliasStart + 2, aliasEnd - (aliasStart + 2));

    string trimmedAliasName = aliasName;
    CFsFunctions::trimString(trimmedAliasName);

    if (trimmedAliasName == "") {
        return false;
    }

    return true;
}

bool CAliasesManager::parseAliasNameAndValue(const string& input, const size_t& aliasStart, const size_t& aliasEnd, string& aliasName, string& aliasValue) const {
    aliasValue = input.substr(aliasEnd + 2); // skip =

    string trimmedAliasValue = aliasValue;
    CFsFunctions::trimString(trimmedAliasValue);

    if (!parseAliasName(input, aliasStart, aliasEnd, aliasName) || trimmedAliasValue == "") {
        return false;
    }

    return true;
}

map<string, string> CAliasesManager::getAllAliases() const {
    return m_aliasesMap;
}

bool CAliasesManager::isSelfRecursive(const string& currentAliasName, string aliasRightSide) const {
    size_t aliasStart, aliasEnd;
    string aliasName;

    while (constainsAlias(aliasRightSide, aliasStart, aliasEnd)) {
        parseAliasName(aliasRightSide, aliasStart, aliasEnd, aliasName);
        aliasRightSide = aliasRightSide.substr(aliasEnd + 1);

        if (aliasName == currentAliasName) {
            return true;
        }
    }

    return false;
}

string CAliasesManager::evaluateInput(const string& input) const {
    string evaluatedInput = input;
    size_t aliasStart, aliasEnd;
    string aliasName;

    while (constainsAlias(evaluatedInput, aliasStart, aliasEnd)) {
        string inputBeforeAlias = evaluatedInput.substr(0, aliasStart);
        string inputAfterAlias = evaluatedInput.substr(aliasEnd + 1);

        if (!parseAliasName(evaluatedInput, aliasStart, aliasEnd, aliasName)) {
            evaluatedInput = inputBeforeAlias.append(inputAfterAlias);
        } else {
            auto it = m_aliasesMap.find(aliasName);
            if (it == m_aliasesMap.end() || isSelfRecursive(aliasName, it->second)) { /* not found or defence agains ${A}=${A} */
                evaluatedInput = inputBeforeAlias.append(inputAfterAlias); // ignore not defined aliases
            } else {
                evaluatedInput = inputBeforeAlias.append(it->second).append(inputAfterAlias);
            }
        }
    }

    return evaluatedInput;
}

bool CAliasesManager::loadFromBackup() {
    CFsFunctions::createPathIfDoesntExist(m_backupFolderName, m_backupFileName);

    string line;
    ifstream is(CFsFunctions::getRelativePath(m_backupFolderName, m_backupFileName));
    if (!is.good())
        return false;

    while (is.good()) {
        getline(is, line);
        CFsFunctions::trimString(line);
        if (line == "")
            break;
        this->addAlias(line);
    }

    is.close();
    return true;
}

void CAliasesManager::backup() const {
    ofstream os(CFsFunctions::getRelativePath(m_backupFolderName, m_backupFileName));
    for (auto it = m_aliasesMap.begin(); it != m_aliasesMap.end(); it++) {
        os << "${" << it->first << "}=" << it->second << endl;
    }
    os.close();
}
