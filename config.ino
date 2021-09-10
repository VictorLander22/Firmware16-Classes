
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

void wifireset()
{
  gRequest->send(200, "text/html", "ESP resetado");
  DevSet.factoryReset();
  delay(300);
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
    //memRtc.outValues = 255 << 8 | 255;
    //memRtc.setOutputs();
    SaveOutputs();

    slogln(F("Set Date"));
    Rtc.get_time();
#ifdef DEBUG_ON
    Serial.printf("%02d/%02d/%04d %02d:%02d:%02d", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
#endif
    slogln();
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

void valorwifi()
{
  gRequest->send(200, "text/html", String(getRSSI()));
}

void fmodelo()
{
  gRequest->send(200, "text/html", "2");
}

String lerMemoria()
{
  String texto = "";
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "r");
  if (f)
    texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  return texto;
}

void Memoria()
{
  if (TipoMemoria)
  {
    slogln("\nSet outputs ON: " + memRtc.getOutputs());
    uint16_t outputs = memRtc.getOutputs();
    chip1.write8(outputs & 0xff);
    chip2.write8((outputs >> 8) & 0xff);
  }
}

void fMemoria()
{
  gRequest->send(200, "text/html", sdefOK);

  if (gRequest->arg("m") == "1")
  {
    bitWrite(DevSet.mode, 3, true);
    SaveOutputs();
  }
  else
  {
    bitWrite(DevSet.mode, 3, false);
    memRtc.outValues = 255 << 8 | 255;
    memRtc.setOutputs();
  }

  DevSet.setMode();
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

void GravaCloud()
{
  if (gRequest->arg("s") == Senha)
  {
    if (gRequest->arg("f") == "w")
    {
      gRequest->send(200, "text/html", "1");

      usaCloud = (gRequest->arg("v") == "1") ? true : false;
      hasCloud = usaCloud;
      bitWrite(DevSet.mode, 1, usaCloud);
      DevSet.setMode();
      DevSet.showVariables();
    }
    else
    {
      gRequest->send(200, "text/html", (usaCloud) ? "1" : "0");
    }
  }
  else
  {
    gRequest->send(200, "text/html", "-1");
  }
}
