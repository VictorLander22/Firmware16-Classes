
void convertConfig()
{ //converter configurações antigas para salvar na EEPROM
  File f;
  String ret;

  SPIFFS.begin();

  if (SPIFFS.exists("/index.html"))
  {

    if (SPIFFS.exists("/wifipadrao.txt"))
    {
      f = SPIFFS.open("/wifipadrao.txt", "r");
      ret = f.readStringUntil('|');
      bool wifipadrao = (ret == "0") ? false : true;
      f.close();
      SPIFFS.remove("/wifipadrao.txt");
      bitWrite(DevSet.mode, 2, wifipadrao); //b0:AllowApi, b1:UsaCloud, b2:wifiPadrao, b3:TipoMemoria ..
    }

    if (SPIFFS.exists("/ssid.txt"))
    {
      f = SPIFFS.open("/ssid.txt", "r");
      ret = f.readStringUntil('|');
      f.close();
      SPIFFS.remove("/ssid.txt");
      DevSet.wifiSSID = ret; //Limit 35 bytes;
    }

    if (SPIFFS.exists("/pass.txt"))
    {
      f = SPIFFS.open("/pass.txt", "r");
      ret = f.readStringUntil('|');
      f.close();
      SPIFFS.remove("/pass.txt");
      DevSet.wifiPwd = ret; //Limit 35 bytes;
    }

    if (SPIFFS.exists("/ip.txt"))
    {
      f = SPIFFS.open("/ip.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/ip.txt");
      DevSet.wifiIP = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/mask.txt"))
    {
      f = SPIFFS.open("/mask.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/mask.txt");
      DevSet.wifiMSK = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/gateway.txt"))
    {
      f = SPIFFS.open("/gateway.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/gateway.txt");
      DevSet.wifiGTW = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/httpuser.txt"))
    {
      SPIFFS.remove("/httpuser.txt");
      DevSet.httpUser = "keepin";
    }

    if (SPIFFS.exists("/httppass.txt"))
    {
      SPIFFS.remove("/httppass.txt");
      DevSet.httpPwd = "keepin";
    }

    if (SPIFFS.exists("/apipass.txt"))
      SPIFFS.remove("/apipass.txt");

    if (SPIFFS.exists("/alowapi.txt"))
      SPIFFS.remove("/alowapi.txt");

    if (SPIFFS.exists("/senhaap.txt"))
      SPIFFS.remove("/senhaap.txt");

    if (SPIFFS.exists("/cloud.txt"))
      SPIFFS.remove("/cloud.txt");

    DevSet.setMode();
    DevSet.setWifi();
    DevSet.setApWifiPwd();
    DevSet.setApiPwd();
    DevSet.setHttpSeg();

    SPIFFS.remove("/index.html");
  }

  SPIFFS.end();
}

void wifireset(AsyncWebServerRequest *request)
{
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  request->send(200, "text/html", "ESP resetado");
  DevSet.factoryReset();
  ESP.restart();
}

int retornaPorta(int vporta)
{
  if ((vporta >= 1) && (vporta <= 16))
  {
    return vporta - 1;
  }
  else
  {
    return -1;
  }
}

void IniciaRTC()
{
  if (!memRtc.getRtcBatStatus())
  {
    Rtc.year = 2000;
    Rtc.month = 1;
    Rtc.day = 1;
    Rtc.hour = 0;
    Rtc.minute = 0;
    Rtc.second = 0;
    Rtc.set_time();

    memRtc.setBateryMemStatus();
    memRtc.outValues = 255 << 8 | 255;
    memRtc.setOutputs();

    (!DEBUG_ON) ?: Serial.println(F("Set Date"));
    Rtc.get_time();
    (!DEBUG_ON) ?: Serial.printf("%02d/%02d/%04d %02d:%02d:%02d", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
    (!DEBUG_ON) ?: Serial.println();
  }

  RtcDateTime now;

  now = carregaHora();
}

RtcDateTime carregaHora()
{
  Rtc.get_time();

  RtcDateTime dt2(Rtc.year, Rtc.month, Rtc.day, Rtc.hour, Rtc.minute, Rtc.second);

  return dt2;
}

void valorwifi(AsyncWebServerRequest *request)
{

  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  // int32_t rssi;
  // rssi = WiFi.RSSI();

  request->send(200, "text/html", String(getRSSI()));
}

void fmodelo(AsyncWebServerRequest *request)
{
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  request->send(200, "text/html", "2");
}

String lerMemoria()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "r");
  String texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  return texto;
}

void Memoria()
{
  String retorno = "1"; //lerMemoria();
  if (retorno == "1")
  {

    (!DEBUG_ON) ?: Serial.printf("\nSet outputs ON: %d", memRtc.getOutputs());
    (!DEBUG_ON) ?: Serial.println();
    uint16_t outputs = memRtc.getOutputs();
    chip1.write8(outputs & 0xff);
    chip2.write8((outputs >> 8) & 0xff);
  }
  else
  {
    (!DEBUG_ON) ?: Serial.printf("\nSet outputs OFF");
    (!DEBUG_ON) ?: Serial.println();
    chip1.write8(255);
    chip2.write8(255);
  }
}

void fMemoria(AsyncWebServerRequest *request)
{
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  request->send(200, "text/html", "ok");

  if (request->arg("m") == "1")
  {
    bitWrite(DevSet.mode, 0, true);
  }
  else
  {
    bitWrite(DevSet.mode, 0, false);
  }
  DevSet.setMode();

  if bitRead (DevSet.mode, 0)
  {
    memRtc.outValues = chip2.read8() << 8 | chip1.read8();
    memRtc.setOutputs();
  }
  else
  {
    memRtc.outValues = 255 << 8 | 255;
    memRtc.setOutputs();
  }
}

void lerConfiguracao()
{
  // garante que o led do IR estará apagado
  IRsend irsend(16, true);
  irsend.begin();
  digitalWrite(16, LOW);

  vSenhaAP = DevSet.apWifiPwd;
  ApiPass = DevSet.apiPwd;
  AlowApi = bitRead(DevSet.mode, 0);
  usaCloud = bitRead(DevSet.mode, 1);
  vConfigWIFI = bitRead(DevSet.mode, 2);
  TipoMemoria = bitRead(DevSet.mode, 3);
}

void GravaCloud(AsyncWebServerRequest *request)
{
  // if (!request->authenticate(www_username, www_password))
  //   return request->requestAuthentication();

  if (request->arg("s") == Senha)
  {
    if (request->arg("f") == "w")
    {
      request->send(200, "text/html", "1");

      usaCloud = (request->arg("v") == "1") ? true : false;

      bitWrite(DevSet.mode, 1, usaCloud);
      DevSet.setMode();
      DevSet.showVariables();
    }
    else
    {
      request->send(200, "text/html", (usaCloud) ? "1" : "0");
    }
  }
  else
  {
    request->send(200, "text/html", "-1");
  }
}
