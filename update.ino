void executeupdate()
{
  gRequest->send(200, "text/html", sdefOK);
  shouldUpdate = 1;
}

void executeupdateBeta(bool isPost)
{
  if (isPost)
    gRequest->send(200, "text/html", sdefOK);
  shouldUpdate = 2;
}

void ExecuteUpdate()
{
  WiFiClient client;
  if (shouldUpdate != 0)
  {
    String url;
    //http://keepin.com.br/firmware/16/autoresidencial.ino.bin
    if (shouldUpdate == 1) //executa update
      url = "http://keepin.com.br/firmware/16/firmware16.bin";
    else if (shouldUpdate == 2) // executa update beta
      url = "http://keepin.com.br/firmware/16/beta/firmware16.bin";

    shouldUpdate = 0;

    slog("Iniciando Update em: ");

    slogln(url);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      //            gRequest->send(200, "text/html", "HTTP_UPDATE_FAILD Error: " + String(ESPhttpUpdate.getLastErrorString().c_str()));
      break;

    case HTTP_UPDATE_NO_UPDATES:
      slogln("HTTP_UPDATE_NO_UPDATES");
      //    gRequest->send(200, "text/html","HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      slogln(sdefOK);
      //    gRequest->send(200, "text/html", "HTTP_UPDATE_OK");
      break;
    }
  }
}

void versao()
{
  gRequest->send(200, "text/html", Placa_Version);
}

// void linkversao()
// {
//   gRequest->send(200, "text/html", "http://keepin.com.br/firmware/16/versao.txt");
// }

// void linkversaoBeta()
// {
//   gRequest->send(200, "text/html", "http://keepin.com.br/firmware/16/beta/versao.txt");
// }

// void logData(String dados)
// {
//   RtcDateTime HorarioAtual;
//   HorarioAtual = carregaHora();

//   char time[50];
//   sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());

//   dados = "Data: " + String(time) + " - " + dados;

//   SPIFFS.begin();
//   File f = SPIFFS.open("/log.txt", "a");
//   f.println(dados);
//   f.close();
//   SPIFFS.end();
// }

// void readlog()
// {
//   SPIFFS.begin();
//   String comando = gRequest->arg("c");

//   if (comando == "delete")
//   {
//     if (SPIFFS.remove("/log.txt"))
//     {
//       SPIFFS.end();
//       gRequest->send(200, "text/html", "remove");
//     }
//     else
//     {
//       SPIFFS.end();
//       gRequest->send(200, "text/html", "falha");
//     }
//   }
//   else if (comando == "read")
//   {
//     File f = SPIFFS.open("/log.txt", "r");
//     String dados = "<html><body><p><h3><strong>Log</strong></p></h3><br><ul>";
//     while (f.available())
//     {
//       dados = dados + "<li>" + f.readStringUntil('\n') + "</li>";
//     }
//     dados = dados + "</ul></body></html>";
//     f.close();
//     SPIFFS.end();
//     gRequest->send(200, "text/html", dados);
//   }
//   else
//   {
//     gRequest->send(200, "text/html", sdefOK);
//   }
// }