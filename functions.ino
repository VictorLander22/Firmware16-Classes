void log(String msg)
{
  Serial.println(msg);
}

void ConfigurarWebServer(void)
{
  server.on("/", configuracao);
  server.on("/grava", grava);
  server.on("/ler", ler);
  server.on("/config", configuracao);
  server.on("/gravarwifi", gravawifi);
  server.on("/gravasenhawifi", gravasenhawifi);
  server.on("/gravasenhahttp", gravasenhahttp);
  server.on("/reset", wifireset);
  server.on("/reiniciar", reiniciar);
  server.on("/valida", valida);
  server.on("/controle", controle);
  server.on("/situacao", situacao);
  server.on("/chipid", retornachip);
  server.on("/chamaddns", chamaddns);
  //server.on("/mesh", mesh);
  //server.on("/consultamesh", meshconsulta);
  server.on("/consultaagenda", conagenda);
  server.on("/gravaragenda", gravaragenda);
  server.on("/atualizahora", atualizahora);
  server.on("/lersensores", lersensores);
  server.on("/gravasensor", gravasensor);
  server.on("/consultasensor", consensor);
  server.on("/gravadevice", gravadevice);
  server.on("/buscadevice", buscadevice);
  server.on("/executeupdate", executeupdate);
  server.on("/executeupdatebeta", executeupdateBeta);
  server.on("/versao", versao);
  server.on("/link", linkversao);
  server.on("/link", linkversaoBeta);
  //server.on("/limpadevice", limpadevice);
  server.on("/ultimodisparo", ultimodisp);
  server.on("/buscaNotificar", buscaNotificar);
  server.on("/gravanot", gravanot);
  server.on("/gravasms", gravasms);
  server.on("/consultasms", consultasms);
  server.on("/wifi", valorwifi);
  server.on("/listawifi", listawifi);
  server.on("/listawifi2", listawifi2);
  //IR
  server.on("/getir", getIR);
  server.on("/sendir", sendir);
  server.on("/habir", habir);
  //RF
  server.on("/habrf", habRF);
  server.on("/getrf", getRF);
  server.on("/gravarf", gravarf);
  server.on("/ultimodisparorf", ultimodisprf);
  server.on("/sendrf", sendRFp);
  server.on("/modelo", fmodelo);
  server.on("/memoria", fMemoria);
  server.on("/html", gravahtml);
  //server.on("/teste", testes2);
  server.on("/api", api);
  server.on("/apiativo", apiativo);
  server.on("/apiconfig", apiconfig);
  server.on("/alterasenhapi", alterasenhapi);
  server.on("/about", about);
  server.on("/gravacena", gravacena);
  server.on("/log", readlog);
  server.on("/gravacloud", GravaCloud);
  server.on("/dirarquivos", dirarquivos);
  server.on("/downloadfile", File_Download);
  server.on("/uploadfile", File_Upload);
  server.on(
      "/fupload", HTTP_POST, []() { server.send(200); }, handleFileUpload);
  server.on("/deletefile", File_Delete);
  //server.on("/cloud", cloud);
  //  server.on("/sendcloud", sendCloud);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("HTTP server started");
}

void ResetSaidasPulsadas()
{
  for (int iPorta = 0; iPorta <= 15; iPorta++)
  {
    if (g_pulsoHabilita[iPorta])
    {
      // proteção no caso de variavel estourar
      if (millisAtual < g_tempoInicioPulso[iPorta])
      {
        g_tempoInicioPulso[iPorta] = 0;
      }
      else if (millisAtual >= g_tempoInicioPulso[iPorta] + 500)
      {
        g_pulsoHabilita[iPorta] = false;
        if (iPorta < 8)
        {
          chip1.write(iPorta, HIGH);
        }
        else
        {
          chip2.write(iPorta - 8, HIGH);
        }
      }
    }
  }
}

void LoopResetFabrica()
{
  int ValorbuttonState = digitalRead(buttonState);

  if (ValorbuttonState == HIGH)
  {
    if (resetIntPulsado() == true)
    {
      ConfigEP();
      //Serial.println("reset pulsado");
    }
    else if (resetIntNormal() == true)
    {
      ConfigEN();
      //Serial.println("reset normal");
    }
    else
    {
      //Serial.print("reset ligado: ");
      //Serial.println(digitalRead(buttonState));
      wifireset2();
      ESP.restart();
    }
  }
}

void LoopLedStatus()
{
  if (chip3.read(LedWiFI) == LOW)
  {
    int32_t rssi;

    //      chip3.write(LedAmarelo, LOW);
    if (WiFi.status() == WL_CONNECTED)
    {
      if (vConfigWIFI == "0")
      {
        lastWifiTime = millisAtual;
      }
      rssi = WiFi.RSSI();
      //Serial.println(String(rssi));

      if (rssi >= -65)
      {
        chip3.write(LedVerde, LOW);
        chip3.write(LedAmarelo, HIGH);
      }
      else if (rssi < -65 && rssi >= -100)
      {
        chip3.write(LedVerde, HIGH);
        chip3.write(LedAmarelo, LOW);
      }
      else
      {
        chip3.write(LedVerde, HIGH);
        chip3.write(LedAmarelo, HIGH);
      }
    }
  }
}

void LoopLedRunning()
{
  //  if (HorarioAtual.Second() != Segundo)
  if (Segundo == -1 || (millisAtual - Segundo) > 200)
  {
    Segundo = millisAtual;
    //Segundo = HorarioAtual.Second();
    chip3.write(LedGeral, !chip3.read(LedGeral));
  }
}