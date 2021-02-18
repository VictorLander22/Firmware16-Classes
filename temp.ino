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

void FreeMemory()
{
  static unsigned long last = millis();
  if (millis() - last > 3000)
  {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
}