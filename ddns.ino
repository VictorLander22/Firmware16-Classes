void chamaddns()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String sIP = server.arg("ip");
  String Senha = server.arg("s");
  int Porta = server.arg("p").toInt();
  String funcao = server.arg("f");
  int IdChip = server.arg("c").toInt();
  String Tipo = server.arg("t");
  String Nome = server.arg("n");
  int Tipoa = server.arg("pu").toInt();

  IPAddress Destino;

  Destino.fromString(sIP);

  if (Destino != IpDispositivo)
  {

    if (Senha == "kdi9e")
    {
      char replyPacekt[255] = "";
      String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip) + "|" + Tipo + "|";

      if (Tipo == "A" || Tipo == "G")
      {
        Texto += server.arg("j") + "|n|";
      }
      else if (Tipo == "B")
      {
        Texto += server.arg("j") + "|" + server.arg("b") + "|";
      }
      else if (Tipo == "E" || Tipo == "X")
      {
        Texto += String(Tipoa) + "|n|";
      }
      else
      {
        Texto += "n|n|";
      }

      Texto.toCharArray(replyPacekt, 255);

      Serial.println("enviado comando UDP");
      Serial.println(Texto);
      Udp.beginPacket(Destino, localUdpPort);
      Udp.write(replyPacekt);
      Udp.endPacket();

      if (Tipo == "E")
      {
        //server.send(200, "text/html", "ok");
      }
      else
      {
        //server.send(200, "text/html", "ok");
      }
    }
  }
  else
  {
    Porta = retornaPorta(Porta);
    if (Porta >= 0)
    {
      if (Senha == "kdi9e")
      {
        if (Tipo == "E")
        {
          if (funcao == "true")
          {
            server.send(200, "text/html", "ok");
            LigaDesliga(Porta, HIGH, Nome, Tipoa);
            Serial.println("led 1 ligado");
          }
          else
          {
            server.send(200, "text/html", "ok");
            LigaDesliga(Porta, LOW, Nome, Tipoa);
            Serial.println("led 1 desligado");
          }
        }
        else if (Tipo == "C")
        {
          if (LePorta(Porta) == HIGH)
          {
            server.send(200, "text/html", "true");
          }
          else
          {
            server.send(200, "text/html", "false");
          }
        }
        else if (Tipo == "S")
        {
          if (ultimoDisparo == "")
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

            server.send(200, "text/html", sSensor1 + sSensor2);
          }
          else
          {
            server.send(200, "text/html", ultimoDisparo);
          }
        }
        else if (Tipo == "N")
        {
          server.send(200, "text/html", String(notificar));
        }
        else if (Tipo == "A")
        {
          server.send(200, "text/html", "ok");
          String Texto = server.arg("j");
          gravasensor2(Texto);
        }
        else if (Tipo == "G")
        {
          String Texto = server.arg("j");
          // liberar depois que colocar o arquivo de RF          gravarf2(Texto);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "B")
        {
          server.send(200, "text/html", "ok");
          String Texto = server.arg("j");
          String Telef = server.arg("b");
          gravasms2(Texto, Telef);
        }
        else if (Tipo == "D")
        {
          server.send(200, "text/html", consultasms2());
        }
        else if (Tipo == "F")
        {
          server.send(200, "text/html", "ok");
          String Valor = server.arg("j");
          gravanot2(Valor);
        }
        else if (Tipo == "I")
        {
          Inverte(Porta);
        }
        else if (Tipo == "H")
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

          int32_t rssi;
          rssi = WiFi.RSSI();
          char replyPacekt[255] = "";
          String valSensores = sSensor1 + sSensor2;
          valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + valSensores + "|" + sChip1 + sChip2 + "|" + rssi + "*";
          valSensores.toCharArray(replyPacekt, 255);
          server.send(200, "text/html", replyPacekt);
        }
      }
    }
  }
}
