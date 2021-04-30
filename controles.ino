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

void retornachip(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", vchipId);
}

void RetornaChipMac(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", gchipId);
}

void controle(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", "ok");

  String p = request->arg("p");
  String k = request->arg("k");
  String f = request->arg("f");
  String Nome = request->arg("n");
  int porta = p.toInt();
  int Tipoa = request->arg("pu").toInt();

  porta = retornaPorta(porta);
  if (porta >= 0)

    if (k == "kdi9e")
    {

      if (f == "true")
      {
        LigaDesliga(porta, HIGH, Nome, Tipoa);
      }
      else
      {
        LigaDesliga(porta, LOW, Nome, Tipoa);
      }
    }
}

void situacao(AsyncWebServerRequest *request)
{
  String p = request->arg("p");
  String k = request->arg("k");
  int porta = p.toInt();

  porta = retornaPorta(porta);
  if (porta >= 0)

    if (k == "kdi9e")
    {
      if (LePorta(porta) == HIGH)
      {
        request->send(200, "text/html", "true");
      }
      else
      {
        request->send(200, "text/html", "false");
      }
    }
}

void valida(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", "16|2|16|" + vchipId + "|");
}

void handleNotFound(AsyncWebServerRequest *request)
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";
  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }
  request->send(404, "text/plain", message);
}

void LigaDesliga(int vPorta, int vFuncao, String Nome, int Tipo)
{
  if (Tipo != 1) //normal
  {
    if (vPorta < 8)
    {
      chip1.write(vPorta, !vFuncao);
    }
    else
    {
      chip2.write(vPorta - 8, !vFuncao);
    }
    SaveOutputs();
  }
  else //pulsado
  {
    executaPulso(vPorta);
  }

  sendCloud(true);
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
  chip1.write8(255);
  chip2.write8(255);
  chip3.write8(240);
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
  SaveOutputs();
}

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
