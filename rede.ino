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
          (!DEBUG_ON) ?: Serial.print("resetando");
          WiFi.disconnect();
          DevSet.factoryReset();
          ESP.restart();
        }

        delay(400);
        chip3.write(LedWifiConnected, !chip3.read(LedWifiConnected));
        (!DEBUG_ON) ?: Serial.print(".");
        waitingWifi++;
      }
      (!DEBUG_ON) ?: Serial.println("");
    }
    else if (WiFi.getMode() < 2)
    {
      wifiConectAP();
    }
  }
  if (WiFi.getMode() != 1)
    getAvalibleNetwork();
}

void listawifi2(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", vListaWifi);
}

void WifiNetworkScan(AsyncWebServerRequest *request)
{
  millisNetworkScan = millisAtual;
  getAvalibleNetwork();
  request->send(200, "text/html", vListaWifi);
}

void getAvalibleNetwork()
{
  int n = WiFi.scanComplete();
  scanningWifi = n;
  if (n >= 0)
  {
    //scanningWifi = n;
    (!DEBUG_ON) ?: Serial.printf("%d network(s) found\n", n);
    vListaWifi = "";
    for (int i = 0; i < n; i++)
    {
      (!DEBUG_ON) ?: Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
      vListaWifi += WiFi.SSID(i);
      vListaWifi += "|";
    }
    vListaWifi += "*";
    WiFi.scanDelete();
  }
  else if ((millisAtual > millisNetworkScan) && n != -1)
  {
    millisNetworkScan = millisAtual + 30000;
    //scanningWifi = n;
    (!DEBUG_ON) ?: Serial.println("\nNetwork scan started");
    WiFi.scanNetworks(true);
  }
}

void gravasenhawifi(AsyncWebServerRequest *request)
{
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  if (request->arg("s") == Senha)
  {
    if (vSenhaAP == request->arg("a"))
    {

      vSenhaAP = request->arg("v");

      if (vSenhaAP.length() >= 8)
      {
        request->send(200, "text/html", "ok");
        DevSet.apWifiPwd = vSenhaAP;
        DevSet.setApWifiPwd();
        (!DEBUG_ON) ?: Serial.println("Alterado: " + vSenhaAP);
      }
      else
      {
        request->send(200, "text/html", "-1");
      }
    }
    else
    {
      request->send(200, "text/html", "-1");
    }
  }
  else
  {
    request->send(200, "text/html", "-1");
  }
}

void gravasenhahttp(AsyncWebServerRequest *request)
{
  //args - [s=senha padrao] [u=new http user] [v=new http pwd] [ua=old http user] [a=old http pwd]
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  if (request->arg("s") == Senha)
  {
    String newHttpUser = request->arg("u");
    String newHttpPwd = request->arg("v");

    if ((newHttpUser.length() >= 4 && newHttpPwd.length() >= 4) && (request->arg("ua") == DevSet.httpUser && request->arg("a") == DevSet.httpPwd))
    {
      String ip = WiFi.localIP().toString();
      if (ip == "(IP unset)")
        ip = DevSet.numberToIpString(DevSet.apWifiIP);
      String restartPage(FPSTR(webRestart));
      restartPage.replace("#oldip#", ip);
      restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
      request->send_P(200, "text/html", restartPage.c_str());

      DevSet.httpUser = newHttpUser;
      DevSet.httpPwd = newHttpPwd;
      DevSet.setHttpSeg();

      (!DEBUG_ON) ?: Serial.println("Reiniciando sistema depois de alterar http senha");
      delay(500);
      ESP.restart();
    }
    else
    {
      request->send(200, "text/html", "-1");
    }
  }
  else
  {
    request->send(200, "text/html", "-1");
  }
}

// void prinScanResult(int networksFound)
// {
//   (!DEBUG_ON) ?: Serial.printf("%d prinScanResult network(s) found\n", networksFound);
//   for (int i = 0; i < networksFound; i++)
//   {
//     (!DEBUG_ON) ?: Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
//   }
// }

void wifiConectSTA()
{
  (!DEBUG_ON) ?: Serial.println(F("\nWifi trying conection in: STA MODE"));
  UpdateDisplay(F("Trying Wifi in: STA MODE"));
  WiFi.mode(WIFI_STA);
  tipoWifiAtual = 1;
  const char *ssid = DevSet.wifiSSID.c_str();
  UpdateDisplay(DevSet.wifiSSID);
  const char *password = DevSet.wifiPwd.c_str();
  IPAddress ip(DevSet.wifiIP);
  IPAddress subnet(DevSet.wifiMSK);
  IPAddress gateway(DevSet.wifiGTW);
  IPAddress dns(8, 8, 8, 8);

  (!DEBUG_ON) ?: Serial.println(ssid);
  // (!DEBUG_ON) ?:   Serial.println(password);
  // (!DEBUG_ON) ?:   Serial.println(ip);
  // (!DEBUG_ON) ?:   Serial.println(subnet);
  // (!DEBUG_ON) ?:   Serial.println(gateway);

  IpDispositivo = ip;
  local_IP = ip;
  setBroadcastIP(DevSet.wifiMSK);

  WiFi.config(ip, gateway, subnet, DNS1, DNS2);
  WiFi.begin(ssid, password);

  chip3.write(LedWifiConnected, LOW);
  chip3.write(LedWifiHI, HIGH);
  chip3.write(LedWifiLOW, HIGH);
}

void wifiConectAP()
{
  (!DEBUG_ON) ?: Serial.print("Rede não localizada: ");
  (!DEBUG_ON) ?: Serial.println(DevSet.wifiSSID);
  (!DEBUG_ON) ?: Serial.println("Wifi trying conection in: AP MODE");
  UpdateDisplay(DevSet.wifiSSID);
  UpdateDisplay(F("Trying Wifi in: AP MODE"));
  WiFi.mode(WIFI_AP);
  tipoWifiAtual = 2;
  //listawifi();
  //getAvalibleNetwork();
  //chip3.write(LedWiFI, HIGH);
  IPAddress local_IP(DevSet.apWifiIP);
  IPAddress gateway(DevSet.apWifiGTW);
  IPAddress subnet(DevSet.apWifiMSK);

  (!DEBUG_ON) ?: Serial.print("Setting soft-AP configuration ... ");
  (!DEBUG_ON) ?: Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  (!DEBUG_ON) ?: Serial.print("Setting soft-AP ... ");
  (!DEBUG_ON) ?: Serial.println("idencitifcador");
  //int chipId = ESP.getChipId();
  String NomeRede = "KEEPIN_" + gchipId;
  (!DEBUG_ON) ?: Serial.println(NomeRede);
  const char *nRede = NomeRede.c_str();
  (!DEBUG_ON) ?: Serial.println(nRede);

  //(!DEBUG_ON) ?:   Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

  if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
  {
    //wifireset2();
    DevSet.factoryReset();
  }

  (!DEBUG_ON) ?: Serial.print("Soft-AP IP address = ");
  (!DEBUG_ON) ?: Serial.println(WiFi.softAPIP());
  IpDispositivo = local_IP;
  setBroadcastIP(DevSet.apWifiMSK);

  chip3.write(LedWifiConnected, HIGH);
  chip3.write(LedWifiHI, HIGH);
  chip3.write(LedWifiLOW, HIGH);
}

void SetupPing()
{ //execute ping routine
  for (int i = 0; i < numDNSquery; i++)
  {
    if (ips[i])
    {
      if (!WiFi.hostByName(ips[i], addrs[i]))
        addrs[i].fromString(ips[i]);
    }

    Pings[i].on(true, [](const AsyncPingResponse &response) {
      IPAddress addr(response.addr);
      if (!response.answer)
        return false;
    });

    Pings[i].on(false, [](const AsyncPingResponse &response) {
      IPAddress addr(response.addr);
      if (response.total_recv > 1)
      {
        Serial.println(F("Internet connected"));
        numberPingResponse++;
      }
      else
        Serial.println(F("Internet disconnected"));

      if (numberPingResponse > 0)
      {
        enableConnection = true;
        hasInternet = true;
      }
      else
      {
        enableConnection = false;
        hasInternet = false;
      }
      return true;
    });
  }

  LoopPing();
}

void LoopPing()
{
  if (!client.connected() && (tipoWifiAtual != 2))
  {
    numberPingResponse = 0;
    for (int i = 0; i < numDNSquery; i++)
    {
      if (addrs[i].toString().c_str() != "255.255.255.255")
      {
        Serial.printf("started ping to %s:\n", addrs[i].toString().c_str());
        Pings[i].begin(addrs[i]);
      }
    }
  }
}

void UpdatePing()
{
  (!DEBUG_ON) ?: Serial.println(F("Atualizando endereços do ping"));
  for (int i; i < numDNSquery; i++)
  {
    if (ips[i])
    {
      if (!WiFi.hostByName(ips[i], addrs[i]))
        addrs[i].fromString(ips[i]);
    }
  }
}

void setBroadcastIP(uint32_t _currentSubnet)
{
  IPAddress _broadcastIP(_currentSubnet);
  for (int i = 0; i <= 3; i++)
  {
    Serial.println(_broadcastIP[i]);
    (_broadcastIP[i] == 0) ? _broadcastIP[i] = 255 : _broadcastIP[i] = IpDispositivo[i];
  }
  broadcastIP = _broadcastIP;
}