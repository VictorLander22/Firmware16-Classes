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

    Serial.println("envia sms");
    Serial.println(data);

    Serial.println("Send data...");
    if (cliente.connect("www.facilitamovel.com.br", 80))
    {
      Serial.println("Connected to the server..");
      cliente.println("POST /api/multipleSend.ft HTTP/1.1");
      //        cliente.println("Authorization: key=" + serve + "");
      cliente.println("Content-Type: application/x-www-form-urlencoded");
      cliente.println("Host: www.facilitamovel.com.br");
      cliente.print("Content-Length: ");
      cliente.println(data.length());
      cliente.println();
      cliente.println(data);
    }
    Serial.println("Enviado sms...Aguardando confirmacao..");
    while (cliente.available())
    {
      char c = cliente.read();
      //        Serial.print(c);
    }
    Serial.println("SMS Enviado!");
    cliente.flush();
    //Serial.println(data);

    msgDisparada[numSensor] = true;
  }
  cliente.stop();
}

void gravasms()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  server.send(200, "text/html", "ok");
  String Senha = server.arg("k");
  String Texto = server.arg("t");
  String Numeros = server.arg("n");

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

    Serial.println(Numeros);
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

void consultasms()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

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
      Serial.println("SMS: " + Texto);
    }

    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    server.send(200, "text/html", Texto + "|" + Numeros);
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
