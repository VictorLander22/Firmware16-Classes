void executeupdate(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", "ok");
  //http://keepin.com.br/firmware/16/autoresidencial.ino.bin
  t_httpUpdate_return ret = ESPhttpUpdate.update("http://keepin.com.br/firmware/16/firmware16.bin");
  //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    (!DEBUG_ON) ?: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    //            request->send(200, "text/html", "HTTP_UPDATE_FAILD Error: " + String(ESPhttpUpdate.getLastErrorString().c_str()));
    break;

  case HTTP_UPDATE_NO_UPDATES:
    (!DEBUG_ON) ?: Serial.println("HTTP_UPDATE_NO_UPDATES");
    //    request->send(200, "text/html","HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    (!DEBUG_ON) ?: Serial.println("ok");
    //    request->send(200, "text/html", "HTTP_UPDATE_OK");
    break;
  }
}

void executeupdateBeta(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  request->send(200, "text/html", "ok");
  //http://keepin.com.br/firmware/16/autoresidencial.ino.bin
  t_httpUpdate_return ret = ESPhttpUpdate.update("http://keepin.com.br/firmware/16/beta/firmware16.bin");
  //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    (!DEBUG_ON) ?: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    //            request->send(200, "text/html", "HTTP_UPDATE_FAILD Error: " + String(ESPhttpUpdate.getLastErrorString().c_str()));
    break;

  case HTTP_UPDATE_NO_UPDATES:
    (!DEBUG_ON) ?: Serial.println("HTTP_UPDATE_NO_UPDATES");
    //    request->send(200, "text/html","HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    (!DEBUG_ON) ?: Serial.println("ok");
    //    request->send(200, "text/html", "HTTP_UPDATE_OK");
    break;
  }
}

void versao(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", Placa_Version);
}

void linkversao(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", "http://keepin.com.br/firmware/16/versao.txt");
}

void linkversaoBeta(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", "http://keepin.com.br/firmware/16/beta/versao.txt");
}

void logData(String dados)
{
  RtcDateTime HorarioAtual;
  HorarioAtual = carregaHora();

  char time[50];
  sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());

  dados = "Data: " + String(time) + " - " + dados;

  SPIFFS.begin();
  File f = SPIFFS.open("/log.txt", "a");
  f.println(dados);
  f.close();
  SPIFFS.end();
}

void readlog(AsyncWebServerRequest *request)
{
  SPIFFS.begin();
  String comando = request->arg("c");

  if (comando == "delete")
  {
    if (SPIFFS.remove("/log.txt"))
    {
      SPIFFS.end();
      request->send(200, "text/html", "remove");
    }
    else
    {
      SPIFFS.end();
      request->send(200, "text/html", "falha");
    }
  }
  else if (comando == "read")
  {
    File f = SPIFFS.open("/log.txt", "r");
    String dados = "<html><body><p><h3><strong>Log</strong></p></h3><br><ul>";
    while (f.available())
    {
      dados = dados + "<li>" + f.readStringUntil('\n') + "</li>";
    }
    dados = dados + "</ul></body></html>";
    f.close();
    SPIFFS.end();
    request->send(200, "text/html", dados);
  }
  else
  {
    request->send(200, "text/html", "ok");
  }
}