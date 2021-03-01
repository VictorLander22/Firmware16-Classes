bool ConfigEN()
{
  String texto = "1|6|1|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "2|6|2|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "3|6|3|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "4|6|4|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "5|6|5|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "6|6|6|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "7|6|7|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "8|6|8|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";

  texto += "9|6|9|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "10|6|10|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "11|6|11|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "12|6|12|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "13|6|13|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "14|6|14|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "15|6|15|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "16|6|16|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|*";

  gravasensor2(texto);

  //(!DEBUG_ON) ?:   Serial.println(texto);
  return true;
}

bool ConfigEP()
{
  String texto = "1|5|1|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "2|5|2|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "3|5|3|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "4|5|4|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "5|5|5|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "6|5|6|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "7|5|7|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "8|5|8|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";

  texto += "9|5|9|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "10|5|10|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "11|5|11|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "12|5|12|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "13|5|13|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "14|5|14|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "15|5|15|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|";
  texto += "16|5|16|1|" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + vchipId + "|0|0|0|0|0|*";

  gravasensor2(texto);

  //(!DEBUG_ON) ?:   Serial.println(texto);
  return true;
}

bool resetIntNormal()
{
  int val1 = sensor1.read(0);
  int val2 = sensor1.read(1);
  int val3 = sensor1.read(2);

  if (val1 == 0 && val2 == 0 && val3 == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool resetIntPulsado()
{
  int val1 = sensor1.read(0);
  int val2 = sensor1.read(1);
  int val3 = sensor1.read(2);
  int val4 = sensor1.read(3);

  if (val1 == 0 && val2 == 0 && val3 == 0 && val4 == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}