void teste()
{
  server.send(200, "text/html", "OK");
  Serial.println(WiFi.status());
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());
  //WiFiPhyMode_t mode;
  Serial.println(WiFi.getPhyMode());
}

void showDateTime()
{
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
    millisFreeMemory = millis() + 2000;
  }
}