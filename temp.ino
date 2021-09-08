void teste()
{
  String functionName = gRequest->arg("fn");
  (!DEBUG_ON) ?: Serial.println(functionName);
  //gRequest = request;
  //asyncExecuteFunction = true;
  if (functionName == "teste")
  {
    gRequest->send(200, "text/html", "OK");
  }
  else
  {
    gRequest->send(200, "text/html", "NOK");
  }
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

void mostarEEProm()
{
  DevSet.getDeviceSettings();
  if (DEBUG_ON)
    DevSet.showVariables();
}
