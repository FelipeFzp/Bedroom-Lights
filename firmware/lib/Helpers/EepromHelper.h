#ifndef EEPROMHELPER_H
#define EEPROMHELPER_H

#include "Arduino.h"
#include "EEPROM.h"

#define EEPROM_SIZE 200

class EepromHelper
{

public:
    static void EEPROM_writeString(char add, String data)
    {
        EEPROM.begin(EEPROM_SIZE);

        int _size = data.length();
        int i;
        for (i = 0; i < _size; i++)
        {
            EEPROM.write(add + i, data[i]);
        }
        EEPROM.write(add + _size, '\0'); //Add termination null character for String Data

        EEPROM.end();
    }

    static String EEPROM_readString(char add)
    {
        EEPROM.begin(EEPROM_SIZE);

        int i;
        char data[255]; //Max 100 Bytes
        int len = 0;
        unsigned char k;
        k = EEPROM.read(add);
        while (k != '\0' && len < 500) //Read until null character
        {
            k = EEPROM.read(add + len);
            data[len] = k;
            len++;
        }
        data[len] = '\0';

        EEPROM.end();

        return String(data);
    }
};

#endif