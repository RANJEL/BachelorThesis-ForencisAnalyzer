/// Header for class CAliasesManager
/** \file caliasesmanager.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef CALIASESMANAGER_H
#define CALIASESMANAGER_H

#include <string>
#include <map>
#include <fstream>

#include "cfsfunctions.h"

using namespace std;

/// Class that contains available aliases. Enables to load or backup them and evaluate some expression.
class CAliasesManager {
public:
    /** \param backupFileName name of backup file that will be used for backup of all aliases */
    CAliasesManager(const string& backupFileName); ///< Constructor
    /** This method will parse given assignment and store VARIABLE and VALUE into map.
     *  \param assignment expected format: ${<VARIABLE>}=<VALUE>
     *  \return false if there was parsing error, or VARIABLE contains only white spaces, etc.
     */
    bool addAlias(const string& assignment); ///< Add new alias or modify existing
    void clearAllAliases(); ///< Clear all existing aliases
    map<string, string> getAllAliases() const; ///< Getter for all existing aliases
    /** \param input input to be evaluated
     *  \return evaluated input (= without any ${<VARIABLE>})
     */
    string evaluateInput(const string& input) const; ///< The key method that recursively evaluate given input until there are no ${<VARIABLE>}
    bool loadFromBackup(); ///< Load all aliases from backup file
    void backup() const; ///< Save all aliases to backup file
    
private:
    map<string, string> m_aliasesMap; ///< map <VARIABLE> -> <VALUE>
    
    string m_backupFolderName;
    string m_backupFileName;
    
    bool constainsAlias(const string& input, size_t& aliasStart, size_t& aliasEnd) const;
    bool parseAliasName(const string& input, const size_t& aliasStart, const size_t& aliasEnd, string& aliasName) const;
    bool parseAliasNameAndValue(const string& input, const size_t& aliasStart, const size_t& aliasEnd, string& aliasName, string& aliasValue) const;
    bool isSelfRecursive(const string& currentAliasName, string aliasRightSide) const;
};

#endif // CALIASESMANAGER_H
