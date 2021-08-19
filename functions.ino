void ResetSaidasPulsadas()
{
  for (int iPorta = 0; iPorta <= 15; iPorta++)
  {
    if (g_pulsoHabilita[iPorta])
    {
      if (millisAtual >= g_tempoInicioPulso[iPorta] + pulseTime)
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
        //memRtc.outValues = chip2.read8() << 8 | chip1.read8();
        //memRtc.setOutputs();
        SaveOutputs();
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
}

void LoopLedStatus()
{

  if (WiFi.getMode() == 1 && millisAtual > millisWifiLed)
  {
    int32_t rssi;

    checkOutput();

    chip3.write(LedWifiConnected, LOW);
    //if (!vConfigWIFI)
    //{
    millisWifiLed = millisAtual + 2000;
    //}
    rssi = getRSSI();
    //(!DEBUG_ON) ?:   Serial.println(String(rssi));

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
    millisIREnabled = 0;
    millisSendUDP = 0;
    for (uint8_t iPorta = 0; iPorta <= 15; iPorta++)
    {
      g_tempoInicioPulso[iPorta] = 0;
    }
  }
}

void NtpSetDateTimeNTP()
{
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, ntpServer);

  (!DEBUG_ON) ?: Serial.print("Config UTC: ");
  (!DEBUG_ON) ?: Serial.println(DevSet.utcConfig);
  const unsigned long initTimeSet = 946684800;
  int8_t tryGetTime = 0;

  timeClient.begin();

  while ((timeClient.getEpochTime() < initTimeSet) && (tryGetTime < 5))
  {
    timeClient.update();
    (!DEBUG_ON) ?: Serial.println(timeClient.getEpochTime());
    tryGetTime++;
    delay(100);
  }

  if (timeClient.getEpochTime() > initTimeSet)
  {
    timeClient.setTimeOffset(DevSet.utcConfig * 3600);
    (!DEBUG_ON) ?: Serial.println("Atualizado UTC");
    RtcDateTime dtNow(timeClient.getEpochTime() - initTimeSet);
    Rtc.year = dtNow.Year();
    Rtc.month = dtNow.Month();
    Rtc.day = dtNow.Day();
    Rtc.hour = dtNow.Hour();
    Rtc.minute = dtNow.Minute();
    Rtc.second = dtNow.Second();
    Rtc.set_time();
  }

  timeClient.end();

  Rtc.get_time();
  (!DEBUG_ON) ?: Serial.printf("Data e hora ajustados para: %02d/%02d/%04d %02d:%02d:%02d", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
  (!DEBUG_ON) ?: Serial.println();
}

void CheckSPIFFS()
{ //Check if SPIFFS is ok, otherwise format it
  SPIFFS.begin();
  File f = SPIFFS.open("/checkSPIFFS.txt", "w+");
  if (!f)
  {
    (!DEBUG_ON) ?: Serial.print(F("Creating file system... "));
    if (SPIFFS.format())
      (!DEBUG_ON) ?: Serial.println(F("OK"));
    else
      (!DEBUG_ON) ?: Serial.println(F("Fail"));
  }
  else
    (!DEBUG_ON) ?: Serial.println(F("Filesystem is OK"));

  f.close();
  SPIFFS.end();
}

int32_t getRSSI()
{
  int32_t rssi;
  (WiFi.getMode() == WIFI_STA) ? rssi = WiFi.RSSI() : rssi = -45;
  return rssi;
}

void SaveOutputs()
{
  memRtc.outValues = chip2.read8() << 8 | chip1.read8();
  if (TipoMemoria)
  {
    memRtc.setOutputs();
  }
}

uint16_t getInputs()
{
  return sensor2.read8() << 8 | sensor1.read8();
}

String getDevStatus()
{
  return "{\"mac\":" + gchipId +
         ",\"ip:\"" + CurrentIP() +
         ",\"v:\"" + Placa_Version +
         ",\"i:\"" + String(getInputs()) +
         ",\"o:\"" + String(memRtc.outValues) +
         ",\"s:\"" + String(getRSSI()) + "}";
}

void AsyncIRSend()
{
  if (irEnSend)
  {
    //sendirCMDAPI(irNumBits, irModel, irData, "", irPort);
    sendIRCMD(irData, "", irNumBits, irPort, irModel, irNumBits);
    irEnSend = false;
    irData = "";
    (!DEBUG_ON) ?: Serial.println(F("...IR Enviado"));
  }
}

void checkOutput()
{
  //(!DEBUG_ON) ?: Serial.println(lastOutputs);
  //(!DEBUG_ON) ?: Serial.println(memRtc.outValues);
  if (lastOutputs != memRtc.outValues)
  {
    sendCloud(true);
    lastOutputs = memRtc.outValues;
  }
}

String CurrentIP()
{
  String ip = WiFi.localIP().toString();
  if (ip == "(IP unset)")
    ip = DevSet.numberToIpString(DevSet.apWifiIP);
  return ip;
}

void scanI2c()
{
  const unsigned long waitDelay = 50;
  uint8_t devices;

  dispText[0] = "EXECUTING SETUP";
  dispText[4] = ("Starting devices..");

  // Testar Display
  Wire.beginTransmission(0x3C);
  if (Wire.endTransmission() == 0)
  {
    hasDisplay = true;
    DisplaySetup();
    (!DEBUG_ON) ?: Serial.println(F("Display OK"));
    UpdateDisplay("Display Present");
  }
  else
  {
    hasDisplay = false;
    (!DEBUG_ON) ?: Serial.print(F("Display Fails"));
  }

  delay(waitDelay);
  //Testar Out1
  Wire.beginTransmission(0x21);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("21 Fail"));
  delay(waitDelay);
  //Testar Out2
  Wire.beginTransmission(0x22);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("22 Fail"));
  delay(waitDelay);
  //Testar In1
  Wire.beginTransmission(0x23);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("23 Fail"));
  delay(waitDelay);
  //Testar In2
  Wire.beginTransmission(0x24);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("24 Fail"));
  delay(waitDelay);
  //Testar chip3
  Wire.beginTransmission(0x25);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("25 Fail"));
  delay(waitDelay);
  //Testar RTC
  Wire.beginTransmission(0x50);
  (Wire.endTransmission() == 0) ? devices++ : UpdateDisplay(F("50 Fail"));
  delay(waitDelay);

  (devices == 6) ? UpdateDisplay(F("Devices OK..")) : UpdateDisplay(F("Devices Fail.."));
}