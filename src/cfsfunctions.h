/// Header for class CFsFunctions
/** \file cfsfunctions.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef CFSFUNCTIONS_H
#define CFSFUNCTIONS_H

#include <string>
#include <algorithm>

using namespace std;

/// Auxiliary class containing usefull static methods only.
class CFsFunctions {
public:
    static string getFileNameFromAbsolutePath(const string& fileName); ///< Get file name with extension from absolute path
    static string getRelativePath(const string& folder, const string& file); ///< This method returns relative path ./<folder>/<file>
    static void createPathIfDoesntExist(const string& folder, const string& file); ///< Creates relative ./<folder>/<file> if this path doesn't exist
    static string generateFormatNameFromFileName(const string& fileName); ///< Generates format name from file name, that is currently relative path without file extension
    /**
     * \param string output parameter that will contain trimmed string
     */
    static void trimString(string& string); ///< Erase whitespaces from given string
};

#endif // CFSFUNCTIONS_H
