void api()
{
  String vPassApi, action, apiPort, apiSource, valueApi, typeApi;
  bool isPost = false;

  if (!newMqttMsg)
  {
    if (!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    isPost = true;
    vPassApi = server.arg("pw");
    vPassApi.toLowerCase();
    action = server.arg("a");
    apiPort = server.arg("p");
    apiSource = server.arg("s");
    //(!DEBUG_ON) ?:   Serial.println(apiSource);
  }
  else
  {
    newMqttMsg = false;
    isPost = false;
    vPassApi = MqttArg(msgMqtt, "pw");
    vPassApi.toLowerCase();
    (!DEBUG_ON) ?: Serial.print("vPassApi: ");
    (!DEBUG_ON) ?: Serial.println(vPassApi);

    action = MqttArg(msgMqtt, "a");
    (!DEBUG_ON) ?: Serial.print("action: ");
    (!DEBUG_ON) ?: Serial.println(action);

    apiPort = MqttArg(msgMqtt, "p");
    (!DEBUG_ON) ?: Serial.print("apiPort: ");
    (!DEBUG_ON) ?: Serial.println(apiPort);

    apiSource = MqttArg(msgMqtt, "s");
    (!DEBUG_ON) ?: Serial.print("apiSource: ");
    (!DEBUG_ON) ?: Serial.println(apiSource);
  }

  if (AlowApi == true && vPassApi == ApiPass)

  {
    // consulta
    if (action == "q")
    {
      //todas portas
      if (apiPort == "a")
      {
        String sDados1 = "";
        String sDados2 = "";
        if (apiSource == "o") // saidas
        {
          sDados1 = String(chip1.read8(), BIN);
          sDados2 = String(chip2.read8(), BIN);
        }
        else if (apiSource == "i") // entradas
        {
          sDados1 = String(sensor1.read8(), BIN);
          sDados2 = String(sensor2.read8(), BIN);
        }
        else
        {
          sDados1 = "00000000";
          sDados2 = "00000000";
        }

        (!DEBUG_ON) ?: Serial.println("tamanho");
        (!DEBUG_ON) ?: Serial.println(sDados1.length());
        while (sDados1.length() < 8)
        {
          sDados1 = '0' + sDados1;
        }

        while (sDados2.length() < 8)
        {
          sDados2 = '0' + sDados2;
        }

        for (int i = 0; i < 8; i++)
        {
          if (sDados1[i] == '0')
          {
            sDados1[i] = '1';
          }
          else
          {
            sDados1[i] = '0';
          }
        }

        for (int i = 0; i < 8; i++)
        {
          if (sDados2[i] == '0')
          {
            sDados2[i] = '1';
          }
          else
          {
            sDados2[i] = '0';
          }
        }

        if (isPost)
          server.send(200, "text/html", sDados2 + sDados1);
      }
      else
      {
        if (apiSource == "o")
        {
          if (LePorta(apiPort.toInt() - 1) == HIGH)
          {
            if (isPost)
              server.send(200, "text/html", "1");
          }
          else
          {
            if (isPost)
              server.send(200, "text/html", "0");
          }
        }
        else
        {
          if (LeSensor(apiPort.toInt() - 1) == HIGH)
          {
            if (isPost)
              server.send(200, "text/html", "1");
          }
          else
          {
            if (isPost)
              server.send(200, "text/html", "0");
          }
        }
      }
    }
    else if (action == "a") // ação
    {
      if (isPost)
      {
        valueApi = server.arg("v");
        typeApi = server.arg("t");
      }
      else
      {
        valueApi = MqttArg(msgMqtt, "v");
        typeApi = MqttArg(msgMqtt, "t");
      }

      if (apiPort == "a") // todos
      {
        if (apiSource == "o") // saidas
        {
          if (valueApi == "0")
          {
            if (typeApi == "n" || typeApi == "p")
            {

              chip1.write8(255);
              chip2.write8(255);
              memRtc.outValues = 255 << 8 | 255;
              memRtc.setOutputs();

              if (isPost)
                server.send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                server.send(200, "text/html", "-1");
            }
          }
          else if (valueApi == "1")
          {
            if (typeApi == "n")
            {
              chip1.write8(0);
              chip2.write8(0);
              memRtc.outValues = 0;
              memRtc.setOutputs();

              if (isPost)
                server.send(200, "text/html", "1");
            }
            else if (typeApi == "p")
            {
              chip1.write8(0);
              chip2.write8(0);
              for (uint8_t pulsoApiCount = 0; pulsoApiCount <= 15; pulsoApiCount++)
              {
                g_tempoInicioPulso[pulsoApiCount] = millisAtual;
                g_pulsoHabilita[pulsoApiCount] = true;
              }

              memRtc.outValues = 255 << 8 | 255; //Se for pulso sempre vai escrever off para ram
              memRtc.setOutputs();

              if (isPost)
                server.send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                server.send(200, "text/html", "-1");
            }
          }
          else
          {
            if (isPost)
              server.send(200, "text/html", "-1");
          }
        }
        else
        {
          if (isPost)
            server.send(200, "text/html", "-1");
        }
      }
      else // porta
      {
        if (apiSource == "o") // saidas
        {
          if (typeApi == "n") //normal
          {
            if (valueApi == "0") // desliga
            {
              LigaDesliga(apiPort.toInt() - 1, LOW, "", 0);
              if (isPost)
                server.send(200, "text/html", "1");
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 0);
              if (isPost)
                server.send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                server.send(200, "text/html", "-1");
            }
          }
          else if (typeApi == "p") // pulsado
          {
            if (valueApi == "0") // desliga
            {
              LigaDesliga(apiPort.toInt() - 1, LOW, "", 1);
              if (isPost)
                server.send(200, "text/html", "1");
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 1);
              if (isPost)
                server.send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                server.send(200, "text/html", "-1");
            }
          }
          else
          {
            if (isPost)
              server.send(200, "text/html", "-1");
          }
        }
        else
        {
          if (isPost)
            server.send(200, "text/html", "-1");
        }
      }
    }
    else if (action == "i") // Infravermelho
    {
      (!DEBUG_ON) ?: Serial.println("api infravermelho");
      String vModel1 = server.arg("m1");
      String vModel2 = server.arg("m2");
      String vModel3 = server.arg("m3");
      String vModel4 = server.arg("m4");
      String Comando1 = server.arg("c1");
      String Comando2 = server.arg("c2");
      String Comando3 = server.arg("c3");
      String Comando4 = server.arg("c4");
      String Comando12 = server.arg("c12");
      String Comando22 = server.arg("c22");
      String Comando32 = server.arg("c32");
      String Comando42 = server.arg("c42");
      String qtde1 = server.arg("qt1");
      String qtde2 = server.arg("qt2");
      String qtde3 = server.arg("qt3");
      String qtde4 = server.arg("qt4");
      String vp1 = server.arg("p1");
      String vp2 = server.arg("p2");
      String vp3 = server.arg("p3");
      String vp4 = server.arg("p4");

      if (vModel1 != "")
      {
        sendirAPI(qtde1.toInt(), vModel1.toInt(), Comando1, Comando12, vp1.toInt());
        (!DEBUG_ON) ?: Serial.println("1");
      }

      if (vModel2 != "")
      {
        delay(300);
        sendirAPI(qtde2.toInt(), vModel2.toInt(), Comando2, Comando22, vp2.toInt());
        (!DEBUG_ON) ?: Serial.println("2");
      }

      if (vModel3 != "")
      {
        delay(300);
        sendirAPI(qtde3.toInt(), vModel3.toInt(), Comando3, Comando32, vp3.toInt());
        (!DEBUG_ON) ?: Serial.println("3");
      }

      if (vModel4 != "")
      {
        delay(300);
        sendirAPI(qtde4.toInt(), vModel4.toInt(), Comando4, Comando42, vp4.toInt());
      }
      if (isPost)
        server.send(200, "text/html", "1");
    }
    else if (action == "c") // Cenas
    {
      (!DEBUG_ON) ?: Serial.println("api cenas");
      if (isPost)
        String valueApi = server.arg("v");
      else
        String valueApi = MqttArg(msgMqtt, "v");
      triggerCena(valueApi);
      if (isPost)
        server.send(200, "text/html", "1");
    }
    else if (action == "l") // linha de ação
    {
      for (int i = 1; i <= apiPort.length(); i++)
      {
        int posicaoPorta = i - 1;
        if (posicaoPorta <= 15)
        {
          if (apiPort[posicaoPorta] == '0')
          {
            // desliga
            LigaDesliga(posicaoPorta, LOW, "", 0);
          }
          else if (apiPort[posicaoPorta] == '1')
          {
            // liga
            LigaDesliga(posicaoPorta, HIGH, "", 0);
          }
        }
      }

      if (isPost)
        server.send(200, "text/html", "1");
    }
    else if (action == "update") // executaupdate
    {
      (!DEBUG_ON) ?: Serial.println("API: Executar update");

      if (isPost)
      {
        if (!server.authenticate(www_username, www_password))
          return server.requestAuthentication();
        executeupdateBeta();
      }
      else
        executeupdateBeta();
    }
  }
  else
  {
    if (isPost)
      server.send(200, "text/html", "-1");
  }
}

void apiativo()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  if (AlowApi == true)
  {
    server.send(200, "text/html", "1");
  }
  else
  {
    server.send(200, "text/html", "0");
  }
}

void apiconfig()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  if (server.arg("s") == Senha)
  {
    server.send(200, "text/html", "ok");

    AlowApi = (server.arg("v") == "1") ? true : false;
    bitWrite(DevSet.mode, 0, AlowApi);
    DevSet.setMode();
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void alterasenhapi()
{

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  if (server.arg("s") == Senha)
  {

    MD5Builder md5;
    md5.begin();
    md5.add(server.arg("a"));
    md5.calculate();

    if (ApiPass == md5.toString())
    {

      server.send(200, "text/html", "ok");

      String req = server.arg("v");
      if (req == "")
      {
        req = "12345678"; // se não houver registro, vai para o padrão
      }

      md5.begin();
      md5.add(req);
      md5.calculate();
      ApiPass = md5.toString();

      DevSet.apiPwd = ApiPass;
      DevSet.setApiPwd();
    }
    else
    {
      server.send(200, "text/html", "-1");
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}