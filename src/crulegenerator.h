/// Header for class CRuleGenerator
/** \file crulegenerator.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef CRULEGENERATOR_H
#define CRULEGENERATOR_H

#include <QString>
#include <sstream>
#include <map>

using namespace std;

/// Class containing all available rule types and their mandatory fields. It provides interface to generate rules based on given SEC+ rule type.
class CRuleGenerator {
public:
    /** There are SEC+ rule definitions*/
    CRuleGenerator(); ///< Constructor
    /** \param ruleType type of the rule you want to generate
     *  \return generated rule, that you can insert to configuration file. Note that generated "action = write - ; " is important for further processing, so keep this action there (you are free too add another actions)
     */
    QString generateRule(const QString& ruleType) const; ///< Generate rule based on given SEC+ rule type
    
private:
    map<QString, QString> m_rules; ///< map <Rule Type> -> <Mandatory Fields>
};


#endif // CRULEGENERATOR_H
