void DisplaySetup()
{
  if (hasDisplay)
  {
    display.init();
    display.flipScreenVertically();
  }
}

void atualizaDisplay(uint8_t in1, uint8_t in2, uint8_t out1, uint8_t out2, bool enIRRec, String internet, String mqtt, String cloud, int16_t rssi)
{
  String sIn = "";
  String sIn2 = "";
  String sOut = "";
  String sOut2 = "";
  for (uint8_t bit = 0; bit <= 7; bit++)
  {
    (bitRead(in1, bit)) ? sIn += String(bit + 1) : sIn += " ";
    (bitRead(in2, bit)) ? sIn2 += String(bit + 1) : sIn2 += " ";
    (bitRead(out1, bit)) ? sOut += String(bit + 1) : sOut += " ";
    (bitRead(out2, bit)) ? sOut2 += String(bit + 1) : sOut2 += " ";
  }
  sIn += ":" + sIn2;
  sOut += ":" + sOut2;

  display.setFont(Cousine_Regular_12);
  display.drawString(3, dispY[0], sOut);
  display.drawString(3, dispY[1], sIn);

  display.setFont(ArialMT_Plain_10);
  display.drawLine(0, 28, 127, 28);
  //display.drawString(1, dispY[2], "WiFi: OK ");
  //display.drawString(50, dispY[2], "Sinal");
  //display.drawProgressBar(80, 31, 47, 8, rssi);
  display.drawString(1, dispY[3], "IP: " + IpDispositivo.toString());
  //display.drawString(1, dispY[3], IpDispositivo.toString());
  display.drawString(121, dispY[3], String(clock2s));
  if (enIRRec)
    display.drawString(1, dispY[4], F("Waiting for IR Code..."));
  else
    display.drawString(1, dispY[4], "I:" + internet + " M:" + mqtt + " C:" + cloud);
}

void UpdateDisplay(String text)
{
  if (hasDisplay)
  {
    display.clear();
    delay(50);
    for (uint8_t i = 2; i < 5; i++)
    {
      dispText[i - 1] = dispText[i];
      display.drawString(1, dispY[i - 1], dispText[i - 1]);
    }
    display.drawString(1, dispY[0], dispText[0]);
    dispText[4] = text;
    display.drawString(1, dispY[4], dispText[4]);
    display.display();
  }
}

void LoopDisplay()
{
  if ((millisAtual > (lastDisplay + 2000)) && hasDisplay)
  {
    lastDisplay = millisAtual;
    display.clear();
    String internet = (hasInternet) ? "OK" : "F";
    String mqtt = (hasMQTT) ? "OK" : "F";
    String cloud = (hasCloud) ? "OK" : "F";
    clock2s = !clock2s;
    atualizaDisplay(~sensor1.read8(), ~sensor2.read8(), ~chip1.read8(), ~chip2.read8(), enReadIR, internet, mqtt, cloud, 60);
    display.display();
  }
}