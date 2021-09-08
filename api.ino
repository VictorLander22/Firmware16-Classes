void api()
{
  String vPassApi, action, apiPort, apiSource, valueApi, typeApi;
  bool isPost = false;

  if (!newMqttMsg)
  {
    isPost = true;
    vPassApi = gRequest->arg("pw");
    vPassApi.toLowerCase();
    action = gRequest->arg("a");
    apiPort = gRequest->arg("p");
    apiSource = gRequest->arg("s");
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
    if (action == "q") //consulta
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
          gRequest->send(200, "text/html", sDados2 + sDados1);
      }
      else
      {
        if (apiSource == "o")
        {
          if (LePorta(apiPort.toInt() - 1) == HIGH)
          {
            if (isPost)
              gRequest->send(200, "text/html", "1");
          }
          else
          {
            if (isPost)
              gRequest->send(200, "text/html", "0");
          }
        }
        else
        {
          if (LeSensor(apiPort.toInt() - 1) == HIGH)
          {
            if (isPost)
              gRequest->send(200, "text/html", "1");
          }
          else
          {
            if (isPost)
              gRequest->send(200, "text/html", "0");
          }
        }
      }
    }
    else if (action == "a") // ação
    {
      if (isPost)
      {
        valueApi = gRequest->arg("v");
        typeApi = gRequest->arg("t");
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
              //memRtc.outValues = 255 << 8 | 255;
              //memRtc.setOutputs();
              SaveOutputs();

              if (isPost)
                gRequest->send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                gRequest->send(200, "text/html", "-1");
            }
          }
          else if (valueApi == "1")
          {
            if (typeApi == "n")
            {
              chip1.write8(0);
              chip2.write8(0);
              //memRtc.outValues = 0;
              //memRtc.setOutputs();
              SaveOutputs();

              if (isPost)
                gRequest->send(200, "text/html", "1");
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
              //memRtc.outValues = 255 << 8 | 255; //Se for pulso sempre vai escrever off para ram
              //memRtc.setOutputs();
              SaveOutputs();

              if (isPost)
                gRequest->send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                gRequest->send(200, "text/html", "-1");
            }
          }
          else
          {
            if (isPost)
              gRequest->send(200, "text/html", "-1");
          }
        }
        else
        {
          if (isPost)
            gRequest->send(200, "text/html", "-1");
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
                gRequest->send(200, "text/html", "1");
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 0);
              if (isPost)
                gRequest->send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                gRequest->send(200, "text/html", "-1");
            }
          }
          else if (typeApi == "p") // pulsado
          {
            if (valueApi == "0") // desliga
            {
              LigaDesliga(apiPort.toInt() - 1, LOW, "", 1);
              if (isPost)
                gRequest->send(200, "text/html", "1");
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 1);
              if (isPost)
                gRequest->send(200, "text/html", "1");
            }
            else
            {
              if (isPost)
                gRequest->send(200, "text/html", "-1");
            }
          }
          else
          {
            if (isPost)
              gRequest->send(200, "text/html", "-1");
          }
        }
        else
        {
          if (isPost)
            gRequest->send(200, "text/html", "-1");
        }
      }
    }
    else if (action == "i") // Infravermelho
    {
      (!DEBUG_ON) ?: Serial.println("api infravermelho");
      // String vModel1 = gRequest->arg("m1");
      // String vModel2 = gRequest->arg("m2");
      // String vModel3 = gRequest->arg("m3");
      // String vModel4 = gRequest->arg("m4");
      // String Comando1 = gRequest->arg("c1");
      // String Comando2 = gRequest->arg("c2");
      // String Comando3 = gRequest->arg("c3");
      // String Comando4 = gRequest->arg("c4");
      // String Comando12 = gRequest->arg("c12");
      // String Comando22 = gRequest->arg("c22");
      // String Comando32 = gRequest->arg("c32");
      // String Comando42 = gRequest->arg("c42");
      // String qtde1 = gRequest->arg("qt1");
      // String qtde2 = gRequest->arg("qt2");
      // String qtde3 = gRequest->arg("qt3");
      // String qtde4 = gRequest->arg("qt4");
      // String vp1 = gRequest->arg("p1");
      // String vp2 = gRequest->arg("p2");
      // String vp3 = gRequest->arg("p3");
      // String vp4 = gRequest->arg("p4");

      // if (vModel1 != "")
      // {
      //   sendirAPI(qtde1.toInt(), vModel1.toInt(), Comando1, Comando12, vp1.toInt());
      //   (!DEBUG_ON) ?: Serial.println("1");
      // }

      // if (vModel2 != "")
      // {
      //   delay(300);
      //   sendirAPI(qtde2.toInt(), vModel2.toInt(), Comando2, Comando22, vp2.toInt());
      //   (!DEBUG_ON) ?: Serial.println("2");
      // }

      // if (vModel3 != "")
      // {
      //   delay(300);
      //   sendirAPI(qtde3.toInt(), vModel3.toInt(), Comando3, Comando32, vp3.toInt());
      //   (!DEBUG_ON) ?: Serial.println("3");
      // }

      // if (vModel4 != "")
      // {
      //   delay(300);
      //   sendirAPI(qtde4.toInt(), vModel4.toInt(), Comando4, Comando42, vp4.toInt());
      // }
      if (isPost)
      {
        irNumBits = gRequest->arg("qt1").toInt();
        irModel = gRequest->arg("m1").toInt();
        irPort = gRequest->arg("p1").toInt();
        irData = gRequest->arg("c1") + gRequest->arg("c12");
      }
      else
      {
        irNumBits = MqttArg(msgMqtt, "qt1").toInt();
        irModel = MqttArg(msgMqtt, "m1").toInt();
        irPort = MqttArg(msgMqtt, "p1").toInt();
        irData = MqttArg(msgMqtt, "c1");
      }
      irEnSend = true;
      (!DEBUG_ON) ?: Serial.println(F("Enviar IR..."));

      if (isPost)
        gRequest->send(200, "text/html", "1");
    }
    else if (action == "c") // Cenas
    {
      (!DEBUG_ON) ?: Serial.println("api cenas");
      if (isPost)
      {
        valueApi = gRequest->arg("v");
        gRequest->send(200, "text/html", "1");
      }
      else
        valueApi = MqttArg(msgMqtt, "v");

      (!DEBUG_ON) ?: Serial.println("Numero da cena: " + valueApi);

      triggerCena(valueApi);
      //if (isPost)
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
        gRequest->send(200, "text/html", "1");
    }
    else if (action == "update") // executaupdate
    {
      (!DEBUG_ON) ?: Serial.println(F("API: Executar update"));

      if (isPost)
      {
        // if (!gRequest->authenticate(www_username, www_password))
        //   return gRequest->gRequestAuthentication();
        //executeupdateBeta(gRequest);
      }
      else
        //executeupdateBeta(gRequest);
        (!DEBUG_ON) ?: Serial.println("");
    }
  }
  else
  {
    if (isPost)
      gRequest->send(200, "text/html", "-1");
  }
}

void apiativo()
{
  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  if (AlowApi == true)
  {
    gRequest->send(200, "text/html", "1");
  }
  else
  {
    gRequest->send(200, "text/html", "0");
  }
}

void apiconfig()
{
  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  if (gRequest->arg("s") == Senha)
  {
    gRequest->send(200, "text/html", "ok");

    AlowApi = (gRequest->arg("v") == "1") ? true : false;
    bitWrite(DevSet.mode, 0, AlowApi);
    DevSet.setMode();
  }
  else
  {
    gRequest->send(200, "text/html", "-1");
  }
}

void alterasenhapi()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  if (gRequest->arg("s") == Senha)
  {

    MD5Builder md5;
    md5.begin();
    md5.add(gRequest->arg("a"));
    md5.calculate();

    if (ApiPass == md5.toString())
    {

      gRequest->send(200, "text/html", "ok");

      String req = gRequest->arg("v");
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
      gRequest->send(200, "text/html", "-1");
    }
  }
  else
  {
    gRequest->send(200, "text/html", "-1");
  }
}