void DisplaySetup()
{
  // if (hasDisplay)
  // {
  //   display.init();
  //   display.flipScreenVertically();
  // }
}

// void UpdateDisplay(String text)
// {
//   if (hasDisplay)
//   {
//     display.clear();
//     delay(50);
//     for (uint8_t i = 2; i < 5; i++)
//     {
//       dispText[i - 1] = dispText[i];
//       display.drawString(1, dispY[i - 1], dispText[i - 1]);
//     }
//     display.drawString(1, dispY[0], dispText[0]);
//     dispText[4] = text;
//     display.drawString(1, dispY[4], dispText[4]);
//     display.display();
//   }
// }

void LoopDisplay()
{
  if ((millisAtual > (lastDisplay + 2000)) && hasDisplay)
  {
    clock2s = !clock2s;
    //DisplayPrint("");
  }
}

// void DisplayPrint(String msg)
// {
//   if (hasDisplay)
//   {
//     String sIn = "";
//     String sIn2 = "";
//     String sOut = "";
//     String sOut2 = "";
//     String srF = "F";
//     String srOK = "OK";

//     lastDisplay = millisAtual;
//     display.clear();

//     for (uint8_t bit = 0; bit <= 7; bit++)
//     {
//       (bitRead(~sensor1.read8(), bit)) ? sIn += String(bit + 1) : sIn += " ";
//       (bitRead(~sensor2.read8(), bit)) ? sIn2 += String(bit + 1) : sIn2 += " ";
//       (bitRead(~chip1.read8(), bit)) ? sOut += String(bit + 1) : sOut += " ";
//       (bitRead(~chip2.read8(), bit)) ? sOut2 += String(bit + 1) : sOut2 += " ";
//     }
//     sIn += ":" + sIn2;
//     sOut += ":" + sOut2;

//     //display.setFont(Cousine_Regular_12);
//     display.drawString(3, dispY[0], sOut);
//     display.drawString(3, dispY[1], sIn);

//     //display.setFont(ArialMT_Plain_10);
//     display.drawLine(0, 28, 127, 28);
//     //display.drawString(1, dispY[2], "WiFi: OK ");
//     //display.drawString(50, dispY[2], "Sinal");
//     //display.drawProgressBar(80, 31, 47, 8, rssi);
//     display.drawString(1, dispY[3], "IP: " + IpDispositivo.toString());
//     //display.drawString(121, dispY[3], String(clock2s));
//     display.drawString(90, dispY[3], String(ESP.getFreeHeap()) + " " + String(clock2s));

//     if (enReadIR)
//       display.drawString(1, dispY[4], F("Waiting for IR Code..."));
//     else if (msg == "")
//       display.drawString(1, dispY[4], "I:" + ((hasInternet) ? srOK : srF) + " M:" + ((hasMQTT) ? srOK : srF) + " C:" + ((hasCloud) ? srOK : srF));
//     else
//       display.drawString(1, dispY[4], msg);

//     display.display();
//   }
// }