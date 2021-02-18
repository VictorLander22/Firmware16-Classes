void gravacena()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    String ia = server.arg("ia");
    String vTipo = server.arg("t");
    String Valor = server.arg("v");
    String IdCena = server.arg("id");
    Serial.println(Valor);
    SPIFFS.begin();

    if (ia == "1") // inicio de arquivo
    {
      File rFile = SPIFFS.open("/ce_" + IdCena + ".cfg", "w");
      rFile.println(Valor);
      rFile.close();
    }
    else
    {
      File rFile = SPIFFS.open("/ce_" + IdCena + ".cfg", "a");
      //String ValorAnterior = rFile.readString();
      //rFile.close();

      //rFile = SPIFFS.open("/ce_"+IdCena+".cfg","w");
      //rFile.print(ValorAnterior);
      rFile.println(Valor);
      rFile.close();
    }

    SPIFFS.end();
    //lerArquivo(IdCena);
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void lerArquivo(String id)
{
  SPIFFS.begin();

  File rFile = SPIFFS.open("/ce_" + id + ".cfg", "r");
  String linhas;
  //linhas = rFile.readString();

  //Serial.println("linhas das cenas");
  int qtde = 0;
  while (rFile.available())
  {
    String linhas = rFile.readStringUntil('\n');
    Serial.print(linhas);
    qtde++;
  }

  rFile.close();
  SPIFFS.end();
  //Serial.print("qtde de linhas: ");
  Serial.println(qtde);
}

void triggerCena(String arq)
{
  cenaExecucao = true;
  cenaPAtual = 0;
  cenaPTotal = 0;
  ArqCena = arq;
}

void checkCena()
{
  if (cenaExecucao == true)
  {
    String Comando;
    static File rFile;
    SPIFFS.begin();
    if (cenaPAtual == 0) // abre spiff e mantem aberto
    {
      rFile = SPIFFS.open("/ce_" + ArqCena + ".cfg", "r");

      while (rFile.available())
      {

        String linhas = rFile.readStringUntil('\n');
        if (cenaPTotal == 0)
        {
          Comando = linhas;
        }
        cenaPTotal++;
      }
      Serial.println("Total cena: " + String(cenaPTotal));
      cenaPAtual++;
      Serial.println("Cena atual: " + String(cenaPAtual));

      rFile.close();
      SPIFFS.end();
    }
    if (cenaPAtual >= 1)
    {
      //Serial.print("processando cena: ");
      //Serial.println(cenaPAtual);

      if (cenaPAtual == 1)
      {
        //Serial.print(Comando);
        //Serial.println("primeiro");
        //Serial.println("Cena atual: " + String(cenaPAtual));
        Serial.println("Executar cena: " + Comando);
        executaCena(Comando);
        //Serial.println("Exceutou cena primeiro");
      }
      else
      {
        Serial.println("Cena atual: " + String(cenaPAtual));
        //rFile.seek(0, SeekSet);

        SPIFFS.begin();
        rFile = SPIFFS.open("/ce_" + ArqCena + ".cfg", "r");
        int conCena = 1;

        while (rFile.available())
        {
          //Serial.println("\nInicio While");
          //Serial.println("conCena while: " + String(conCena));
          //Serial.println("Cena atual while: " + String(cenaPAtual));

          String linhas = rFile.readStringUntil('\n');

          //Serial.println("Codigo lido arquivo: " + linhas + "Posição no arquivo: " + rFile.position());

          if (conCena == cenaPAtual)
          {

            Comando = linhas;
            //Serial.println("Executar cena while: " + Comando);
            //Serial.println("Cena atual antes execução: " + String(cenaPAtual));
            Serial.println("Executar cena: " + Comando);
            executaCena(Comando);
            //Serial.println("Cena atual apos execução" + String(cenaPAtual));
            rFile.close();
            SPIFFS.end();
            //conCena = 0;
          }
          conCena++;

          //Serial.println("ConCena fim while: " + String(conCena));
        }
        //Serial.println("Sai do while");
      }
    }

    if (cenaPAtual > cenaPTotal)
    {
      rFile.close();
      SPIFFS.end();
      Serial.println("\nfim da cena\n");
      cenaExecucao = false;
      cenaPAtual = 0;
      cenaPTotal = 0;
    }
  }
}

void executaCena(String comandoCena)
{
  //Serial.print(comandoCena);
  String cmdChipID;
  String cmdIP;
  String cmdTipo;
  String cmdPorta;
  String cmdAcao;
  String cmdAcao2;
  String cmdQtde;
  String cmdValorMin;
  String cmdModelo;
  int i;
  int posicaoi = 1;
  int posicaof = 4;

  for (i = 0; i < comandoCena.length(); i++)
  {
    //Serial.println("Posicao " + String(i));
    if (comandoCena[i] != '|' && posicaoi <= posicaof)
    {
      if (posicaoi == 1) // chip id
      {
        cmdChipID += comandoCena[i];
      }
      if (posicaoi == 2) // IP
      {
        cmdIP += comandoCena[i];
      }
      if (posicaoi == 3) // Tipo do comando (1 - Rele; 2 - IR; 3 - Intervalo; 4 - Dimmer)
      {
        cmdTipo += comandoCena[i];
      }
      if (posicaoi == 4)
      {
        if (cmdTipo == "1")
        {
          posicaof = 5;
          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          posicaof = 8;
          cmdQtde += comandoCena[i];
        }
        else if (cmdTipo == "3")
        {
          posicaof = 4;
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          posicaof = 6;
          cmdPorta += comandoCena[i];
        }
      }
      else if (posicaoi == 5)
      {
        if (cmdTipo == "1")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdValorMin += comandoCena[i];
        }
      }
      else if (posicaoi == 6)
      {
        if (cmdTipo == "2")
        {
          cmdAcao2 += comandoCena[i];
          //          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdAcao += comandoCena[i];
        }
      }
      else if (posicaoi == 7)
      {
        if (cmdTipo == "2")
        {
          cmdPorta += comandoCena[i];
          //cmdModelo += comandoCena[i];
        }
      }
      else if (posicaoi == 8)
      {
        if (cmdTipo == "2")
        {
          cmdModelo += comandoCena[i];
        }
      }
    }
    else // encontrou o caracter '|'
    {
      posicaoi++;
    }
  }

  IPAddress Destino;
  Destino.fromString(cmdIP);
  String Texto;
  // comandos
  //Serial.println("Tipo " + cmdTipo);
  if (cmdTipo == "1") // Rele
  {
    if (Destino == IpDispositivo)
    {
      if (cmdAcao == "0") // desliga
      {
        LigaDesliga(cmdPorta.toInt() - 1, LOW, "", 0);
      }
      else if (cmdAcao == "1") // liga
      {
        LigaDesliga(cmdPorta.toInt() - 1, HIGH, "", 0);
      }
      else if (cmdAcao == "2") // pulso
      {
        LigaDesliga(cmdPorta.toInt() - 1, LOW, "", 1);
      }
      cenaPAtual++;
    }
    else //upd
    {
      char replyPacekt[255] = "";
      if (cmdAcao == "0") // desliga
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "1") // liga
      {
        Texto = cmdIP + "|" + cmdPorta + "|true|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "2") // pulso
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|1|";
      }
      Serial.println(Texto);
      Texto.toCharArray(replyPacekt, 255);
      Serial.println("enviado comando UDP");
      Udp.beginPacket(Destino, localUdpPort);
      Udp.write(replyPacekt);
      Udp.endPacket();
      delay(200);
      cenaPAtual++;
    }
  }
  else if (cmdTipo == "2") // IR
  {
    if (Destino == IpDispositivo)
    {
      if (lastCnTime == 0 || (millisAtual - lastCnTime >= 300))
      {
        //Serial.println("CenaAntes " + String(cenaPAtual));

        sendIRCMD(cmdAcao, cmdAcao2, cmdQtde.toInt(), cmdPorta.toInt(), cmdModelo.toInt(), cmdQtde.toInt());
        lastCnTime = millisAtual;
        //lastCnTime = millis();
        //Serial.println("terminou IR");
        // Serial.println("Cena apos ir " + String(cenaPAtual));
        cenaPAtual++;
        // Serial.println("Cena apos ir + 1 " + String(cenaPAtual));
        //delay(300);
      }
      else if (millisAtual - lastCnTime < 0)
      {
        lastCnTime = millisAtual;
        //lastCnTime = millis();
      }
    }
    else //upd - não implentado UPD para IR
    {
    }
  }
  else if (cmdTipo == "3") // timer
  {
    //Serial.println("Entrou no timer " + String(lastCnTime));
    if (lastCnTime <= 0)
    {
      lastCnTime = millisAtual;
    }

    if (lastCnTime == 0 || (millisAtual - lastCnTime >= cmdAcao.toInt()))
    {
      cenaPAtual++;
      lastCnTime = -1;
    }
    else if (millisAtual - lastCnTime < 0)
    {
      lastCnTime = millisAtual;
    }
  }
  else if (cmdTipo == "4") // Dimmer
  {
    char replyPacekt[255] = "";
    String Texto = cmdIP + "|" + cmdPorta + "|" + cmdAcao + "|" + cmdChipID + "|X|";

    Texto += cmdValorMin + "|n|";
    Texto.toCharArray(replyPacekt, 255);

    Serial.println("enviado comando UDP");
    Udp.beginPacket(Destino, localUdpPort);
    Udp.write(replyPacekt);
    Udp.endPacket();

    cenaPAtual++;
  }
  else
  {
    cenaPAtual++;
  }
}