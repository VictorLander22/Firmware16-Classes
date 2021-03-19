void trataDevice(String (&Devices)[20])
{
  SPIFFS.begin();
  File f = SPIFFS.open("/device.txt", "r");

  String valorDevice = f.readStringUntil('*');
  //(!DEBUG_ON) ?:   Serial.println(valorDevice);
  f.close();
  SPIFFS.end();

  int posicaoDevice = 0;

  for (int i = 0; i < 20; i++)
  {
    Devices[i] = "";
  }

  for (int i = 0; i <= valorDevice.length(); i++)
  {
    if (valorDevice[i] != ';')
    {
      if (valorDevice[i] != '*' && int(valorDevice[i]) != 13 && int(valorDevice[i]) != 0)
      {
        Devices[posicaoDevice] += valorDevice[i];
      }
    }
    else
    {
      if (posicaoDevice >= 20)
      {
        posicaoDevice = 20;
      }
      else
      {
        posicaoDevice += 1;
      }
    }
  }
  (!DEBUG_ON) ?: Serial.println(valorDevice);
  DeviceAlterado = false;
}

void trataSensores()
{
  static unsigned long limparUltimoDisparo = 0;

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

  if (DeviceAlterado)
  {
    //trataDevice();
  }

  String sChip1 = String(chip1.read8(), BIN);
  String sChip2 = String(chip2.read8(), BIN);

  while (sChip1.length() < 8)
  {
    sChip1 = '0' + sChip1;
  }

  while (sChip2.length() < 8)
  {
    sChip2 = '0' + sChip2;
  }

  // separa os valores ligos no array
  String valorSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
  int posicaoSensor;

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    valorSensores[posicaoSensor] = sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor);
  }

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    valorSensores[posicaoSensor + 8] = sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor);
  }
  // fim separação dos valroes lidos

  //sensor1.write(7, LOW);
  //(!DEBUG_ON) ?:   Serial.println(sSensor1);
  //(!DEBUG_ON) ?:   Serial.println(String(sensor1.read(7)));

  unsigned long currenTime = millisAtual;
  //Contador += 1;
  if (currenTime - starTime >= interval)
  {
    limparUltimoDisparo++;

    if (limparUltimoDisparo > 1200)
    {
      limparUltimoDisparo = 0;
      ultimoDisparo = "";
    }
    int32_t rssi;
    rssi = WiFi.RSSI();
    char replyPacekt[255] = "";
    String valSensores = sSensor1 + sSensor2;
    valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + valSensores + "|" + sChip1 + sChip2 + "|" + rssi + "*";
    valSensores.toCharArray(replyPacekt, 255);
    //(!DEBUG_ON) ?:   Serial.println(valSensores);
    IPAddress broadcastIp = IpDispositivo;
    broadcastIp[0] = 255;
    broadcastIp[1] = 255;
    broadcastIp[2] = 255;
    broadcastIp[3] = 255;

    if (IpDispositivo[0] == 192 && IpDispositivo[1] == 168 && IpDispositivo[2] == 4)
    {
      broadcastIp[0] = 192;
      broadcastIp[1] = 168;
      broadcastIp[2] = 4;
      broadcastIp[3] = 255;
    }

    if (subnet[0] == 255 && subnet[1] == 255 && subnet[2] == 255)
    {
      broadcastIp[0] = IpDispositivo[0];
      broadcastIp[1] = IpDispositivo[1];
      broadcastIp[2] = IpDispositivo[2];
      broadcastIp[3] = 255;
    }

    if (subnet[0] == 255 && subnet[1] == 255 && subnet[2] == 0)
    {
      broadcastIp[0] = IpDispositivo[0];
      broadcastIp[1] = IpDispositivo[1];
      broadcastIp[2] = 255;
      broadcastIp[3] = 255;
    }

    //(!DEBUG_ON) ?:   Serial.println(Contador);
    //    Contador = 0;
    Udp.beginPacketMulticast(broadcastIp, localUdpPort, IpDispositivo);
    //Udp.beginPacket(broadcastIp, localUdpPort);
    Udp.write(replyPacekt);
    Udp.endPacket();
    starTime = millisAtual;
  }
  else if ((currenTime - starTime) < 0)
  {
    starTime = millisAtual;
  }

  for (int numSensor = 0; numSensor <= 15; numSensor++)
  {

    //  if (SensorAlterado && (sSensor1 != "11111111" || sSensor2 != "11111111"))
    if (verificaSensores(numSensor, valorSensores[numSensor]))
    {
      if (msgDisparada[numSensor] == false)
      {
        if (notificar)
        {
          if (nomeSensores[numSensor + 1] != "")
          {
            sendDataToFirebase("Sensor " + nomeSensores[numSensor + 1] + " disparado", numSensor, "1");
          }
          else
          {
            sendDataToFirebase("Sensor " + String(numSensor + 1) + " disparado", numSensor, "1");
          }
        }
        else if (enviarsms)
        {
          sendSMS(numSensor);
        }
        ultimoDisparo = sSensor1 + sSensor2;
        limparUltimoDisparo = 0;
        (!DEBUG_ON) ?: Serial.println("Sensor disparado");
      }
    }
  }
}

void sendDataToFirebase(String MSG, int numSen, String vTag)
{
  WiFiClient cliente;
  String Devices[20];
  trataDevice(Devices);
  String serve = "AAAAKmhc-Lk:APA91bFrNKIu9Ufqj3AmtXBPcd-87smD9kO4Z6CNdBOejY9nGFjvMCBCP2-7eTVqXlqmJpL2o4cTXD6eD4gbkvnpihPFxougju-536CCPZZn62DNTGhgtUFHoBPw0yyq4xmbjb7Bm9IF";

  String reg = "";

  for (int i = 0; i <= 9; i++)
  {
    //    reg = "APA91bH1RwO17t1VXFfQ4HkQTsvIncLp-SjhGH3WTDtO3_fTDJiOdk43wjNR2-IaOPcEOJvfm-Gp5iRMdk8c-Sy-GNyspsUxK4JS8ZuPo6Nhe9tR9smyrxvSRWBU216mVgTN6UjzdjEj";
    //    reg = "fshYIY1u_GY:APA91bFXybZ0qcmldA8uIqltpGz8nHImMYV6B2I_PqsCmQiE6WKQFSXJhq0zo3vlIi-h-JZLt5i6HyZbJE9qFGJvA_Qg2ioDdQqfNnIY8Emun_LzEUCO48xXTKs9GcyU9UuaaASkEDOX";
    reg = String(Devices[i]);
    //reg.trim();

    if (reg.length() > 10)
    {
      //(!DEBUG_ON) ?:   Serial.println(reg.substring(0,1));
      //(!DEBUG_ON) ?:   Serial.println(reg.substring(1));
      String data = "";
      if (reg.substring(0, 1) == "2")
      {
        (!DEBUG_ON) ?: Serial.println("Iphone");
        data = "{";
        data = data + "\"to\": \"" + reg.substring(1) + "\",";
        data = data + "\"notification\": {";
        //data = data + "\"custom_notification\": {";
        data = data + "\"body\": \"" + MSG + "\",";
        data = data + "\"title\" : \"" + MSG + "\",";
        data = data + "\"tag\":\"" + vTag + "\",";
        data = data + "\"badge\":\"0\",";
        data = data + "\"content-available\": \"1\",";
        data = data + "\"sound\":\"default\"";
        data = data + "},";
        data = data + "\"data\": {";
        data = data + "\"app\": \"keepin\",";
        data = data + "\"msg\": \"" + MSG + "\"";
        data = data + "}";
        data = data + " }";
      }
      else
      {
        data = "{";
        data = data + "\"to\": \"" + reg.substring(1) + "\",";
        data = data + "\"notification\": {";
        //data = data + "\"custom_notification\": {";
        data = data + "\"body\": \"" + MSG + "\",";
        data = data + "\"title\" : \"" + MSG + "\",";
        data = data + "\"tag\":\"" + vTag + "\",";
        data = data + "\"badge\":\"0\",";
        data = data + "\"content-available\": \"1\",";
        data = data + "\"sound\":\"default\"";
        data = data + "},";
        data = data + "\"data\": {";
        data = data + "\"app\": \"keepin\",";
        data = data + "\"msg\": \"" + MSG + "\"";
        data = data + "}";
        data = data + " }";
      }
      (!DEBUG_ON) ?: Serial.println("Send data...");
      //      if (cliente.connect("fcm.googleapis.com", 80)) {
      if (cliente.connect("cloud.fcleal.com.br", 80))
      {
        (!DEBUG_ON) ?: Serial.println("Connected to the server..");
        //        cliente.println("POST /fcm/send HTTP/1.1");
        cliente.println("POST /api/keepin/fcm HTTP/1.1");
        cliente.println("Authorization: key=" + serve + "");
        cliente.println("Content-Type: application/json");
        //        cliente.println("Host: fcm.googleapis.com");
        cliente.println("Host: cloud.fcleal.com.br");
        cliente.print("Content-Length: ");
        cliente.println(data.length());
        cliente.print("\n");
        cliente.print(data);
      }
      (!DEBUG_ON) ?: Serial.println("Data sent...Reading response..");
      while (cliente.available())
      {
        char c = cliente.read();
        (!DEBUG_ON) ?: Serial.print(c);
      }
      (!DEBUG_ON) ?: Serial.println("Finished!");
      cliente.flush();
      (!DEBUG_ON) ?: Serial.println(data);
      msgDisparada[numSen] = true;
      msgDisparadaRF[numSen] = true;
    }
  }
  cliente.stop();
}

void lersensores(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

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

  request->send(200, "text/html", sSensor1);
}

void gravasensor(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", "ok");

  String idAgenda = request->arg("ag");
  String Valor = request->arg("s");
  String Senha = request->arg("k");
  String nomeS = request->arg("nome");
  String gv = request->arg("gn");
  int Indice = request->arg("p").toInt();
  String nomesG = "";

  if (gv == "1")
  {
    nomeSensores[Indice] = nomeS;
    Serial.println(nomeS);
    Serial.println("Estou aqui 1");
  }

  if (Senha == "kdi9e")
  {

    for (int id = 0; id < 16; id++)
    {
      nomesG += nomeSensores[id] + "|";
    }
    nomesG += "*";

    SPIFFS.begin();
    File f = SPIFFS.open("/sensores.txt", "w");

    // if (!f)
    // {
    //   SPIFFS.format();
    //   File f = SPIFFS.open("/sensores.txt", "w");
    // }

    f.println(Valor);
    f.close();

    f = SPIFFS.open("/nsensores.txt", "w");
    f.println(nomesG);
    f.close();

    SPIFFS.end();
    //(!DEBUG_ON) ?: Serial.println("valor salvo na ag" + idAgenda + ".txt");
    (!DEBUG_ON) ?: Serial.println(Valor);
  }

  //Serial.println("Vou consultar sensor");
  //consultaSensor();
  //Serial.println("Sai de consulta sensor");
  SensorAlterado = true;
}

void gravasensor2(String Valor)
{
  SensorAlterado = true;

  SPIFFS.begin();
  File f = SPIFFS.open("/sensores.txt", "w");
  f.println(Valor);
  f.close();
  SPIFFS.end();
}

boolean verificaSensores(int nsensor, String vsAtual)
{
  if (SensorAlterado)
  {
    SensorAlterado = false;
    consultaSensor();
  }

  //String texto = consultaAgenda(i);
  String texto = Sensores[nsensor];
  texto.trim();

  String Porta = "";
  String Funcao = "";
  String PortaAparelho = "";
  String Msg = "";
  String vChip = "";
  String vIPDest = "";
  String PortaAparelho2 = "";
  String vChip2 = "";
  String vIPDest2 = "";
  String vTipo1 = "";
  String vTipo2 = "";
  int i3 = 1;
  boolean valorRetorno = false;
  enviarsms = false;
  static unsigned long inicioAcionado[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static boolean vDisparadoSensor[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

  //separa dados nas variaveis
  for (int i2 = 0; i2 <= 100; i2++)
  {
    if (texto[i2] != '|' && i3 <= 11 && texto != "")
    {
      if (i3 == 1)
      {
        Porta += texto[i2];
      }

      if (i3 == 2)
      {
        Funcao += texto[i2];
      }

      if (i3 == 3)
      {
        PortaAparelho += texto[i2];
      }

      if (i3 == 4)
      {
        Msg += texto[i2];
      }

      if (i3 == 5) //ip
      {
        vIPDest += texto[i2];
      }

      if (i3 == 6) // ChipID
      {
        vChip += texto[i2];
      }

      if (i3 == 7) // Porta Aparelho 2
      {
        PortaAparelho2 += texto[i2];
      }

      if (i3 == 8) // IP Destino 2
      {
        vIPDest2 += texto[i2];
      }

      if (i3 == 9) // ChipID 2
      {
        vChip2 += texto[i2];
      }

      if (i3 == 10) // vTipo1 - Se é retenção ou pulso
      {
        vTipo1 += texto[i2];
      }

      if (i3 == 11) // vTipo2 - Se é retenção ou pulso
      {
        vTipo2 += texto[i2];
      }
    }
    else
    {
      i3++;
    }

  } // fim separar dados

  // iniciar tratamento.
  String ValorAtuacao;
  if (Funcao == "0" || Funcao == "2" || Funcao == "3" || Funcao == "8")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "1")
  {
    ValorAtuacao = "1";
  }
  else if (Funcao == "4")
  {
    enviarsms = true;
    ValorAtuacao = "0";
  }
  else if (Funcao == "5")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "6" || Funcao == "9" || Funcao == "10")
  {
    ValorAtuacao = vsAtual;
  }
  else if (Funcao == "7")
  {
    ValorAtuacao = "0";
  }

  //varial com o status atual da prota

  if (vsAtual == "0")
  {
    estadoAtual[nsensor] = HIGH;
  }
  else
  {
    estadoAtual[nsensor] = LOW;
  }

  if (nsensor == 3)
  {
    //(!DEBUG_ON) ?:   Serial.println(Funcao);
    //(!DEBUG_ON) ?:   Serial.print("estadoAtual: ");
    //(!DEBUG_ON) ?:   Serial.println(estadoAtual[nsensor]);
    //       ultimoEstado[nsensor] = estadoAtual[nsensor];
    //(!DEBUG_ON) ?:   Serial.print("ultimoEstado: ");
    //      (!DEBUG_ON) ?:   Serial.println(ultimoEstado[nsensor]);
  }

  ////
  ///  Reseta os marcadores de acionamento
  if (estadoAtual[nsensor] == ultimoEstado[nsensor] && ValorAtuacao != vsAtual)
  {
    if (Funcao == "7")
    {
      ultimoEstado[nsensor] = estadoAtual[nsensor];
    }
  }

  if (Funcao == "8" && ValorAtuacao != vsAtual) // trata dupla funcao
  {
    if (inicioAcionado[nsensor] > 0)
    {
      if (millisAtual < inicioAcionado[nsensor])
      {
        inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
      }

      if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
      {
        (!DEBUG_ON) ?: Serial.println("maior que 2 segundos"); /// acionamento 2
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest2);

        IPAddress Comparacao(192, 168, 4, 1);
        if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
        {
          lastDebounceTime = millisAtual;
          if (vTipo2 == "2") // cenas
          {
            triggerCena(PortaAparelho2);
          }
          else if (vTipo2 == "1")
          {
            LigaDesliga(PortaAparelho2.toInt() - 1, HIGH, "", 1);
          }
          else
          {
            Inverte(PortaAparelho2.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          //udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo2 == "1")
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
          }
          else
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparada[nsensor] = true;
        }
      }
      else if (vDisparadoSensor[nsensor] == false)
      {
        (!DEBUG_ON) ?: Serial.println("Menor que 2 segundos");
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest);
        IPAddress Comparacao(192, 168, 4, 1);

        if (Destino == IpDispositivo || IpDispositivo == Comparacao)
        {
          lastDebounceTime = millisAtual;
          if (vTipo1 == "1")
          {
            LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 1);
          }
          else
          {
            Inverte(PortaAparelho.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          //udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo1 == "1")
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
          }
          else
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparada[nsensor] = true;
        }
      }
    }
  }
  else if (Funcao == "8" && ValorAtuacao == vsAtual) // caso tenha ficado mais de 2 segundos ligado já executa.
  {
    if (millisAtual < inicioAcionado[nsensor])
    {
      inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
    }

    if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
    {
      (!DEBUG_ON) ?: Serial.println("maior que 2 segundos");
      inicioAcionado[nsensor] = 0;
      vDisparadoSensor[nsensor] = true;

      IPAddress Destino;
      Destino.fromString(vIPDest2);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
      {
        lastDebounceTime = millisAtual;
        if (vTipo2 == "2") // cenas
        {
          triggerCena(PortaAparelho2);
        }
        else if (vTipo2 == "1")
        {
          LigaDesliga(PortaAparelho2.toInt() - 1, HIGH, "", 1);
        }
        else
        {
          Inverte(PortaAparelho2.toInt() - 1);
        }
        ultimoEstado[nsensor] = estadoAtual[nsensor];
        //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
      }
      else
      {
        //udp
        char replyPacekt[255] = "";
        String Texto = "";
        if (vTipo2 == "1")
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
        }
        else
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
        }
        Texto.toCharArray(replyPacekt, 255);
        (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
    }
  }

  if (Msg == "1")
  {
    valorRetorno = true;
  }

  if (ValorAtuacao == vsAtual && estadoAtual[nsensor] != ultimoEstado[nsensor])
  {
    if (Funcao == "0" || Funcao == "1" || Funcao == "4")
    {
      valorRetorno = true;
    }
    else if (Funcao == "2") // liga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 0);
      }
      else // envia udp
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|true|" + String(vChip) + "|E|0|";
        (!DEBUG_ON) ?: Serial.println(Texto);
        Texto.toCharArray(replyPacekt, 255);
        (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "3") // desliga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, LOW, "", 0);
      }
      else
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|0|";
        Texto.toCharArray(replyPacekt, 255);
        (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "5")
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            //  delay(300);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      //valorRetorno = false;
    }
    else if (Funcao == "6") // interruptor normal
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || Destino == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            (!DEBUG_ON) ?: Serial.print("enviado comando UDP: ");
            (!DEBUG_ON) ?: Serial.println(Texto);
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          Inverte(PortaAparelho.toInt() - 1);
          //LigaDesliga(PortaAparelho.toInt()-1, LOW, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "7")
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if ((Destino == IpDispositivo && ultimoEstado[nsensor] != estadoAtual[nsensor]) || (ultimoEstado[nsensor] != estadoAtual[nsensor] && IpDispositivo == Comparacao))
      {
        LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 1);
        ultimoEstado[nsensor] = estadoAtual[nsensor];
      }
      //      else if (Destino == IpDispositivo && ultimoEstado[Porta.toInt()] == estadoAtual[nsensor])
      //      {
      //        if (millisAtual - lastPulso > 10000 || millisAtual - lastPulso < 0)
      //        {
      //          ultimoEstado[Porta.toInt()] = !estadoAtual[nsensor];
      //          lastPulso = millisAtual;
      //        }
      //      }
      else if (Destino != IpDispositivo && ultimoEstado[Porta.toInt()] != estadoAtual[nsensor] && IpDispositivo != Comparacao)
      {
        //udp
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
        Texto.toCharArray(replyPacekt, 255);
        (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
        ultimoEstado[nsensor] = estadoAtual[nsensor];
      }
    }
    else if (Funcao == "8")
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {

          if (inicioAcionado[nsensor] == 0)
          {
            inicioAcionado[nsensor] = millisAtual;
            vDisparadoSensor[nsensor] = false;
          }
        }
      }

      //     valorRetorno = false;
    }
    else if (Funcao == "9") // interruptor invertido
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            //Inverte(PortaAparelho.toInt()-1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            LigaDesliga(PortaAparelho.toInt() - 1, LOW, "", 0);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          //Inverte(PortaAparelho.toInt()-1);
          LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "10") // Interruptor Cena
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena1
          triggerCena(PortaAparelho);
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena2
          triggerCena(PortaAparelho2);
        }
      }
      valorRetorno = false;
    }
  }
  else
  {
    if (msgDisparada[nsensor] == true)
    {
      msgDisparada[nsensor] = false;
    }
    valorRetorno = false;
  }
  ultimoEstado[nsensor] = estadoAtual[nsensor];
  return valorRetorno;
}

void consultaSensor()
{
  SPIFFS.begin();

  File f = SPIFFS.open("/sensores.txt", "r");
  String texto = f.readStringUntil('*');
  texto += '*';
  f.close();

  f = SPIFFS.open("/nsensores.txt", "r");
  String nomeS = f.readStringUntil('*');
  nomeS += '*';
  f.close();

  SPIFFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoSensor = "";
  if (texto.length() > 4)
  {
    Sensores[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoSensor += texto[posicao];
      }
      else if (texto[posicao] == '|' && contador < 11)
      {
        contador++;
        textoSensor += texto[posicao];
      }

      if (contador == 11)
      {
        Sensores[i] = textoSensor;
        textoSensor = "";
        contador = 0;
        i++;
      }
      posicao++;
    }
  }
  // nomes dos sensores
  posicao = 0;
  i = 0;
  String textoNome = "";

  if (nomeS.length() > 4)
  {
    nomeSensores[i] = "";
    while (nomeS[posicao] != '*')
    {
      if (nomeS[posicao] != '|')
      {
        textoNome += nomeS[posicao];
      }
      else if (nomeS[posicao] == '|')
      {
        nomeSensores[i] = textoNome;
        textoNome = "";
        i++;
      }
      posicao++;
    }
  }
}

String lerSensor()
{
  String texto;
  SPIFFS.begin();
  File f = SPIFFS.open("/sensores.txt", "r");
  if (f)
    texto = f.readStringUntil('*');
  f.close();
  SPIFFS.end();

  return texto;
}

void consensor(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    //String texto = lerSensor();
    request->send(200, "text/html", lerSensor());
  }
  else
  {
    request->send(200, "text/html", "-1");
  }
}

void gravadevice(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
  request->send(200, "text/html", "ok");
  String Valor = request->arg("d");
  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/device.txt", "w");

    // if (!f)
    // {
    //   SPIFFS.format();
    //   File f = SPIFFS.open("/device.txt", "w");
    // }

    f.println(Valor);

    f.close();
    SPIFFS.end();
    DeviceAlterado = true;
    (!DEBUG_ON) ?: Serial.println("gravado device");
  }
}

void buscadevice(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/device.txt", "r");

    String valorDevice = f.readStringUntil('*');

    f.close();
    SPIFFS.end();
    request->send(200, "text/html", valorDevice);
  }
}

/*
void limpadevice() 
{
    SPIFFS.begin();   
    //File f = SPIFFS.open("/device.txt", "w");
    SPIFFS.remove("/device.txt");  
    //if (!f) {
    //  SPIFFS.format();
    //  File f = SPIFFS.open("/device.txt", "w");
    //}

    //f.println("");    

    //f.close();
    SPIFFS.end();

    request->send(200, "text/html", "OK");    
}
*/

void ultimodisp(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    request->send(200, "text/html", ultimoDisparo);
  }
}

void retornaNotificar()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/notific.txt", "r");

  String valor = f.readStringUntil('*');

  if (valor == "true")
    notificar = true;
  else
    notificar = false;
  f.close();
  SPIFFS.end();
}

void buscaNotificar(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    request->send(200, "text/html", String(notificar));
  }
  else
  {
    request->send(200, "text/html", "0");
  }
}

void gravanot(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
  request->send(200, "text/html", "ok");
  String Valor = request->arg("v");
  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/notific.txt", "w");

    // if (!f)
    // {
    //   SPIFFS.format();
    //   File f = SPIFFS.open("/notific.txt", "w");
    // }

    f.println(Valor);

    f.close();
    SPIFFS.end();
    (!DEBUG_ON) ?: Serial.println("gravado notificacao");

    if (Valor == "true")
    {
      notificar = true;
    }
    else
    {
      notificar = false;
    }
  }
}

void gravanot2(String Valor)
{

  SPIFFS.begin();
  File f = SPIFFS.open("/notific.txt", "w");

  // if (!f)
  // {
  //   SPIFFS.format();
  //   File f = SPIFFS.open("/notific.txt", "w");
  // }

  f.println(Valor);

  f.close();
  SPIFFS.end();
  (!DEBUG_ON) ?: Serial.println("gravado notificacao");

  if (Valor == "true")
  {
    notificar = true;
  }
  else
  {
    notificar = false;
  }
}
