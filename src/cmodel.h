/// Header for class CModel
/** \file cmodel.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef CMODEL_H
#define CMODEL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>

#include "cfsfunctions.h"
#include "caliasesmanager.h"
#include <QtGui/QPlainTextEdit>

using namespace std;

///  The model of Forensic Analyzer, that contains the main program logic and hold program state.
class CModel {
public:
    /** Contains definitions of backup file names, and starts to load state from backup files. */
    CModel(); ///< Constructor
    /** Destructor backups current model state */
    ~CModel(); ///< Destructor

    //-----------------------------------------------------------------------------------------------

    /** \param inputFileName absolute file path 
     *  \param formatName name of the format that will be later searched in logFormats.conf for date parsing regular expression 
     *  \param dateRegex date parsing regular expressing that contains named capture groups 
     *  \return false if model already contains this input file, true otherwise */
    bool addInputFile(const string& inputFileName, const string& formatName, const string& dateRegex); ///< Add input log file.
    /** Simpler version of the method addInputFile.*/
    bool addConfigFile(const string& configFileName); ///< Add configuration file. 
    /** Needs moreover formatName parameter, because I have 2 maps because of searching efficiency
     *  \return false if model doesn't contain this input file, true otherwise */
    bool deleteInputFile(const string& inputFileName, const string& formatName); ///< Delete input log file from model.
    bool deleteConfigFile(const string& configFileName); ///< Delete configuration file from model.
    /** \return false if model doesn't contain given input log file name and input log file format name */
    bool modifyInputFile(const string& inputFileName, const string& formatName, const string& dateRegex); ///< Mofidy input log file in sense of etc. set different dateRegex
    /** \param inputFileName input log file to activate \return false if model doesn't contain given input log file name*/
    bool activateInputFile(const string& inputFileName); ///< Activate input log file.
    bool activateConfigFile(const string& configFileName); ///< Activate configuration file.
    bool deactivateInputFile(const string& inputFileName); ///< Deactivate input log file.
    bool deactivateConfigFile(const string& configFileName); ///< Deactivate configuration file.

    set<string> getAllInputFiles() const; ///< Get all input log files
    set<string> getAllConfigFiles() const; ///< Get all configuration files
    string getLogDialogAliases() const; ///< Get all log dialog (date parsing) aliases
    string getConfigDialogAliases() const; ///< Get all configuration aliases

    string getFormatName(const string& fileName) const; ///< Get format name of given input log file
    string getFormatRegex(const string& formatName) const; ///< Get date parsing regex of given format name

    //----------------------------------------------------------------------------------------------

    /** This method generates sequences of --input=<input log file name> 
     *  \param fullPath if is called with false, only file name with extension will be added
     *  \return input parameters
     */
    string getInputParams(bool fullPath) const; ///< Get input parameters for SEC+
    string getConfigParams(bool fullPath) const; ///< Get configuration parameters for SEC+
    string getFixedAndTimeParams() const; ///< Get fixed parameters and time filter parameters for SEC+
    /** \param output Qt-field where SEC+ output will be stored
     *  \param optionalParams optional parametrs from GUI that are not stored in the model
     *  \return false if popen failed
     */
    bool startSec(QPlainTextEdit* output, string optionalParams) const; ///< Method that starts SEC+ as another process and writes into Forensic Analyzer using a pipe.

    /** \param regex regular expression
     *  \param missingElement output parameter that will be filled with first matched missing named capture group
     *  \return true if regex is valid (=contains all mandatory named capture groups)
     */
    bool validateDateParsingRegex(const string& regex, string& missingElement) const; ///< Method that validates if regex contains required named capture groups
    /** Note that evaluating of some input is redirected to CAliasManager */
    string evaluateInputAccordingToConfigAliases(const string& input) const; ///< Method that removes all ${} variables from input

    void setDebugLevel(int debugLevel); ///< Set "--debug" level of SEC+, default is 3
    /** \param dateAndTime string in expected format: <month>/<day>[/<year>]:<hour>:<minute>:<second>*/
    void setTimeFilterFrom(const string& dateAndTime); ///< Set "--timeFilterFrom" for SEC+
    void setTimeFilterTo(const string& dateAndTime); ///< Set "--timeFilterTo" for SEC+

    /** \param assignment assignment of ${<VARIABLE>}=<VALUE>
     */
    void addLogDialogAlias(const string& assignment); ///< Add new log dialog (date parsing) alias
    void updateLogDialogAliases(const string& allAliases); ///< Clear all input log dialog aliases and update by given parameter
    void updateConfigDialogAlises(const string& allAliases); ///< Clear all configuration dialog aliases and update by given parameter

private:
    /// Auxiliary structure to track input log file state
    struct TInputFileState {
        TInputFileState(bool isActive, const string& formatName);
        bool m_isActive; ///< is input log file activated / deactivated
        string m_formatName; ///< input log file format name
    };

    /// Auxiliary structure to track input log file format state
    struct TFormatNameState {
        TFormatNameState(const string& dateRegex, const string& inputFileName);
        string m_dateRegex; ///< date parsing regular expression
        string m_inputFileName; ///< input log file
    };


    map<string, TInputFileState*> m_inputFiles; ///< map of input log files and their state
    map<string, TFormatNameState*> m_format2RegexMap; ///< map of input log files formats and their regular expressions
    map<string, bool> m_configFiles; ///< map of configuration files and their state (bool - are they activated / deactivated)

    string m_fixedAndTimeParams[6];

    CAliasesManager* m_logDialogAliasManager;
    CAliasesManager* m_configDialogAliasManager;

    //----------------------------------------------------------------------------------------------

    string m_backupFolderName;
    string m_logFormatsResourcesFolderName;

    string m_inputBackupFileName;
    string m_configBackupFileName;
    string m_logFormatsFileName;

    bool loadState(); ///< Method that loads backup files when Forensic Analyzer starts
    bool loadInputFilesFromBackup(const string& folder, const string& fromFile);
    bool loadConfigFilesFromBackup(const string& folder, const string& fromFile);
    bool loadLogFormatsFromBackup(const string& folder, const string& fromFile);

    bool containsInputFile(const string& inputFileName) const;
    bool containsInputFile(const string& inputFileName, const string& formatName) const; ///< Stricter version of above
    bool containsConfigFile(const string& configFileName) const;

    void backupState() const; ///< Method that saves current state of Forensic Analyzer to backup file
    /** Note that backup methods will backup to ./<folder>/<toFile> and if this path doesn't exist, it will it */
    void backupInputFiles(const string& folder, const string& toFile) const;
    void backupConfigFiles(const string& folder, const string& toFile) const;
    void backupLogFormats(const string& folder, const string& toFile) const;

    //----------------------------------------------------------------------------------------------
    /** Note that there are 2 versions of logFormats.conf: one for backup in folder backup and second(evaluated) in folder res*/
    void createEvaluatedLogFormatsFile(const string& folder, const string& toFile) const; ///< This method creates evaluated file logFormats.conf that is needed by SEC+
    string buildSecCommand(string inputParams, string configParams, string fixedAndTimeParams, string optionalParams) const; ///< Method that creates SEC command according to be executed according to current state.
};


#endif /* CMODEL_H */

