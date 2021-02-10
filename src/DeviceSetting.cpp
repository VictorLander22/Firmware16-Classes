#include "Arduino.h"
#include "DeviceSetting.h"

DeviceSettingClass::DeviceSettingClass()
{
    config = 3; //b2:wifiPadrao, b1:UsaCloud, b0:AllowApi

    apiPwd = "25d55ad283aa400af464c76d713c07ad"; ///Limit 35 bytes;

    apWifiSSID = "KEEPIN_" + (String)ESP.getChipId(); //Limit 35 bytes;
    apWifiPwd = "12345678";                   //Limit 35 bytes;
    apWifiIP = 3232236545;                  //192.168.4.1
    apWifiMSK = 4294967040;                 //255.255.255.0
    apWifiGTW = 3232236545;                 //192.168.4.1
}

void DeviceSettingClass::saveConfig()
{
  DeviceSettingClass::EEPROMCtrl::begin(CFG_TOTAL);
  DeviceSettingClass::EEPROMCtrl::setEEPROMInt8(CFG_CONFIG, config);
  DeviceSettingClass::EEPROMCtrl::commit();
  DeviceSettingClass::EEPROMCtrl::end();
}

void DeviceSettingClass::saveWifi()
{
  DeviceSettingClass::EEPROMCtrl::begin(CFG_TOTAL);
  DeviceSettingClass::EEPROMCtrl::setEEPROMString(CFG_WIFISSID, wifiSSID);
  DeviceSettingClass::EEPROMCtrl::setEEPROMString(CFG_WIFIPWD, wifiPwd);
  DeviceSettingClass::EEPROMCtrl::setEEPROMInt32(CFG_WIFIIP, wifiIP);
  DeviceSettingClass::EEPROMCtrl::setEEPROMInt32(CFG_WIFIMSK, wifiMSK);
  DeviceSettingClass::EEPROMCtrl::setEEPROMInt32(CFG_WIFIGTW, wifiGTW);
  DeviceSettingClass::EEPROMCtrl::commit();
  DeviceSettingClass::EEPROMCtrl::end();
}

void DeviceSettingClass::saveApWifiPwd()
{
  DeviceSettingClass::EEPROMCtrl::begin(CFG_TOTAL);
  DeviceSettingClass::EEPROMCtrl::setEEPROMString(CFG_APWIFIPWD, apWifiPwd);
  DeviceSettingClass::EEPROMCtrl::commit();
  DeviceSettingClass::EEPROMCtrl::end();
}

void DeviceSettingClass::saveApiPwd()
{
  DeviceSettingClass::EEPROMCtrl::begin(CFG_TOTAL);
  DeviceSettingClass::EEPROMCtrl::setEEPROMString(CFG_APIPWD, apiPwd);
  DeviceSettingClass::EEPROMCtrl::commit();
  DeviceSettingClass::EEPROMCtrl::end();
}

void DeviceSettingClass::loadConfig()
{
  DeviceSettingClass::EEPROMCtrl::begin(CFG_TOTAL);
  config = DeviceSettingClass::EEPROMCtrl::getEEPROMUInt8(CFG_CONFIG);
  wifiSSID = DeviceSettingClass::EEPROMCtrl::getEEPROMString(CFG_WIFISSID,CFG_TOTAL);
  wifiPwd = DeviceSettingClass::EEPROMCtrl::getEEPROMString(CFG_WIFIPWD,CFG_TOTAL);
  wifiIP = DeviceSettingClass::EEPROMCtrl::getEEPROMUInt32(CFG_WIFIIP);
  wifiMSK = DeviceSettingClass::EEPROMCtrl::getEEPROMUInt32(CFG_WIFIMSK);
  wifiGTW = DeviceSettingClass::EEPROMCtrl::getEEPROMUInt32(CFG_WIFIGTW);
  apiPwd = DeviceSettingClass::EEPROMCtrl::getEEPROMString(CFG_APIPWD,CFG_TOTAL);
  apWifiPwd = DeviceSettingClass::EEPROMCtrl::getEEPROMString(CFG_APWIFIPWD,CFG_TOTAL);
  DeviceSettingClass::EEPROMCtrl::end();
}

uint32_t DeviceSettingClass::getMemSize()
{
  Serial.println(CFG_TOTAL);
  return CFG_TOTAL;
}

void DeviceSettingClass::setAPWifiSSID(String s)
{
  apWifiSSID = "KEEPIN_" + s;
}

String DeviceSettingClass::getAPWifiSSID()
{
  return apWifiSSID;
}

String DeviceSettingClass::dateTimeStr(const time_t &t)
{
  // Retorna data/hora no formato dd/mm/aaaa hh:mm:ss
  String s = "";
  if (day(t) < 10)
  {
    s += '0';
  }
  s += String(day(t)) + '/';
  if (month(t) < 10)
  {
    s += '0';
  }
  s += String(month(t)) + '/' + String(year(t)) + ' ';
  if (hour(t) < 10)
  {
    s += '0';
  }
  s += String(hour(t)) + ':';
  if (minute(t) < 10)
  {
    s += '0';
  }
  s += String(minute(t)) + ':';
  if (second(t) < 10)
  {
    s += '0';
  }
  s += String(second(t));
  return s;
}
