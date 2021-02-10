/*
*****************************************************************************
* EEPROMCtrlr
* Keepin - Leonardo Aquino
* Controle de escrita e leitura da EEPROM
* v1.0 - Implementação
*****************************************************************************
*/

#ifndef EEPROMCTRL_H
#define EEPROMCTRL_H

// Bibliotecas

#include "EEPROM.h"
//#include "TimeLib.h"

class EEPROMCtrl : public EEPROMClass
{
public:
  EEPROMCtrl();
  using EEPROMClass::begin;
  using EEPROMClass::commit;
  using EEPROMClass::end;

  //Funções para leitura da EEPROM
  int8_t getEEPROMInt8(byte offset);
  uint8_t getEEPROMUInt8(byte offset);
  int16_t getEEPROMInt16(byte offset);
  uint16_t getEEPROMUInt16(byte offset);
  int32_t getEEPROMInt32(byte offset);
  uint32_t getEEPROMUInt32(byte offset);
  String getEEPROMString(byte offset, word memSize);
  //Funções para gravação na EEPROM
  void setEEPROMInt8(byte offset, int8_t v);
  void setEEPROMInt8(byte offset, uint8_t v);
  void setEEPROMInt16(byte offset, int16_t v);
  void setEEPROMInt16(byte offset, uint16_t v);
  void setEEPROMInt32(byte offset, int32_t v);
  void setEEPROMInt32(byte offset, uint32_t v);
  void setEEPROMString(byte offset, String s);
};

#endif
