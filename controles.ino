void executaPulso(int porta)
{
  if (porta < 8)
  {
    chip1.write(porta, LOW);
  }
  else
  {
    chip2.write(porta - 8, LOW);
  }
  g_tempoInicioPulso[porta] = millisAtual;
  g_pulsoHabilita[porta] = true;
}

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base)
{
  for (int i = 0; i < maxBytes; i++)
  {
    bytes[i] = strtoul(str, NULL, base);
    str = strchr(str, sep);
    if (str == NULL || *str == '\0')
    {
      break;
    }
    str++;
  }
}

void retornachip()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", vchipId);
}

void RetornaChipMac()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", gchipId);
}

void controle()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  //(!DEBUG_ON) ?:   Serial.println("user: " + String(www_username) + " - pass: " + String(www_password));
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");

  String p = server.arg("p");
  String k = server.arg("k");
  String f = server.arg("f");
  String Nome = server.arg("n");
  int porta = p.toInt();
  int Tipoa = server.arg("pu").toInt();

  //  if (porta == 1)
  //  {
  //porta = 14;
  //}
  porta = retornaPorta(porta);
  if (porta >= 0)
    //pinMode(porta, OUTPUT);

    if (k == "kdi9e")
    {

      if (f == "true")
      {
        //digitalWrite(porta, 1);
        LigaDesliga(porta, HIGH, Nome, Tipoa);
        (!DEBUG_ON) ?: Serial.println("led 1 ligado - Porta: " + String(porta));
      }
      else
      {
        //digitalWrite(porta, 0);
        LigaDesliga(porta, LOW, Nome, Tipoa);
        (!DEBUG_ON) ?: Serial.println("led 1 desligado - Porta: " + String(porta));
      }
    }
}

void situacao()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String p = server.arg("p");
  String k = server.arg("k");
  int porta = p.toInt();

  //  if (porta == 1)
  //  {
  //    porta = 14;
  //  }
  porta = retornaPorta(porta);
  if (porta >= 0)
    //pinMode(porta, OUTPUT);

    if (k == "kdi9e")
    {
      if (LePorta(porta) == HIGH)
      {
        server.send(200, "text/html", "true");
      }
      else
      {
        server.send(200, "text/html", "false");
      }
    }
}

// void grava()
// {
//   //const char* www_username = www_username2.c_str();
//   //const char* www_password = www_password2.c_str();
//   if (!server.authenticate(www_username, www_password))
//     return server.requestAuthentication();

//   SPIFFS.begin();
//   File f = SPIFFS.open("/config.txt", "w");

//   if (!f)
//   {
//     SPIFFS.format();
//     File f = SPIFFS.open("/config.txt", "w");
//   }

//   f.println("IP: teste");

//   f.close();
//   SPIFFS.end();
// }

void valida()
{

  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //server.send(200, "text/html", "16");
  server.send(200, "text/html", "16|2|16|" + vchipId + "|");
}

// void ler()
// {
//   //const char* www_username = www_username2.c_str();
//   //const char* www_password = www_password2.c_str();
//   if (!server.authenticate(www_username, www_password))
//     return server.requestAuthentication();

//   SPIFFS.begin();
//   File f = SPIFFS.open("/config.txt", "r");
//   String texto = f.readStringUntil('\n');
//   server.send(200, "text/html", texto);
//   f.close();
//   SPIFFS.end();
// }

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  // digitalWrite(led, 0);

  //ESP.restart();
}

void LigaDesliga(int vPorta, int vFuncao, String Nome, int Tipo)
{
  if (Tipo != 1) //normal
  {
    // if (vFuncao == HIGH)
    // {
    //   vFuncao = LOW;
    // }
    // else
    // {
    //   vFuncao = HIGH;
    // }

    if (vPorta < 8)
    {
      chip1.write(vPorta, !vFuncao);
    }
    else
    {
      chip2.write(vPorta - 8, !vFuncao);
    }

    if (TipoMemoria)
    {
      memRtc.outValues = chip2.read8() << 8 | chip1.read8();
      memRtc.setOutputs();
    }
  }
  else //pulsado
  {
    executaPulso(vPorta);
  }
}

int LePorta(int vPorta)
{
  if (vPorta < 8)
  {
    if (chip1.read(vPorta) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
  else
  {
    if (chip2.read(vPorta - 8) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
}

int LeSensor(int vPorta)
{
  if (vPorta < 8)
  {
    if (sensor1.read(vPorta) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
  else
  {
    if (sensor2.read(vPorta - 8) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
}

void ApagaPortas()
{
  for (int i = 1; i <= 16; i++)
  {
    if (i <= 8)
    {
      chip1.write(retornaPorta(i), HIGH);
      chip2.write(retornaPorta(i), HIGH);
    }
    else
    {
      chip2.write(retornaPorta(i) - 8, HIGH);
    }
  }

  for (int i = 0; i < 8; i++)
  {
    chip3.write(i, LOW);
  }

  chip3.write(4, HIGH);
  chip3.write(5, HIGH);
  chip3.write(6, HIGH);
  chip3.write(7, HIGH);
}

void Inverte(int vPorta)
{
  if (vPorta < 8)
  {
    chip1.write(vPorta, !chip1.read(vPorta));
  }
  else
  {
    chip2.write(vPorta - 8, !chip2.read(vPorta - 8));
  }

  if (TipoMemoria)
  {
    memRtc.outValues = chip2.read8() << 8 | chip1.read8();
    memRtc.setOutputs();
    //Rtc.chip1 = String(chip1.read8()).toInt();
    //Rtc.chip2 = String(chip2.read8()).toInt();
    //Rtc.set_chip1();
    //Rtc.set_chip2();
  }
}
/*
// Return RSSI or 0 if target SSID not found
int32_t getRSSI(const char* target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    if (strcmp(WiFi.SSID(network), target_ssid) == 0) {
      return WiFi.RSSI(network);
    }
  }
  return 0;
}
*/

void CarregaEntradas()
{
  String sSensor1 = String(sensor1.read8(), BIN);
  String sSensor2 = String(sensor2.read8(), BIN);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  int posicaoSensor;

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    Sensores[posicaoSensor] = sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor);
    if (sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor) == "0")
    {
      estadoAtual[posicaoSensor] = HIGH;
      ultimoEstado[posicaoSensor] = HIGH;
    }
    else
    {
      estadoAtual[posicaoSensor] = LOW;
      ultimoEstado[posicaoSensor] = LOW;
    }
  }

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    Sensores[posicaoSensor + 8] = sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor);

    if (sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor) == "0")
    {
      estadoAtual[posicaoSensor + 8] = HIGH;
      ultimoEstado[posicaoSensor + 8] = HIGH;
    }
    else
    {
      estadoAtual[posicaoSensor + 8] = LOW;
      ultimoEstado[posicaoSensor + 8] = LOW;
    }
  }
}
