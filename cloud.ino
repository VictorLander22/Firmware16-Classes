void cloud(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", "ok");

  executeCloud = true;
}

void cloud1()
{
  if (executeCloud)
  {
    executeCloud = false;
    HTTPClient http;
    http.begin("http://cloud.keepin.com.br/api/control/3");
    int httpCode = http.GET();
    String payload;
    if (httpCode > 0)
    {
      payload = http.getString();
      (!DEBUG_ON) ?: Serial.println(payload);
    }

    http.end();

    //    StaticJsonBuffer<200> jsonBuffer;

    //DynamicJsonBuffer jsonBuffer(payload.length());
    //JsonObject &root = jsonBuffer.parseObject(payload);
    (!DEBUG_ON) ?: Serial.println(payload.length());

    DynamicJsonDocument root(payload.length() * 2);
    auto error = deserializeJson(root, payload);

    if (error)
    {
      //(!DEBUG_ON) ?: Serial.println("parseObject() failed");
      (!DEBUG_ON) ?: Serial.print(F("deserializeJson() failed with code "));
      (!DEBUG_ON) ?: Serial.println(error.c_str());
      return;
    }
    else
    {

      const String Descricao = root["descricao"];
      const bool ED1 = root["ed1"];
      const bool ED3 = root["ed3"];

      (!DEBUG_ON) ?: Serial.println(Descricao);
      (!DEBUG_ON) ?: Serial.println(ED1);
      (!DEBUG_ON) ?: Serial.println(ED3);

      //(!DEBUG_ON) ?: Serial.println(root["ed4"]);
      //(!DEBUG_ON) ?: Serial.println(root["ed4"]);
    }
  }
  // if (!root.success())
  // {
  //   (!DEBUG_ON) ?: Serial.println("parseObject() failed");
  // }
  // else
  // {

  //   const String Descricao = root["descricao"];
  //   const bool ED1 = root["ed1"];
  //   const bool ED3 = root["ed3"];

  //   (!DEBUG_ON) ?: Serial.println(Descricao);
  //   (!DEBUG_ON) ?: Serial.println(ED1);
  //   (!DEBUG_ON) ?: Serial.println(ED3);
  // }
}

void sendCloud(bool onlyNotify = false)
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

  // int32_t rssi;
  // rssi = WiFi.RSSI();

  String dataPost = "";
  dataPost = "{";
  dataPost = dataPost + "\"idchip\": \"" + vchipId + "\",";
  dataPost = dataPost + "\"SD1\": \"" + retornaValorCloud(sChip1[7]) + "\",";
  dataPost = dataPost + "\"SD2\": \"" + retornaValorCloud(sChip1[6]) + "\",";
  dataPost = dataPost + "\"SD3\": \"" + retornaValorCloud(sChip1[5]) + "\",";
  dataPost = dataPost + "\"SD4\": \"" + retornaValorCloud(sChip1[4]) + "\",";
  dataPost = dataPost + "\"SD5\": \"" + retornaValorCloud(sChip1[3]) + "\",";
  dataPost = dataPost + "\"SD6\": \"" + retornaValorCloud(sChip1[2]) + "\",";
  dataPost = dataPost + "\"SD7\": \"" + retornaValorCloud(sChip1[1]) + "\",";
  dataPost = dataPost + "\"SD8\": \"" + retornaValorCloud(sChip1[0]) + "\",";
  dataPost = dataPost + "\"SD9\": \"" + retornaValorCloud(sChip2[7]) + "\",";
  dataPost = dataPost + "\"SD10\": \"" + retornaValorCloud(sChip2[6]) + "\",";
  dataPost = dataPost + "\"SD11\": \"" + retornaValorCloud(sChip2[5]) + "\",";
  dataPost = dataPost + "\"SD12\": \"" + retornaValorCloud(sChip2[4]) + "\",";
  dataPost = dataPost + "\"SD13\": \"" + retornaValorCloud(sChip2[3]) + "\",";
  dataPost = dataPost + "\"SD14\": \"" + retornaValorCloud(sChip2[2]) + "\",";
  dataPost = dataPost + "\"SD15\": \"" + retornaValorCloud(sChip2[1]) + "\",";
  dataPost = dataPost + "\"SD16\": \"" + retornaValorCloud(sChip2[0]) + "\",";
  dataPost = dataPost + "\"ED1\": \"" + retornaValorCloud(sSensor1[7]) + "\",";
  dataPost = dataPost + "\"ED2\": \"" + retornaValorCloud(sSensor1[6]) + "\",";
  dataPost = dataPost + "\"ED3\": \"" + retornaValorCloud(sSensor1[5]) + "\",";
  dataPost = dataPost + "\"ED4\": \"" + retornaValorCloud(sSensor1[4]) + "\",";
  dataPost = dataPost + "\"ED5\": \"" + retornaValorCloud(sSensor1[3]) + "\",";
  dataPost = dataPost + "\"ED6\": \"" + retornaValorCloud(sSensor1[2]) + "\",";
  dataPost = dataPost + "\"ED7\": \"" + retornaValorCloud(sSensor1[1]) + "\",";
  dataPost = dataPost + "\"ED8\": \"" + retornaValorCloud(sSensor1[0]) + "\",";
  dataPost = dataPost + "\"ED9\": \"" + retornaValorCloud(sSensor2[7]) + "\",";
  dataPost = dataPost + "\"ED10\": \"" + retornaValorCloud(sSensor2[6]) + "\",";
  dataPost = dataPost + "\"ED11\": \"" + retornaValorCloud(sSensor2[5]) + "\",";
  dataPost = dataPost + "\"ED12\": \"" + retornaValorCloud(sSensor2[4]) + "\",";
  dataPost = dataPost + "\"ED13\": \"" + retornaValorCloud(sSensor2[3]) + "\",";
  dataPost = dataPost + "\"ED14\": \"" + retornaValorCloud(sSensor2[2]) + "\",";
  dataPost = dataPost + "\"ED15\": \"" + retornaValorCloud(sSensor2[1]) + "\",";
  dataPost = dataPost + "\"ED16\": \"" + retornaValorCloud(sSensor2[0]) + "\",";
  dataPost = dataPost + "\"ip\": \"" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "\",";
  dataPost = dataPost + "\"notificacao\": \"" + String(notificar) + "\",";
  dataPost = dataPost + "\"sinal\": \"" + String(getRSSI()) + "\"";
  dataPost = dataPost + " }";

  //(!DEBUG_ON) ?: Serial.println(dataPost);

  WiFiClient cliente;
  HTTPClient http;
  String payload;
  http.setTimeout(600);
  http.setReuse(true);
  //http.useHTTP10(false);
  //  http.begin(cliente, "192.168.1.147", 443, "/api/keepin", true);
  http.begin(cliente, "http://cloud.keepin.com.br/api/keepin");
  //  http.begin(cliente, "http://192.168.1.147/api/keepin");
  http.addHeader("Content-Type", "application/json");
  http.setUserAgent("KEEPIN/" + String(Placa_Version) + " Automacao");
  //http.setUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0");
  int httpCode = http.POST(dataPost);
  //http.writeToStream(&Serial);
  payload = http.getString();
  http.end();
  //(!DEBUG_ON) ?:   Serial.println(payload);
  //(!DEBUG_ON) ?: Serial.println("Cloud code: " + String(httpCode));
  if (httpCode == 200 && payload != "[]" && !onlyNotify)
  {
    //(!DEBUG_ON) ?:   Serial.println("Payload: " + payload);
    //DynamicJsonBuffer jsonBuffer(payload.length());
    //JsonArray &array1 = jsonBuffer.parseArray(payload);
    (!DEBUG_ON) ?: Serial.println(payload);
    (!DEBUG_ON) ?: Serial.println(payload.length());
    DynamicJsonDocument array1(payload.length() * 2);
    auto error = deserializeJson(array1, payload);
    //    JsonObject& root = jsonBuffer.parseObject(payload);

    if (error)
    {
      (!DEBUG_ON) ?: Serial.print(F("deserializeJson() failed with code "));
      (!DEBUG_ON) ?: Serial.println(error.c_str());
      return;
    }
    else
    {
      //(!DEBUG_ON) ?:   Serial.println("qtde registros");
      //(!DEBUG_ON) ?:   Serial.println(array1.size());

      for (int indice = 0; indice < array1.size(); indice++)
      {
        //JsonObject& root = jsonBuffer.parseObject(array1[indice]);

        //if(!root.success()) {
        //(!DEBUG_ON) ?:   Serial.println("parseObject() failed");
        //}
        //{"tipo":"1","acao":"1","modelo":null,"qtdeBit":null,"porta":3},
        const String tipoJson = array1[indice]["tipo"];
        const String acaoJson = array1[indice]["acao"];
        const String qtdeJson = array1[indice]["qtdeBit"];
        const String modeloJson = array1[indice]["modelo"];
        const String portaJson = array1[indice]["porta"];
        //(!DEBUG_ON) ?:   Serial.println("porta: " + portaJson);

        if (tipoJson == "1") // saida
        {
          //(!DEBUG_ON) ?:   Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            if (acaoJson == "1")
            {
              LigaDesliga(porta, HIGH, "", 0);
            }
            else if (acaoJson == "0")
            {
              LigaDesliga(porta, LOW, "", 0);
            }
          }
        }
        else if (tipoJson == "2") // saida pulsada
        {
          //(!DEBUG_ON) ?:   Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            LigaDesliga(porta, HIGH, "", 1);
          }
        }
        else if (tipoJson == "3") // IR
        {
          //(!DEBUG_ON) ?:   Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            cenaPAtual++;
            sendIRCMD(acaoJson, "", qtdeJson.toInt(), portaJson.toInt(), modeloJson.toInt(), qtdeJson.toInt());

            // irNumBits = qtdeJson.toInt();
            // irModel = modeloJson.toInt();
            // irPort = portaJson.toInt();
            // irData = acaoJson;

            // irEnSend = true;
            //(!DEBUG_ON) ?: Serial.println(F("Enviar IR..."));

            lastCnTime = millisAtual;
            delay(300);
          }
        }
        else if (tipoJson == "4") // RF
        {
          unsigned long Valor = strtoul(acaoJson.c_str(), NULL, 10);

          sSendRF.send(Valor, 32);
        }
        else if (tipoJson == "5")
        {
          triggerCena(acaoJson);
        }
        else if (tipoJson == "6") // notificacao
        {
          SPIFFS.begin();
          File f = SPIFFS.open("/notific.txt", "w");

          f.println(acaoJson);

          f.close();
          SPIFFS.end();

          if (acaoJson == "true")
          {
            notificar = true;
          }
          else
          {
            notificar = false;
          }
        }
      }
    }

    if (payload != "[]")
    {
      http.setTimeout(600);
      http.setReuse(true);
      http.begin(cliente, "http://cloud.keepin.com.br/api/keepinactions/delete");
      http.addHeader("Content-Type", "application/json");
      http.setUserAgent("KEEPIN/" + String(Placa_Version) + " Automacao");
      //http.setUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0");
      httpCode = http.POST(payload);
      if (httpCode == 200)
      {
        payload = http.getString();
        //(!DEBUG_ON) ?:   Serial.println(payload);
      }
    }
    http.end();
  }
  else if (httpCode != 200 && !onlyNotify)
  {
    (!DEBUG_ON) ?: Serial.println("servidor fora! Code: " + String(httpCode));
    http.end();
  }
  cliente.stop();
}

String retornaValorCloud(char val)
{
  if (val == '0')
  {
    return "true";
  }
  else
  {
    return "false";
  }
}

void LoopCloud()
{
  // só acessa o cloud se estiver marcado para usar.
  if ((usaCloud == true) && WiFi.status() == WL_CONNECTED && hasInternet)
  {
    if (HorarioAtual.Second() % 5 == 0 && ConsultouCloud == false)
    {
      ConsultouCloud = true;
      //      (!DEBUG_ON) ?:   Serial.println("Segundo Atual: " + String(HorarioAtual.Second()));
      sendCloud();
    }
    else if (HorarioAtual.Second() % 5 != 0 && ConsultouCloud == true)
    {
      ConsultouCloud = false;
    }
  }
}