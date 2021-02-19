void log(String msg)
{
  Serial.println(msg);
}

void ConfigurarWebServer(void)
{
  //server.on("/", configuracao);
  //server.on("/", handleHtmlConfig);
  server.on("/", handleHtmlConfig);
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
  server.on("/chipmac", RetornaChipMac);
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
  server.on("/listawifi", WifiNetworkScan);
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
  server.on("/testehtml", handleHtmlConfig);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/teste", teste);

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
      if (millisAtual >= g_tempoInicioPulso[iPorta] + 500)
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
  //int ValorbuttonState = digitalRead(buttonState);

  if (digitalRead(buttonState))
  {
    ESP.restart();
  };
  // {
  //   if (resetIntPulsado() == true)
  //   {
  //     ConfigEP();
  //     //Serial.println("reset pulsado");
  //   }
  //   else if (resetIntNormal() == true)s
  //   {
  //     ConfigEN();
  //     //Serial.println("reset normal");
  //   }
  //   else
  //   {
  //     //Serial.print("reset ligado: ");
  //     //Serial.println(digitalRead(buttonState));
  //     wifireset2();
  //     ESP.restart();
  //   }
  // }
}

void LoopLedStatus()
{

  if (WiFi.getMode() == 1 && millisAtual > millisWifiLed)
  {
    int32_t rssi;
    chip3.write(LedWifiConnected, LOW);
    if (!vConfigWIFI)
    {
      millisWifiLed = millisAtual + 2000;
    }
    rssi = WiFi.RSSI();
    //Serial.println(String(rssi));

    if (rssi >= -65)
    {
      chip3.write(LedWifiHI, LOW);
      chip3.write(LedWifiLOW, HIGH);
    }
    else if (rssi < -65 && rssi >= -100)
    {
      chip3.write(LedWifiHI, HIGH);
      chip3.write(LedWifiLOW, LOW);
    }
    else
    {
      chip3.write(LedWifiHI, HIGH);
      chip3.write(LedWifiLOW, HIGH);
    }
  }
}

void LoopLedRunning()
{
  if (millisAtual > millisLedRunning)
  {
    millisLedRunning = millisAtual + 200;
    chip3.write(LedRunning, !chip3.read(LedRunning));
  }
}

byte *conv4Bytes(uint32_t f1)
{
  byte *fb = (byte *)&f1;
  return fb;
}

uint32_t convUint32(byte *d)
{
  uint32_t adc_value;
  adc_value = *((long *)d);
  return adc_value;
}

void MillisResets()
{
  if (millisAtual == 0)
  {
    millisDebug = 0;
    millisMqttReconnect = 0;
    millisNetworkScan = 0;
    lastCnTime = 0;
    rfmilis = 0;
    millisWifiLed = 0;
    lastDebounceTime = 0;
    millisLedRunning = 0;
    for (uint8_t iPorta = 0; iPorta <= 15; iPorta++)
    {
      g_tempoInicioPulso[iPorta] = 0;
    }
  }
}