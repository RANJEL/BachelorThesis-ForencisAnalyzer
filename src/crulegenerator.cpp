/// Implementation of crulegenerator.h
/** \file crulegenerator.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "crulegenerator.h"

CRuleGenerator::CRuleGenerator() {
    ostringstream os1;
    os1 << "type = Single" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl;
    m_rules["1) SINGLE RULE"] = os1.str().c_str();

    ostringstream os2;
    os2 << "type = SingleWithScript" << endl << "ptype = " << endl << "pattern = " << endl << "script = " << endl << "desc = " << endl << "action = write - ; " << endl;
    m_rules["2) SINGLEWITHSCRIPT RULE"] = os2.str().c_str();

    ostringstream os3;
    os3 << "type = SingleWithSuppress" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl
            << "window = " << endl;
    m_rules["3) SINGLEWITHSUPPRESS RULE"] = os3.str().c_str();

    ostringstream os4;
    os4 << "type = Pair" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl 
            << "ptype2 = " << endl << "pattern2 = " << endl << "desc2 = " << endl << "action2 = " << endl;
    m_rules["4) PAIR RULE"] = os4.str().c_str();

    ostringstream os5;
    os5 << "type = PairWithWindow" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl 
            << "ptype2 = " << endl << "pattern2 = " << endl << "desc2 = " << endl << "action2 = " << endl << "window = " << endl;
    m_rules["5) PAIRWITHWINDOW RULE"] = os5.str().c_str();

    ostringstream os6;
    os6 << "type = SingleWithThreshold" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; "
            << endl << "window = " << endl << "thresh = " << endl;
    m_rules["6) SINGLEWITHTHRESHOLD RULE"] = os6.str().c_str();

    ostringstream os7;
    os7 << "type = SingleWith2Thresholds" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl
            << "window = " << endl << "thresh = " << endl << "desc2 = " << endl << "action2 = " << endl << "window2 = " << endl << "thresh2 = " << endl;
    m_rules["7) SINGLEWITH2THRESHOLDS RULE"] = os7.str().c_str();

    ostringstream os8;
    os8 << "# Enter to the field type - number of rules (default is 1). And for 2..N define: ptypeN and patternN" << endl
            << "type = EventGroup[1]" << endl << "ptype = " << endl << "pattern = " << endl << "desc = " << endl << "action = write - ; " << endl
            << "window = " << endl;
    m_rules["8) EVENTGROUP RULE"] = os8.str().c_str();

    ostringstream os9;
    os9 << "type = Suppress" << endl << "ptype = " << endl << "pattern = " << endl;
    m_rules["9) SUPPRESS RULE"] = os9.str().c_str();    

    ostringstream os10;
    os10 << "type = Calendar" << endl << "time = " << endl << "desc = " << endl << "action = write - ; " << endl;
    m_rules["10) CALENDAR RULE"] = os10.str().c_str();

    ostringstream os11;
    os11 << "type = Jump" << endl << "ptype = " << endl << "pattern = " << endl;
    m_rules["11) JUMP RULE"] = os11.str().c_str();

    ostringstream os12;
    os12 << "type = Options" << endl;
    m_rules["12) OPTIONS RULE"] = os12.str().c_str();
}

QString CRuleGenerator::generateRule(const QString& ruleType) const {
    auto it = m_rules.find(ruleType);
    if (it == m_rules.end())
        return "";

    return it->second;
}
