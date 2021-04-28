void teste()
{
  request->send(200, "text/html", "OK");
  (!DEBUG_ON) ?: Serial.println(WiFi.status());
  (!DEBUG_ON) ?: Serial.println(WiFi.softAPIP());
  (!DEBUG_ON) ?: Serial.println(WiFi.localIP());
  (!DEBUG_ON) ?: Serial.println(WiFi.getMode());
  //WiFiPhyMode_t mode;
  (!DEBUG_ON) ?: Serial.println(WiFi.getPhyMode());
}

void showDateTime()
{
  (!DEBUG_ON) ?: Serial.printf("Data hora: %u/%u/%u %u:%u:%u\n", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
  delay(200);
}

void FreeMemory(String functionName)
{
  static unsigned long millisFreeMemory = millis();
  if ((millis() > millisFreeMemory) || (millis() == 0))
  {
    Serial.print(functionName);
    Serial.print(F(" Aloc: "));
    Serial.print((espMemory - ESP.getFreeHeap()));
    Serial.print(F(" Livre: "));
    Serial.println(ESP.getFreeHeap());
    millisFreeMemory = millis() + 10000;
  }
}

void mostarEEProm(AsyncWebServerRequest *request)
{
  DevSet.getDeviceSettings();
  if (DEBUG_ON)
    DevSet.showVariables();
}

// void print_uint64_t(uint64_t num)
// {

//   char rev[128];
//   char *p = rev + 1;

//   while (num > 0)
//   {
//     *p++ = '0' + (num % 10);
//     num /= 10;
//   }
//   p--;
//   /*Print the number which is now in reverse*/
//   while (p > rev)
//   {
//     Serial.print(*p--);
//   }
// }