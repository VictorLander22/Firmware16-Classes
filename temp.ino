void teste()
{
  FreeMemory(F("ptest()"));
  char *ptest = (char *)malloc(2048);
  strcpy(ptest, "abcdef");
  slogln(ptest);
  FreeMemory(F("ptest()"));
  free(ptest);
  FreeMemory(F("ptest()"));
}

void teste1()
{
  uint8_t qtdDados = 5;
  uint8_t numObj = 1;

  SPIFFS.begin();
  File f = SPIFFS.open("/ce_10.cfg", "r");
  uint16_t numDados = f.readStringUntil('\n').toInt() + 1;
  String str = f.readStringUntil('\0');
  f.close();
  SPIFFS.end();

  DynamicJsonDocument doc(numDados * JSON_ARRAY_SIZE(qtdDados) + JSON_ARRAY_SIZE(numDados) + JSON_OBJECT_SIZE(numObj));
  deserializeJson(doc, str);

  JsonArray array = doc.as<JsonArray>();
  for (JsonVariant v : array)
  {
    Serial.println(v[0].as<String>());
    // Serial.println(v[1].as<String>());
    // Serial.println(v[2].as<int>());
    // Serial.println(v[3].as<int>());
    // Serial.println(v[4].as<int>());
  }

  gRequest->send(200, sdefTextHtml, sdefOK);
}

void ReceiveSceneJson()
{ // criar o arquivo json para cena

  DynamicJsonDocument doc(gRequest->arg("json").toInt() * 16 * 5 + gRequest->arg("json").toInt() * 16 + 2 * 16);

  // parse a JSON array
  deserializeJson(doc, gRequest->arg("v"));

  // extract the values
  Serial.println((String)doc["q"]);

  JsonArray array = doc["v"].as<JsonArray>();
  for (JsonVariant v : array)
  {
    Serial.println(v[0].as<String>());
    Serial.println(v[1].as<String>());
    Serial.println(v[2].as<int>());
    Serial.println(v[3].as<int>());
    Serial.println(v[4].as<int>());
  }
}

void showDateTime()
{
  Serial.printf("Data hora: %u/%u/%u %u:%u:%u\n", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
  delay(200);
}

void mostarEEProm()
{
  DevSet.getDeviceSettings();
  DevSet.showVariables();
}
