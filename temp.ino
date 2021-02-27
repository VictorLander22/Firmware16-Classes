void teste()
{
  server.send(200, "text/html", "OK");
  if (DEBUG_ON)
    Serial.println(WiFi.status());
  if (DEBUG_ON)
    Serial.println(WiFi.softAPIP());
  if (DEBUG_ON)
    Serial.println(WiFi.localIP());
  if (DEBUG_ON)
    Serial.println(WiFi.getMode());
  //WiFiPhyMode_t mode;
  if (DEBUG_ON)
    Serial.println(WiFi.getPhyMode());
}

void showDateTime()
{
  if (DEBUG_ON)
    Serial.printf("Data hora: %u/%u/%u %u:%u:%u\n", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
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
    millisFreeMemory = millis() + 3000;
  }
}