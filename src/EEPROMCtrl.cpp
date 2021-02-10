/*
*****************************************************************************
* EEPROMCtrlr
* Keepin - Leonardo Aquino
* Controle de escrita e leitura da EEPROM
* v1.0 - Implementação
*****************************************************************************
*/

#include "Arduino.h"
#include "EEPROMCtrl.h"

EEPROMCtrl::EEPROMCtrl()
{
}

int8_t EEPROMCtrl::getEEPROMInt8(byte offset)
{
  // Le dado Int8 da EEPROMCtrl::EEPROMClass::(–128 a 127)
  return int8(EEPROMCtrl::EEPROMClass::read(offset));
}

uint8_t EEPROMCtrl::getEEPROMUInt8(byte offset)
{
  // Le dado UInt8 da EEPROMCtrl::EEPROMClass::(0 a 255)
  return EEPROMCtrl::EEPROMClass::read(offset);
}

int16_t EEPROMCtrl::getEEPROMInt16(byte offset)
{
  // Le dado Int16 da EEPROMCtrl::EEPROMClass::(–32768 a 32767)
  return short(word(EEPROMCtrl::EEPROMClass::read(offset), EEPROMCtrl::EEPROMClass::read(offset + 1)));
}

uint16_t EEPROMCtrl::getEEPROMUInt16(byte offset)
{
  // Le dado UInt16 da EEPROMCtrl::EEPROMClass::(0 a 65535)
  return word(EEPROMCtrl::EEPROMClass::read(offset), EEPROMCtrl::EEPROMClass::read(offset + 1));
}

int32_t EEPROMCtrl::getEEPROMInt32(byte offset)
{
  // Le dado Int32 da EEPROMCtrl::EEPROMClass::(–2.147.483.648 a 2.147.483.647)
  int32 l;
  l = EEPROMCtrl::EEPROMClass::read(offset);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 1);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 2);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 3);
  return l;
}

uint32_t EEPROMCtrl::getEEPROMUInt32(byte offset)
{
  // Le dado UInt32 da EEPROMCtrl::EEPROMClass::(0 a 4.294.967.295)
  uint32 l;
  l = EEPROMCtrl::EEPROMClass::read(offset);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 1);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 2);
  l = l << 8;
  l |= EEPROMCtrl::EEPROMClass::read(offset + 3);
  return l;
}

String EEPROMCtrl::getEEPROMString(byte offset, word memSize)
{
  // Le String da EEPROM
  String s = "";
  while (offset < memSize)
  {
    if (EEPROMCtrl::EEPROMClass::read(offset) == 0)
    {
      break;
    }
    s += char(EEPROMCtrl::EEPROMClass::read(offset));
    offset++;
  }
  return s;
}

void EEPROMCtrl::setEEPROMInt8(byte offset, int8_t v)
{
  // Grava dado Int8 na EEPROMCtrl::EEPROMClass::(–128 a 127)
  EEPROMCtrl::EEPROMClass::write(offset, v);
}

void EEPROMCtrl::setEEPROMInt8(byte offset, uint8_t v)
{
  // Grava dado UInt8 na EEPROMCtrl::EEPROMClass::(0 a 255)
  EEPROMCtrl::EEPROMClass::write(offset, v);
}

void EEPROMCtrl::setEEPROMInt16(byte offset, int16_t v)
{
  // Grava dado Int16 na EEPROMCtrl::EEPROMClass::(–32768 a 32767)
  EEPROMCtrl::EEPROMClass::write(offset, highByte(v));
  EEPROMCtrl::EEPROMClass::write(offset + 1, lowByte(v));
}

void EEPROMCtrl::setEEPROMInt16(byte offset, uint16_t v)
{
  // Grava dado UInt16 na EEPROMCtrl::EEPROMClass::(0 a 65535)
  EEPROMCtrl::EEPROMClass::write(offset, highByte(v));
  EEPROMCtrl::EEPROMClass::write(offset + 1, lowByte(v));
}

void EEPROMCtrl::setEEPROMInt32(byte offset, int32_t v)
{
  // Grava dado Int32 na EEPROMCtrl::EEPROMClass::(–2147483648 a 2147483647)
  EEPROMCtrl::EEPROMClass::write(offset + 3, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset + 2, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset + 1, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset, byte(v));
}

void EEPROMCtrl::setEEPROMInt32(byte offset, uint32_t v)
{
  // Grava dado UInt32 na EEPROMCtrl::EEPROMClass::(0 a 4294967295)
  EEPROMCtrl::EEPROMClass::write(offset + 3, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset + 2, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset + 1, byte(v));
  v = v >> 8;
  EEPROMCtrl::EEPROMClass::write(offset, byte(v));
}

void EEPROMCtrl::setEEPROMString(byte offset, String s)
{
  // Grava String na EEPROM
  for (byte b = 0; b <= s.length(); b++)
  {
    EEPROMCtrl::EEPROMClass::write(offset + b, s[b]);
  }
}
