#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include "Arduino.h"
#include "IPAddress.h"
#include "Regexp.h"

class StringHelper
{
public:
    static String splitString(String data, char separator, int index = 0)
    {
        int found = 0;
        int strIndex[] = {0, -1};
        int maxIndex = data.length();

        for (int i = 0; i <= maxIndex && found <= index; i++)
        {
            if (data.charAt(i) == separator || i == maxIndex)
            {
                found++;
                strIndex[0] = strIndex[1] + 1;
                strIndex[1] = (i == maxIndex) ? i + 1 : i;
            }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    }

    static bool match(String text, String pattern)
    {
        MatchState ms;
        ms.Target(const_cast<char *>(text.c_str()));
        char matchResult = ms.Match(const_cast<char *>(pattern.c_str()));

        return matchResult == REGEXP_MATCHED;
    }
};

#endif