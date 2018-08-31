/// Implementation of cmodel.h
/** \file cmodel.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "cmodel.h"

CModel::TInputFileState::TInputFileState(bool isActive, const string& formatName) {
    m_isActive = isActive;
    m_formatName = formatName;
}

CModel::TFormatNameState::TFormatNameState(const string& dateRegex, const string& inputFileName) {
    m_dateRegex = dateRegex;
    m_inputFileName = inputFileName;
}

CModel::CModel() {
    m_backupFolderName = "backup";
    m_logFormatsResourcesFolderName = "res";

    m_inputBackupFileName = "inputFilesBackup.txt";
    m_configBackupFileName = "configFilesBackup.txt";
    m_logFormatsFileName = "logFormats.conf";

    m_logDialogAliasManager = new CAliasesManager("dateAliasesBackup.txt");
    m_configDialogAliasManager = new CAliasesManager("formatAliasesBackup.txt");

    m_fixedAndTimeParams[0] = "notail";
    m_fixedAndTimeParams[1] = "parseEventTime";
    m_fixedAndTimeParams[2] = "parsedTimeOrder";
    m_fixedAndTimeParams[3] = "debug=3";
    m_fixedAndTimeParams[4] = "";
    m_fixedAndTimeParams[5] = "";

    loadState();
}

CModel::~CModel() {
    backupState();

    for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); it++) {
        delete it->second;
    }

    for (auto it = m_format2RegexMap.begin(); it != m_format2RegexMap.end(); it++) {
        delete it->second;
    }

    delete m_logDialogAliasManager;
    delete m_configDialogAliasManager;
}

bool CModel::loadInputFilesFromBackup(const string& folder, const string& fromFile) {
    CFsFunctions::createPathIfDoesntExist(folder, fromFile);

    string line;
    ifstream is(CFsFunctions::getRelativePath(folder, fromFile));
    if (!is.good())
        return false;

    while (is.good()) {
        getline(is, line);
        CFsFunctions::trimString(line);
        if (line == "")
            break;
        string formatName = CFsFunctions::generateFormatNameFromFileName(line);
        m_inputFiles[line] = new TInputFileState(false, formatName);
        m_format2RegexMap[formatName] = new TFormatNameState("", line);
    }

    is.close();
    return true;
}

bool CModel::loadConfigFilesFromBackup(const string& folder, const string& fromFile) {
    CFsFunctions::createPathIfDoesntExist(folder, fromFile);

    string line;
    ifstream is(CFsFunctions::getRelativePath(folder, fromFile));
    if (!is.good())
        return false;

    while (is.good()) {
        getline(is, line);
        CFsFunctions::trimString(line);
        if (line == "")
            break;
        m_configFiles[line] = false;
    }

    is.close();
    return true;
}

bool CModel::loadLogFormatsFromBackup(const string& folder, const string& fromFile) {
    string relativeFilePath = CFsFunctions::getRelativePath(folder, fromFile);
    string createBackupFileCommand = "touch " + relativeFilePath + " 2>/dev/null";
    system(createBackupFileCommand.c_str());

    string line;
    ifstream is(relativeFilePath);
    if (!is.good())
        return false;

    size_t delimIndex;
    string formatName, regex;
    while (is.good()) {
        getline(is, line);
        CFsFunctions::trimString(line);
        if (line == "")
            break;

        delimIndex = line.find_first_of('=');
        if (delimIndex == string::npos) {
            is.close();
            return false;
        }
        formatName = line.substr(0, delimIndex);
        regex = line.substr(delimIndex + 1);
        auto it = m_format2RegexMap.find(formatName);
        if (it != m_format2RegexMap.end()) {
            m_format2RegexMap[formatName]->m_dateRegex = regex;
        } else {
            m_format2RegexMap[formatName] = new TFormatNameState("", line);
        }
    }

    is.close();
    return true;
}

bool CModel::loadState() {
    loadInputFilesFromBackup(m_backupFolderName, m_inputBackupFileName);
    loadConfigFilesFromBackup(m_backupFolderName, m_configBackupFileName);

    m_logDialogAliasManager->loadFromBackup();
    m_configDialogAliasManager->loadFromBackup();
    loadLogFormatsFromBackup(m_backupFolderName, m_logFormatsFileName);
    return true;
}

void CModel::backupInputFiles(const string& folder, const string& toFile) const {
    ofstream os(CFsFunctions::getRelativePath(folder, toFile));
    for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); it++) {
        os << it->first << endl;
    }
    os.close();
}

void CModel::backupConfigFiles(const string& folder, const string& toFile) const {
    ofstream os(CFsFunctions::getRelativePath(folder, toFile));
    for (auto it = m_configFiles.begin(); it != m_configFiles.end(); it++) {
        os << it->first << endl;
    }
    os.close();
}

void CModel::backupLogFormats(const string& folder, const string& toFile) const {
    ofstream os(CFsFunctions::getRelativePath(folder, toFile));
    for (auto it = m_format2RegexMap.begin(); it != m_format2RegexMap.end(); it++) {
        os << it->first << '=' << it->second->m_dateRegex << endl;
    }
    os.close();
}

void CModel::backupState() const {
    string createBackupFolderCommand = "mkdir " + m_backupFolderName + " 2>/dev/null";
    system(createBackupFolderCommand.c_str());

    backupInputFiles(m_backupFolderName, m_inputBackupFileName);
    backupConfigFiles(m_backupFolderName, m_configBackupFileName);

    m_logDialogAliasManager->backup();
    m_configDialogAliasManager->backup();
    backupLogFormats(m_backupFolderName, m_logFormatsFileName);
}

bool CModel::containsInputFile(const string& inputFileName) const {
    auto it = m_inputFiles.find(inputFileName);
    if (it == m_inputFiles.end())
        return false;

    return true;
}

bool CModel::containsInputFile(const string& inputFileName, const string& formatName) const {
    if (containsInputFile(inputFileName) == false)
        return false;

    auto it2 = m_format2RegexMap.find(formatName);
    if (it2 == m_format2RegexMap.end())
        return false;

    return true;
}

bool CModel::containsConfigFile(const string& configFileName) const {
    auto it = m_configFiles.find(configFileName);
    if (it == m_configFiles.end())
        return false;

    return true;
}

bool CModel::addInputFile(const string& inputFileName, const string& formatName, const string& dateRegex) {
    if (containsInputFile(inputFileName, formatName))
        return false;

    m_inputFiles[inputFileName] = new TInputFileState(true, formatName);
    m_format2RegexMap[formatName] = new TFormatNameState(dateRegex, inputFileName);

    return true;
}

bool CModel::addConfigFile(const string& configFileName) {
    if (containsConfigFile(configFileName))
        return false;

    m_configFiles[configFileName] = true;

    return true;
}

bool CModel::deleteInputFile(const string& inputFileName, const string& formatName) {
    if (!containsInputFile(inputFileName, formatName))
        return false;

    auto it = m_inputFiles.find(inputFileName);
    auto it2 = m_format2RegexMap.find(formatName);
    delete it->second;
    m_inputFiles.erase(it);
    delete it2->second;
    m_format2RegexMap.erase(it2);

    return true;
}

bool CModel::deleteConfigFile(const string& configFileName) {
    if (!containsConfigFile(configFileName))
        return false;

    auto it = m_configFiles.find(configFileName);
    m_configFiles.erase(it);

    return true;
}

bool CModel::modifyInputFile(const string& inputFileName, const string& formatName, const string& dateRegex) {
    if (!containsInputFile(inputFileName, formatName))
        return false;

    m_inputFiles[inputFileName]->m_formatName = formatName;
    m_format2RegexMap[formatName]->m_inputFileName = inputFileName;
    m_format2RegexMap[formatName]->m_dateRegex = dateRegex;
    return true;
}

bool CModel::activateInputFile(const string& inputFileName) {
    if (!containsInputFile(inputFileName))
        return false;

    m_inputFiles[inputFileName]->m_isActive = true;

    return true;
}

bool CModel::activateConfigFile(const string& configFileName) {
    if (!containsConfigFile(configFileName))
        return false;

    m_configFiles[configFileName] = true;

    return true;
}

bool CModel::deactivateInputFile(const string& inputFileName) {
    if (!containsInputFile(inputFileName))
        return false;

    m_inputFiles[inputFileName]->m_isActive = false;

    return true;
}

bool CModel::deactivateConfigFile(const string& configFileName) {
    if (!containsConfigFile(configFileName))
        return false;

    m_configFiles[configFileName] = false;

    return true;
}

void CModel::setDebugLevel(int debugLevel) {
    m_fixedAndTimeParams[3] = "debug=" + to_string(debugLevel);
}

void CModel::setTimeFilterFrom(const string& dateAndTime) {
    if (dateAndTime == "") {
        m_fixedAndTimeParams[4] = "";
    } else {
        m_fixedAndTimeParams[4] = "timeFilterFrom=" + dateAndTime;
    }
}

void CModel::setTimeFilterTo(const string& dateAndTime) {
    if (dateAndTime == "") {
        m_fixedAndTimeParams[5] = "";
    } else {
        m_fixedAndTimeParams[5] = "timeFilterTo=" + dateAndTime;
    }
}

set<string> CModel::getAllInputFiles() const {
    set<string> res;

    for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); it++) {
        res.insert(it->first);
    }

    return res;
}

set<string> CModel::getAllConfigFiles() const {
    set<string> res;

    for (auto it = m_configFiles.begin(); it != m_configFiles.end(); it++) {
        res.insert(it->first);
    }

    return res;
}

string CModel::getFormatRegex(const string& formatName) const {
    auto it = m_format2RegexMap.find(formatName);
    if (it == m_format2RegexMap.end())
        return "";

    return it->second->m_dateRegex;
}

string CModel::getFormatName(const string& fileName) const {
    auto it = m_inputFiles.find(fileName);
    if (it == m_inputFiles.end())
        return CFsFunctions::generateFormatNameFromFileName(fileName);

    return it->second->m_formatName;
}

string CModel::getInputParams(bool fullPath) const {
    ostringstream os;
    for (auto it = m_inputFiles.begin(); it != m_inputFiles.end(); it++) {
        if (it->second->m_isActive) {
            os << "--input=";
            if (fullPath == true) {
                os << it->first;
            } else {
                os << CFsFunctions::getFileNameFromAbsolutePath(it->first);
            }
            os << "=" << it->second->m_formatName << " ";
        }
    }

    return os.str();
}

string CModel::getConfigParams(bool fullPath) const {
    ostringstream os;
    for (auto it = m_configFiles.begin(); it != m_configFiles.end(); it++) {
        if (it->second == true) { // is active
            os << "--conf=";
            if (fullPath == true) {
                os << it->first;
            } else {
                os << CFsFunctions::getFileNameFromAbsolutePath(it->first);
            }
            os << " ";
        }
    }

    return os.str();
}

string CModel::getFixedAndTimeParams() const {
    ostringstream os;
    os << "--" << m_fixedAndTimeParams[0] << " " << "--" << m_fixedAndTimeParams[1] << " "
            << "--" << m_fixedAndTimeParams[2] << " " << "--" << m_fixedAndTimeParams[3] << " ";
    if (m_fixedAndTimeParams[4] != "")
        os << "--" << m_fixedAndTimeParams[4] << " ";

    if (m_fixedAndTimeParams[5] != "")
        os << "--" << m_fixedAndTimeParams[5] << " ";

    return os.str();
}

string CModel::buildSecCommand(string inputParams, string configParams, string fixedAndTimeParams, string optionalParams) const {
    ostringstream os;
    os << "./res/sec+ " << inputParams << configParams << fixedAndTimeParams << optionalParams << " 2>&1";
    return os.str();
}

bool CModel::startSec(QPlainTextEdit* output, string optionalParams) const {
    createEvaluatedLogFormatsFile(m_logFormatsResourcesFolderName, m_logFormatsFileName);

    string secCommand = buildSecCommand(getInputParams(true), getConfigParams(true), getFixedAndTimeParams(), optionalParams);
    cout << "--------------------------------------------------------------------------------" << endl << secCommand << endl; // CLI debug

    FILE* pipe;
    char buffer[1024];

    if (!(pipe = popen(secCommand.c_str(), "r"))) {
        return false;
    }

    while (fgets(buffer, sizeof (buffer), pipe) != NULL) {
        output->insertPlainText(buffer); // This line is very time-consuming compared to manual startup of SEC+!
    }

    pclose(pipe);

    cout << "--------------------------------------------------------------------------------" << endl; // CLI debug
    return true;
}

string CModel::getLogDialogAliases() const {
    ostringstream os;
    map<string, string> allAliases = m_logDialogAliasManager->getAllAliases();
    for (auto it = allAliases.begin(); it != allAliases.end(); it++) {
        os << "${" << it->first << "}=" << it->second << endl;
    }

    return os.str();
}

string CModel::getConfigDialogAliases() const {
    ostringstream os;
    map<string, string> allAliases = m_configDialogAliasManager->getAllAliases();
    for (auto it = allAliases.begin(); it != allAliases.end(); it++) {
        os << "${" << it->first << "}=" << it->second << endl;
    }

    return os.str();
}

void CModel::addLogDialogAlias(const string& assignment) {
    m_logDialogAliasManager->addAlias(assignment);
}

void CModel::updateLogDialogAliases(const string& allAliases) {
    m_logDialogAliasManager->clearAllAliases();

    string line;
    istringstream is(allAliases);

    while (is.good()) {
        getline(is, line);
        m_logDialogAliasManager->addAlias(line);
    }
}

void CModel::updateConfigDialogAlises(const string& allAliases) {
    m_configDialogAliasManager->clearAllAliases();

    string line;
    istringstream is(allAliases);

    while (is.good()) {
        getline(is, line);
        m_configDialogAliasManager->addAlias(line);
    }
}

bool CModel::validateDateParsingRegex(const string& regex, string& missingElement) const {
    string evaluatedRegex = m_logDialogAliasManager->evaluateInput(regex);

    missingElement = "?<month>";
    if (evaluatedRegex.find(missingElement) == string::npos)
        return false;

    missingElement = "?<day>";
    if (evaluatedRegex.find(missingElement) == string::npos)
        return false;

    missingElement = "?<hour>";
    if (evaluatedRegex.find(missingElement) == string::npos)
        return false;

    missingElement = "?<minute>";
    if (evaluatedRegex.find(missingElement) == string::npos)
        return false;

    missingElement = "?<second>";
    if (evaluatedRegex.find(missingElement) == string::npos)
        return false;

    missingElement = "";
    return true;
}

string CModel::evaluateInputAccordingToConfigAliases(const string& input) const {
    return m_configDialogAliasManager->evaluateInput(input);
}

void CModel::createEvaluatedLogFormatsFile(const string& folder, const string& toFile) const {
    ofstream os(CFsFunctions::getRelativePath(folder, toFile));
    for (auto it = m_format2RegexMap.begin(); it != m_format2RegexMap.end(); it++) {
        os << it->first << '=' << m_logDialogAliasManager->evaluateInput(it->second->m_dateRegex) << endl;
    }
    os.close();
}
