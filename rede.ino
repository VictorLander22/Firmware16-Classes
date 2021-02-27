void LoopReconexao()
{
  // reconexao
  if ((!vConfigWIFI && ((WiFi.getMode() != WIFI_STA) || (WiFi.status() != WL_CONNECTED))) || (WiFi.getMode() == WIFI_OFF))
  {
    conectar();
  }
}

void conectar()
{
  //WiFi.disconnect();
  //WiFi.softAPdisconnect();
  DevSet.getDeviceSettings();

  vConfigWIFI = bitRead(DevSet.mode, 2);

  //String retorno = wifiPadrao();
  //vConfigWIFI = retorno;
  if (scanningWifi >= 0)
  {
    if (!vConfigWIFI && (vListaWifi.lastIndexOf(DevSet.wifiSSID) >= 0)) //Wifi STA
    {

      wifiConectSTA();

      int waitingWifi = 0;

      while (WiFi.status() != WL_CONNECTED)
      {
        if (waitingWifi > 20 && WiFi.getMode() < 2)
        {
          wifiConectAP();
          break;
        }

        if (digitalRead(buttonState) == HIGH)
        {
          if (DEBUG_ON)
            Serial.print("resetando");
          WiFi.disconnect();
          DevSet.factoryReset();
          ESP.restart();
        }

        delay(400);
        chip3.write(LedWifiConnected, !chip3.read(LedWifiConnected));
        if (DEBUG_ON)
          Serial.print(".");
        waitingWifi++;
      }
      if (DEBUG_ON)
        Serial.println("");
    }
    else if (WiFi.getMode() < 2)
    {
      wifiConectAP();
    }
  }
  if (WiFi.getMode() != 1)
    getAvalibleNetwork();
}

void listawifi2()
{
  server.send(200, "text/html", vListaWifi);
}

void WifiNetworkScan()
{
  millisNetworkScan = millisAtual;
  getAvalibleNetwork();
  server.send(200, "text/html", vListaWifi);
}

void getAvalibleNetwork()
{
  int n = WiFi.scanComplete();
  scanningWifi = n;
  if (n >= 0)
  {
    //scanningWifi = n;
    if (DEBUG_ON)
      Serial.printf("%d network(s) found\n", n);
    vListaWifi = "";
    for (int i = 0; i < n; i++)
    {
      if (DEBUG_ON)
        Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
      vListaWifi += WiFi.SSID(i);
      vListaWifi += "|";
    }
    vListaWifi += "*";
    WiFi.scanDelete();
  }
  else if ((millisAtual > millisNetworkScan) && n != -1)
  {
    millisNetworkScan = millisAtual + 15000;
    //scanningWifi = n;
    if (DEBUG_ON)
      Serial.println("\nNetwork scan started");
    WiFi.scanNetworks(true);
  }
}

void gravasenhawifi()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String req = server.arg("v");
  String CurrentPass = server.arg("a");

  if (vSenha == Senha)
  {
    if (req == "")
    {
      req = "12345678"; // se não houver registro, vai para o padrão
    }

    if (vSenhaAP == CurrentPass)
    {

      server.send(200, "text/html", "ok");
      SPIFFS.begin();
      File f = SPIFFS.open("/senhaap.txt", "w");

      f.println(req + "|");
      f.close();

      SPIFFS.end();
      vSenhaAP = req;

      if (DEBUG_ON)
        Serial.println("Alterado: " + vSenhaAP);
    }
    else
    {
      server.send(200, "text/html", "-1");
      if (DEBUG_ON)
        Serial.println("Senha invalida: Atual = " + vSenhaAP + " Informada: " + CurrentPass);
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
    if (DEBUG_ON)
      Serial.println("erro de senha de comunicacao: Senha Registrada: " + Senha + " senha enviada: " + vSenha);
  }
}

void gravasenhahttp()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = server.arg("s");
  String vreq = server.arg("v");
  String CurrentPass = server.arg("a");
  String vUsuarioAntigo = server.arg("ua");
  String vUsuariohttp = server.arg("u");

  if (vSenha == Senha)
  {
    if (vreq == "" || vreq == " ")
    {
      vreq = "keepin"; // se não houver registro, vai para o padrão
    }

    if (vUsuariohttp == "" || vUsuariohttp == " ")
    {
      vUsuariohttp = "keepin"; // se não houver usuario vai para o padrao
    }

    if (senha1 == CurrentPass && vUsuarioAntigo == usuario1)
    {
      server.send(200, "text/html", "ok");
      seg.gravar(vUsuariohttp, vreq);
      if (DEBUG_ON)
        Serial.println("Reiniciando sistema depois de alterar http senha");

      delay(500);
      ESP.restart();
    }
    else
    {
      server.send(200, "text/html", "-1");
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void prinScanResult(int networksFound)
{
  if (DEBUG_ON)
    Serial.printf("%d prinScanResult network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    if (DEBUG_ON)
      Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
}

void wifiConectSTA()
{
  if (DEBUG_ON)
    Serial.println(F("\nWifi trying conection in: STA MODE"));
  WiFi.mode(WIFI_STA);
  tipoWifiAtual = 1;
  const char *ssid = DevSet.wifiSSID.c_str();
  const char *password = DevSet.wifiPwd.c_str();
  IPAddress ip(DevSet.wifiIP);
  IPAddress subnet(DevSet.wifiMSK);
  IPAddress gateway(DevSet.wifiGTW);
  IPAddress dns(8, 8, 8, 8);

  if (DEBUG_ON)
    Serial.println(ssid);
  // if (DEBUG_ON) Serial.println(password);
  // if (DEBUG_ON) Serial.println(ip);
  // if (DEBUG_ON) Serial.println(subnet);
  // if (DEBUG_ON) Serial.println(gateway);

  IpDispositivo = ip;
  local_IP = ip;

  WiFi.config(ip, gateway, subnet, DNS1, DNS2);
  WiFi.begin(ssid, password);

  chip3.write(LedWifiConnected, LOW);
  chip3.write(LedWifiHI, HIGH);
  chip3.write(LedWifiLOW, HIGH);
}

void wifiConectAP()
{
  if (DEBUG_ON)
    Serial.print("Rede não localizada: ");
  if (DEBUG_ON)
    Serial.println(DevSet.wifiSSID);
  if (DEBUG_ON)
    Serial.println("Wifi trying conection in: AP MODE");
  WiFi.mode(WIFI_AP);
  tipoWifiAtual = 2;
  //listawifi();
  //getAvalibleNetwork();
  //chip3.write(LedWiFI, HIGH);
  IPAddress local_IP(DevSet.apWifiIP);
  IPAddress gateway(DevSet.apWifiGTW);
  IPAddress subnet(DevSet.apWifiMSK);

  if (DEBUG_ON)
    Serial.print("Setting soft-AP configuration ... ");
  if (DEBUG_ON)
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  if (DEBUG_ON)
    Serial.print("Setting soft-AP ... ");
  if (DEBUG_ON)
    Serial.println("idencitifcador");
  //int chipId = ESP.getChipId();
  String NomeRede = "KEEPIN_" + gchipId;
  if (DEBUG_ON)
    Serial.println(NomeRede);
  const char *nRede = NomeRede.c_str();
  if (DEBUG_ON)
    Serial.println(nRede);

  //if (DEBUG_ON) Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

  if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
  {
    //wifireset2();
    DevSet.factoryReset();
  }

  if (DEBUG_ON)
    Serial.print("Soft-AP IP address = ");
  if (DEBUG_ON)
    Serial.println(WiFi.softAPIP());
  IpDispositivo = local_IP;

  chip3.write(LedWifiConnected, HIGH);
  chip3.write(LedWifiHI, HIGH);
  chip3.write(LedWifiLOW, HIGH);
}