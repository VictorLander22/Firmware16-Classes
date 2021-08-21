void teste(AsyncWebServerRequest *request)
{
  gRequest = request;
  asyncExecuteFunction = true;
  request->send(200, "text/html", "OK");
}

void asyncFunctions()
{

  if (asyncExecuteFunction)
  {
    asyncExecuteFunction = false;

    for (size_t i = 0; i < gRequest->args(); i++)
    {
      (!DEBUG_ON) ?: Serial.println("[" + (String)i + "] " + gRequest->getParam(i)->name() + " : " + gRequest->getParam(i)->value());
    }

    if (gRequest->url() == "/backupesp")
    {
      AsyncBackupEsp();
    }
    else if (gRequest->url() == "/restoreesp")
    {
      AsyncRestoreEsp();
    }
    else if (gRequest->url() == "/teste")
    {
      AsyncFormatEsp();
    }
    else if (gRequest->url() == "/gravasensor")
    {
      AsyncSaveInputConfig();
    }
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

void mostarEEProm(AsyncWebServerRequest *request)
{
  DevSet.getDeviceSettings();
  if (DEBUG_ON)
    DevSet.showVariables();
}
