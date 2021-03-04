void sendSMS(int numSensor)
{
  WiFiClient cliente;
  SPIFFS.begin();
  File fTexto = SPIFFS.open("/sms_text.txt", "r");
  File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

  String Texto = fTexto.readStringUntil('*');
  String Numeros = fNumeros.readStringUntil('*');

  fTexto.close();
  fNumeros.close();
  SPIFFS.end();

  if (Numeros.length() > 8 && Texto.length() > 4)
  {
    String data = "user=fcleal&password=201277&destinatario=" + Numeros + "&msg=" + Texto;

    (!DEBUG_ON) ?: Serial.println("envia sms");
    (!DEBUG_ON) ?: Serial.println(data);

    (!DEBUG_ON) ?: Serial.println("Send data...");
    if (cliente.connect("www.facilitamovel.com.br", 80))
    {
      (!DEBUG_ON) ?: Serial.println("Connected to the server..");
      cliente.println("POST /api/multipleSend.ft HTTP/1.1");
      //        cliente.println("Authorization: key=" + serve + "");
      cliente.println("Content-Type: application/x-www-form-urlencoded");
      cliente.println("Host: www.facilitamovel.com.br");
      cliente.print("Content-Length: ");
      cliente.println(data.length());
      cliente.println();
      cliente.println(data);
    }
    (!DEBUG_ON) ?: Serial.println("Enviado sms...Aguardando confirmacao..");
    while (cliente.available())
    {
      char c = cliente.read();
      //        (!DEBUG_ON) ?:   Serial.print(c);
    }
    (!DEBUG_ON) ?: Serial.println("SMS Enviado!");
    cliente.flush();
    //(!DEBUG_ON) ?:   Serial.println(data);

    msgDisparada[numSensor] = true;
  }
  cliente.stop();
}

void gravasms(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
  request->send(200, "text/html", "ok");
  String Senha = request->arg("k");
  String Texto = request->arg("t");
  String Numeros = request->arg("n");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File fTexto = SPIFFS.open("/sms_text.txt", "w");
    File fNumeros = SPIFFS.open("/sms_numb.txt", "w");

    if (!fTexto)
    {
      SPIFFS.format();
      File fTexto = SPIFFS.open("/sms_text.txt", "w");
    }

    if (!fNumeros)
    {
      SPIFFS.format();
      File fNumeros = SPIFFS.open("/sms_numb.txt", "w");
    }

    fTexto.println(Texto);
    fNumeros.println(Numeros);
    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    (!DEBUG_ON) ?: Serial.println(Numeros);
  }
}

void gravasms2(String Texto, String Numeros)
{
  SPIFFS.begin();
  File fTexto = SPIFFS.open("/sms_text.txt", "w");
  File fNumeros = SPIFFS.open("/sms_numb.txt", "w");

  if (!fTexto)
  {
    SPIFFS.format();
    File fTexto = SPIFFS.open("/sms_text.txt", "w");
  }

  if (!fNumeros)
  {
    SPIFFS.format();
    File fNumeros = SPIFFS.open("/sms_numb.txt", "w");
  }

  fTexto.println(Texto);
  fNumeros.println(Numeros);
  fTexto.close();
  fNumeros.close();
  SPIFFS.end();
}

void consultasms(AsyncWebServerRequest *request)
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String Senha = request->arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File fTexto = SPIFFS.open("/sms_text.txt", "r");
    File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

    String Texto = fTexto.readStringUntil('*');
    String Numeros = fNumeros.readStringUntil('*');

    if (Texto == "" && Numeros == "")
    {
      gravasms2("*", "*");
      (!DEBUG_ON) ?: Serial.println("SMS: " + Texto);
    }

    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    request->send(200, "text/html", Texto + "|" + Numeros);
  }
}

String consultasms2()
{
  SPIFFS.begin();
  File fTexto = SPIFFS.open("/sms_text.txt", "r");
  File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

  String Texto = fTexto.readStringUntil('*');
  String Numeros = fNumeros.readStringUntil('*');

  fTexto.close();
  fNumeros.close();
  SPIFFS.end();

  return Texto + "|" + Numeros;
}
