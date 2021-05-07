void leituraUDP()
{

  //UDP
  int packetSize = Udp.parsePacket();
  int len = Udp.read(incomingPacket, 255);
  if (len > 0)
  {
    incomingPacket[len] = 0;
  }

  String cabecalho = "";
  for (int i = 0; i < 255; i++)
  {
    cabecalho += incomingPacket[i];
  }

  if (packetSize > 16)
  {
    //(!DEBUG_ON) ?:   Serial.println(cabecalho.substring(0,7));

    if (cabecalho.substring(0, 7) != "2934d03")
    {
      if (cabecalho.substring(0, 3) == "pw=")
      {
        //(!DEBUG_ON) ?: Serial.println(incomingPacket);
        Udp.beginPacket(Udp.remoteIP(), 35701);
        Udp.write(getDevStatus().c_str());
        //Udp.write(resp);
        Udp.endPacket();
        //api(request);
      }
      else
      {

        // receive incoming UDP packets
        (!DEBUG_ON) ?: Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        (!DEBUG_ON) ?: Serial.printf("UDP packet contents: %s\n", incomingPacket);

        String sIP = "";
        String Porta = "";
        int i2 = 0;
        String funcao = "";
        String ChipId = "";
        String Tipo = "";
        String Texto = "";
        String Telefone = "";

        for (int i = 0; i < 255; i++)
        {
          if (incomingPacket[i] != '|' && i2 <= 6)
          {
            if (i2 == 0)
            {
              sIP += incomingPacket[i];
            }

            if (i2 == 1)
            {
              Porta += incomingPacket[i];
            }
            if (i2 == 2)
            {
              funcao += incomingPacket[i];
            }
            if (i2 == 3)
            {
              ChipId += incomingPacket[i];
            }
            if (i2 == 4)
            {
              Tipo += incomingPacket[i];
            }
            if (i2 == 5)
            {
              Texto += incomingPacket[i];
            }
            if (i2 == 6)
            {
              Telefone += incomingPacket[i];
            }
          }
          else
          {
            i2++;
          }
        }
        //    (!DEBUG_ON) ?:   Serial.println("");
        //(!DEBUG_ON) ?:   Serial.println("IP: " + sIP);
        //(!DEBUG_ON) ?:   Serial.println("Porta: " + Porta);
        //(!DEBUG_ON) ?:   Serial.println("Funcao: " + funcao);
        //(!DEBUG_ON) ?:   Serial.println("ChipId: " + ChipId);
        //(!DEBUG_ON) ?:   Serial.println("Tipo: " + Tipo);
        IPAddress Destino;
        Destino.fromString(sIP);

        // if (Porta != "")
        //pinMode(Porta.toInt(), OUTPUT);

        //if (Porta.toInt() == 1)
        //{
        //      Porta = "14";
        //  }

        if (Destino == IpDispositivo)
        {
          Porta = String(retornaPorta(Porta.toInt()));
          if (Tipo == "E")
          {
            if (funcao == "true")
            {
              //digitalWrite(Porta.toInt(), 1);
              LigaDesliga(Porta.toInt(), HIGH, "", Texto.toInt());
              (!DEBUG_ON) ?: Serial.println("led ligado UDP");
            }
            else
            {
              //digitalWrite(Porta.toInt(), 0);
              LigaDesliga(Porta.toInt(), LOW, "", Texto.toInt());
              (!DEBUG_ON) ?: Serial.println("led 1 desligado UDP");
            }
          }
          else if (Tipo == "C")
          {
            if (LePorta(Porta.toInt()) == HIGH)
            {
              //request->send(200, "text/html", "true");

              Tipo = "R";
              funcao = "true";
              char replyPacekt[255] = "";
              IPAddress ipRetorno = Udp.remoteIP();
              String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId + "|" + Tipo + "|";
              Texto.toCharArray(replyPacekt, 255);

              (!DEBUG_ON) ?: Serial.println("enviado comando UDP");
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
              Udp.write(replyPacekt);
              Udp.endPacket();
            }
            else
            {
              //request->send(200, "text/html", "false");

              Tipo = "R";
              funcao = "false";
              char replyPacekt[255] = "";
              IPAddress ipRetorno = Udp.remoteIP();
              String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId + "|" + Tipo + "|";
              Texto.toCharArray(replyPacekt, 255);

              //(!DEBUG_ON) ?:   Serial.println("enviado comando UDP");
              Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
              Udp.write(replyPacekt);
              Udp.endPacket();
            }
          }
          else if (Tipo == "R")
          {
            if (funcao == "true")
            {
              //request->send(200, "text/html", "true");
            }
            else
            {
              //request->send(200, "text/html", "false");
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

              //request->send(200, "text/html", sSensor1 + sSensor2);
            }
            else
            {
              //request->send(200, "text/html", ultimoDisparo);
            }
          }
          else if (Tipo == "N")
          {
            //request->send(200, "text/html", String(notificar));
          }
          else if (Tipo == "A")
          {
            //request->send(200, "text/html", "ok");
            gravasensor2(Texto);
          }
          else if (Tipo == "B")
          {
            //request->send(200, "text/html", "ok");
            String Texto = ""; //request->arg("j");
            String Telef = ""; //request->arg("b");
            gravasms2(Texto, Telef);
          }
          else if (Tipo == "D")
          {
            //request->send(200, "text/html", consultasms2());
          }
          else if (Tipo == "F")
          {
            //request->send(200, "text/html", "ok");
            String Valor = ""; //request->arg("j");
            gravanot2(Valor);
          }
          else if (Tipo == "X") // retorno udp valor do dimmer
          {
            //request->send(200, "text/html", funcao);
          }
          else if (Tipo == "I")
          {
            Inverte(Porta.toInt());
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

            // int32_t rssi;
            // rssi = WiFi.RSSI();
            char replyPacekt[255] = "";
            String valSensores = sSensor1 + sSensor2;
            valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + valSensores + "|" + sChip1 + sChip2 + "|" + getRSSI() + "*";
            valSensores.toCharArray(replyPacekt, 255);
            //retorna udp

            //(!DEBUG_ON) ?:   Serial.println(Contador);
            //    Contador = 0;

            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write(replyPacekt);
            Udp.endPacket();

            //request->send(200, "text/html", replyPacekt);
          }

          //retorna udp
          char replyPacekt[255] = "";
          String Texto = "ok";
          Texto.toCharArray(replyPacekt, 255);

          //(!DEBUG_ON) ?:   Serial.println("enviado comando UDP");
          Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
          Udp.write(replyPacekt);
          Udp.endPacket();
        }
      }
    }
  }
  else if (packetSize == 2)
  {
    //request->send(200, "text/html", "ok");
  }
}
