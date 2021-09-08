void configRF()
{
  mySwitch.enableReceive(rxRF);
  sSendRF.enableTransmit(txRF);
  sSendRF.setRepeatTransmit(5);
}

void getRF()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  gRequest->send(200, "text/html", String(tamanhoRF) + "|" + codigoRF + "|" + String(gProtocoloRF) + "*");
  tamanhoRF = -1;
  gProtocoloRF = -1;
  codigoRF = "-1";
}

void habRF()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  //enReadRF = true;
  gRequest->send(200, "text/html", "ok");
}

void gravarf()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();
  gRequest->send(200, "text/html", "ok");

  SensorRFAlterado = true;
  //String idAgenda = gRequest->arg("ag");
  String Valor = gRequest->arg("s");
  String Senha = gRequest->arg("k");

  (!DEBUG_ON) ?: Serial.println(Valor);
  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/rf.txt", "w");

    // if (!f)
    // {
    //   SPIFFS.format();
    //   File f = SPIFFS.open("/rf.txt", "w");
    // }

    f.println(Valor);
    f.close();
    SPIFFS.end();
  }
  consultaSensorRF();
}

void trataRF()
{
  static unsigned long limparUltimoDisparoRF = 0;
  unsigned long currenTime = millisAtual;
  //Contador += 1;
  if (currenTime - starTime >= interval)
  {
    limparUltimoDisparoRF++;

    if (limparUltimoDisparoRF > 1200)
    {
      limparUltimoDisparoRF = 0;
      ultimoDisparoRF = "";
    }
  }

  if (verificaSensoresRF())
  {
    if (msgDisparadaRF[numSensorMSG] == false)
    {
      //(!DEBUG_ON) ?:   Serial.print("Notificar? ");
      //(!DEBUG_ON) ?:   Serial.println(notificar);
      if (notificar)
      {
        sendDataToFirebase("Sensor " + String(numSensorMSG + 1) + " disparado", numSensorMSG, "2");
      }
      else if (enviarsms)
      {
        sendSMS(numSensorMSG);
      }
      ultimoDisparo = numSensorMSG;
      limparUltimoDisparoRF = 0;
    }
  }
}

boolean verificaSensoresRF()
{
  if (SensorRFAlterado)
  {
    (!DEBUG_ON) ?: Serial.println(" sensor alterado ");
    SensorRFAlterado = false;
    consultaSensorRF();
  }
  sensorRFDisparado = false;

  String Funcao;
  String PortaAparelho;
  String Msg;
  String vChip;
  String vIPDest;
  String vCodigo;
  String vTamanho;
  int i3;
  String texto = "";
  boolean valorRetorno = false;

  for (int nsensor = 0; nsensor < 30; nsensor++)
  {
    texto = SensoresRF[nsensor];
    texto.trim();
    //separa dados nas variaveis
    Funcao = "";
    PortaAparelho = "";
    Msg = "";
    vChip = "";
    vIPDest = "";
    vCodigo = "";
    vTamanho = "";
    i3 = 1;
    for (int i2 = 0; i2 <= 60; i2++)
    {
      if (texto[i2] != '|' && i3 <= 7 && texto != "")
      {
        if (i3 == 1)
        {
          Funcao += texto[i2];
        }

        if (i3 == 2)
        {
          PortaAparelho += texto[i2];
        }

        if (i3 == 3)
        {
          Msg += texto[i2];
        }

        if (i3 == 4) //ip
        {
          vIPDest += texto[i2];
        }

        if (i3 == 5) // ChipID
        {
          vChip += texto[i2];
        }

        if (i3 == 6) // Codigo
        {
          vCodigo += texto[i2];
        }

        if (i3 == 7) // Tamanho
        {
          vTamanho += texto[i2];
        }
      }
      else
      {
        i3++;
      }

    } // fim separar dados
    vCodigo.trim();
    if (vCodigo == codigoRF)
    {
      sensorRFDisparado = true;
      numSensorDisparado = nsensor;
    }
  }

  if (sensorRFDisparado)
  {
    // pega somente o disparo
    numSensorMSG = numSensorDisparado;

    texto = SensoresRF[numSensorDisparado];
    texto.trim();
    Funcao = "";
    PortaAparelho = "";
    Msg = "";
    vChip = "";
    vIPDest = "";
    vCodigo = "";
    vTamanho = "";
    i3 = 1;

    //separa dados nas variaveis
    for (int i2 = 0; i2 <= 60; i2++)
    {
      if (texto[i2] != '|' && i3 <= 7 && texto != "")
      {
        if (i3 == 1)
        {
          Funcao += texto[i2];
        }

        if (i3 == 2)
        {
          PortaAparelho += texto[i2];
        }

        if (i3 == 3)
        {
          Msg += texto[i2];
        }

        if (i3 == 4) //ip
        {
          vIPDest += texto[i2];
        }

        if (i3 == 5) // ChipID
        {
          vChip += texto[i2];
        }

        if (i3 == 6) // Codigo
        {
          vCodigo += texto[i2];
        }

        if (i3 == 7) // Tamanho
        {
          vTamanho += texto[i2];
        }
      }
      else
      {
        i3++;
      }

    } // fim separar dados
  }

  if (sensorRFDisparado) // se houve correspondencia
  {
    sensorRFDisparado = false;
    enviarsms = false;
    // iniciar tratamento.
    String ValorAtuacaoRF;
    String vsAtualRF = "1";
    if (Funcao == "0" || Funcao == "2" || Funcao == "3" || Funcao == "7")
    {
      ValorAtuacaoRF = "1";
    }
    else if (Funcao == "1")
    {
      ValorAtuacaoRF = "0";
    }
    else if (Funcao == "4")
    {
      enviarsms = true;
      ValorAtuacaoRF = "1";
      msgDisparadaRF[numSensorDisparado] = false;
    }
    else if (Funcao == "5")
    {
      ValorAtuacaoRF = "1";
    }
    else if (Funcao == "6")
    {
      ValorAtuacaoRF = vsAtualRF;
    }
    else if (Funcao == "8")
    {
      ValorAtuacaoRF = "1";
    }
    else
    {
      ValorAtuacaoRF = "1";
    }

    //varia com o status atual da prota
    if (vsAtualRF == "0")
    {
      estadoAtualRF[numSensorDisparado] = LOW;
    }
    else
    {
      estadoAtualRF[numSensorDisparado] = HIGH;
    }

    ////
    //    (!DEBUG_ON) ?:   Serial.print("Valor Atuacao: ");
    //  (!DEBUG_ON) ?:   Serial.println(ValorAtuacaoRF);
    //    (!DEBUG_ON) ?:   Serial.print("Valor vsAtualRF: ");
    //(!DEBUG_ON) ?:   Serial.println(vsAtualRF);
    //(!DEBUG_ON) ?:   Serial.print("Valor estadoAtualRF: ");
    //(!DEBUG_ON) ?:   Serial.println(estadoAtualRF[nsensor]);
    //(!DEBUG_ON) ?:   Serial.print("Valor ultimoestadorf: ");
    //(!DEBUG_ON) ?:   Serial.println(ultimoEstadoRF[nsensor]);

    ///  Reseta os marcadores de acionamento

    if (estadoAtualRF[numSensorDisparado] == ultimoEstadoRF[numSensorDisparado] && ValorAtuacaoRF != vsAtualRF)
    {
      if (Funcao == "7")
      {
        ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
      }
    }

    if (ValorAtuacaoRF == vsAtualRF && estadoAtualRF[numSensorDisparado] != ultimoEstadoRF[numSensorDisparado])
    {
      if (Funcao == "0" || Funcao == "1" || Funcao == "4")
      {
        //(!DEBUG_ON) ?:   Serial.println("disparou");
        ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
        msgDisparadaRF[numSensorDisparado] = false;
        valorRetorno = true;
      }
      else if (Funcao == "2") // liga porta
      {
        IPAddress Destino;
        Destino.fromString(vIPDest);

        if (Destino == IpDispositivo)
        {
          LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 0);
        }
        else // envia udp
        {
          char replyPacekt[255] = "";
          String Texto = vIPDest + "|" + PortaAparelho + "|true|" + String(vChip) + "|E|";
          (!DEBUG_ON) ?: Serial.println(Texto);
          Texto.toCharArray(replyPacekt, 255);
          (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
          SendUDP(Destino, localUdpPort, String(replyPacekt));
          // Udp.beginPacket(Destino, localUdpPort);
          // Udp.write(replyPacekt);
          // Udp.endPacket();
          msgDisparadaRF[numSensorDisparado] = true;
        }
        if (Msg == "1")
        {
          valorRetorno = true;
          msgDisparadaRF[numSensorDisparado] = false;
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

        if (Destino == IpDispositivo)
        {
          LigaDesliga(PortaAparelho.toInt() - 1, LOW, "", 0);
        }
        else
        {
          char replyPacekt[255] = "";
          String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|";
          Texto.toCharArray(replyPacekt, 255);
          (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
          SendUDP(Destino, localUdpPort, String(replyPacekt));
          // Udp.beginPacket(Destino, localUdpPort);
          // Udp.write(replyPacekt);
          // Udp.endPacket();
          msgDisparadaRF[numSensorDisparado] = true;
        }
        if (Msg == "1")
        {
          valorRetorno = true;
          msgDisparadaRF[numSensorDisparado] = false;
        }
        else
        {
          valorRetorno = false;
        }
      }
      else if (Funcao == "5")
      {

        if (estadoAtualRF[numSensorDisparado] == HIGH && ultimoEstadoRF[numSensorDisparado] == LOW)
        {
          //            Inverte(PortaAparelho.toInt()-1);
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
              //  delay(300);
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              (!DEBUG_ON) ?: Serial.println(Texto);
              Texto.toCharArray(replyPacekt, 255);
              (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
              SendUDP(Destino, localUdpPort, String(replyPacekt));
              // Udp.beginPacket(Destino, localUdpPort);
              // Udp.write(replyPacekt);
              // Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        valorRetorno = false;
      }
      else if (Funcao == "6")
      {
        if (estadoAtualRF[numSensorDisparado] == HIGH && ultimoEstadoRF[numSensorDisparado] == LOW)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
              //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              Texto.toCharArray(replyPacekt, 255);
              (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
              SendUDP(Destino, localUdpPort, String(replyPacekt));
              // Udp.beginPacket(Destino, localUdpPort);
              // Udp.write(replyPacekt);
              // Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        else if (estadoAtualRF[numSensorDisparado] == LOW && ultimoEstadoRF[numSensorDisparado] == HIGH)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              //            LigaDesliga(PortaAparelho.toInt()-1, LOW, "", 0);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              Texto.toCharArray(replyPacekt, 255);
              (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
              SendUDP(Destino, localUdpPort, String(replyPacekt));
              // Udp.beginPacket(Destino, localUdpPort);
              // Udp.write(replyPacekt);
              // Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        valorRetorno = false;
      }
      else if (Funcao == "7")
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay * 3)
        {
          lastDebounceTime = millisAtual;
          IPAddress Destino;
          Destino.fromString(vIPDest);

          if (Destino == IpDispositivo && ultimoEstadoRF[numSensorDisparado] != estadoAtualRF[numSensorDisparado])
          {
            LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 1);
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
          else if (Destino != IpDispositivo && ultimoEstadoRF[numSensorDisparado] != estadoAtualRF[numSensorDisparado])
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
            Texto.toCharArray(replyPacekt, 255);
            (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
            SendUDP(Destino, localUdpPort, String(replyPacekt));
            // Udp.beginPacket(Destino, localUdpPort);
            // Udp.write(replyPacekt);
            // Udp.endPacket();
            msgDisparadaRF[numSensorDisparado] = true;
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
        }
      }
      else if (Funcao == "8")
      {
        if (estadoAtualRF[numSensorDisparado] == HIGH && ultimoEstadoRF[numSensorDisparado] == LOW)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            // Cena
            triggerCena(PortaAparelho);
            lastDebounceTime = millisAtual;
            msgDisparadaRF[numSensorDisparado] = true;
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
        }
        valorRetorno = false;
      }
      else
      {
        if (msgDisparadaRF[numSensorDisparado] == true)
        {
          msgDisparadaRF[numSensorDisparado] = false;
        }
        valorRetorno = false;
      }
    }
    numSensorDisparado = -1;
  }
  ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
  return valorRetorno;
}

void consultaSensorRF()
{
  String texto = "";
  SPIFFS.begin();
  File f = SPIFFS.open("/rf.txt", "r");
  if (f)
    texto = f.readStringUntil('*');
  texto += '*';
  (!DEBUG_ON) ?: Serial.println(texto);
  f.close();
  SPIFFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoSensor = "";

  if (texto.length() > 4)
  {
    SensoresRF[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoSensor += texto[posicao];
      }
      else if (texto[posicao] == '|' && contador < 7)
      {
        contador++;
        textoSensor += texto[posicao];
      }

      if (contador == 7)
      {
        if (i <= 29)
        {
          SensoresRF[i] = textoSensor;
          textoSensor = "";
          contador = 0;
        }
        i++;
      }
      posicao++;
    }
  }
}

void ultimodisprf()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {
    gRequest->send(200, "text/html", ultimoDisparoRF);
  }
}

void sendRFp()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();
  gRequest->send(200, "text/html", "ok");
  unsigned long Valor = strtoul(gRequest->arg("c").c_str(), NULL, 10);
  unsigned long _tamanhorf = strtoul(gRequest->arg("t").c_str(), NULL, 10);
  unsigned long _protocol = strtoul(gRequest->arg("p").c_str(), NULL, 10);
  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {

    (!DEBUG_ON) ?: Serial.println("Valor: " + String(Valor));
    (!DEBUG_ON) ?: Serial.println("Tamanho: " + String(_tamanhorf));
    (!DEBUG_ON) ?: Serial.println("Protocolo: " + String(_protocol));
    sSendRF.setProtocol(_protocol);
    sSendRF.send(Valor, _tamanhorf);
  }
}

void LoopRF()
{
  if (mySwitch.available())

  {

    //(!DEBUG_ON) ?: Serial.println("sinal de radio detectado");
    //      output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    if (millisAtual - rfmilis >= 10000 || millisAtual - rfmilis < 0)
    {
      rfmilis = millisAtual;
      for (int i = 0; i < 30; i++)
      {
        ultimoEstadoRF[i] = LOW;
      }
    }
    int value = mySwitch.getReceivedValue();

    if (value == 0)
    {
      (!DEBUG_ON) ?: Serial.print("Unknown encoding");
    }
    else
    {
      //    tone(Buzzer, 4000, 800);
      (!DEBUG_ON) ?: Serial.print("Received ");
      codigoRF = mySwitch.getReceivedValue();
      ultimoDisparoRF = codigoRF;
      (!DEBUG_ON) ?: Serial.print(mySwitch.getReceivedValue());
      (!DEBUG_ON) ?: Serial.print(" / ");
      (!DEBUG_ON) ?: Serial.print(mySwitch.getReceivedBitlength());
      tamanhoRF = mySwitch.getReceivedBitlength();
      (!DEBUG_ON) ?: Serial.print("bit ");
      (!DEBUG_ON) ?: Serial.print("Protocol: ");
      (!DEBUG_ON) ?: Serial.println(mySwitch.getReceivedProtocol());
      gProtocoloRF = mySwitch.getReceivedProtocol();

      trataRF();
    }
    mySwitch.resetAvailable();

    //delay(300);
    //    noTone(Buzzer);
    //  enReadRF = false;
  }
}