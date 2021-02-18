void teste()
{
  server.send(200, "text/html", configuracao);
  Serial.println(WiFi.status());
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());
  //WiFiPhyMode_t mode;
  Serial.println(WiFi.getPhyMode());
}