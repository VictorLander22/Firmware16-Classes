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
          Serial.print("resetando");
          WiFi.disconnect();
          DevSet.factoryReset();
          ESP.restart();
        }

        delay(400);
        chip3.write(LedWifiConnected, !chip3.read(LedWifiConnected));
        Serial.print(".");
        waitingWifi++;
      }
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
    Serial.printf("%d network(s) found\n", n);
    vListaWifi = "";
    for (int i = 0; i < n; i++)
    {
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

      Serial.println("Alterado: " + vSenhaAP);
    }
    else
    {
      server.send(200, "text/html", "-1");
      Serial.println("Senha invalida: Atual = " + vSenhaAP + " Informada: " + CurrentPass);
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
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
  Serial.printf("%d prinScanResult network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
}

void gravawifi()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  //server.send(200, "text/html", F("<html>ok<meta charset='UTF-8'><script>alert('Configuração salva.');history.back()</script></html>"));
  server.send(200, "text/html", F("<html>ok<meta charset='UTF-8'><script>history.back()</script></html>"));
  String wifiSSID = server.arg("txtnomerede");
  String wifiPWD = server.arg("txtsenha");
  const char *wifiIP = server.arg("txtip").c_str();
  const char *wifiMSK = server.arg("txtmascara").c_str();
  const char *wifiGTW = server.arg("txtgateway").c_str();
  bitWrite(DevSet.mode, 2, 0); //wifiPadrao
  DevSet.wifiSSID = wifiSSID;
  DevSet.wifiPwd = wifiPWD;
  DevSet.wifiIP = DevSet.ipStringToNumber(wifiIP);
  DevSet.wifiMSK = DevSet.ipStringToNumber(wifiMSK);
  DevSet.wifiGTW = DevSet.ipStringToNumber(wifiGTW);
  DevSet.setWifi();
  Serial.println(F("New WIFI Settings"));
  DevSet.showVariables();

  //gravahtml();
}

void wifiConectSTA()
{
  Serial.println(F("\nWifi trying conection in: STA MODE"));
  WiFi.mode(WIFI_STA);
  tipoWifiAtual = 1;
  const char *ssid = DevSet.wifiSSID.c_str();
  const char *password = DevSet.wifiPwd.c_str();
  IPAddress ip(DevSet.wifiIP);
  IPAddress subnet(DevSet.wifiMSK);
  IPAddress gateway(DevSet.wifiGTW);
  IPAddress dns(8, 8, 8, 8);

  Serial.println(ssid);
  // Serial.println(password);
  // Serial.println(ip);
  // Serial.println(subnet);
  // Serial.println(gateway);

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
  Serial.print("Rede não localizada: ");
  Serial.println(DevSet.wifiSSID);
  Serial.println("Wifi trying conection in: AP MODE");
  WiFi.mode(WIFI_AP);
  tipoWifiAtual = 2;
  //listawifi();
  //getAvalibleNetwork();
  //chip3.write(LedWiFI, HIGH);
  IPAddress local_IP(DevSet.apWifiIP);
  IPAddress gateway(DevSet.apWifiGTW);
  IPAddress subnet(DevSet.apWifiMSK);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println("idencitifcador");
  //int chipId = ESP.getChipId();
  String NomeRede = "KEEPIN_" + gchipId;
  Serial.println(NomeRede);
  const char *nRede = NomeRede.c_str();
  Serial.println(nRede);

  //Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

  if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
  {
    //wifireset2();
    DevSet.factoryReset();
  }

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  IpDispositivo = local_IP;

  chip3.write(LedWifiConnected, HIGH);
  chip3.write(LedWifiHI, HIGH);
  chip3.write(LedWifiLOW, HIGH);
}