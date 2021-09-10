// void sendSMS(int numSensor)
// {
//   WiFiClient cliente;
//   String Texto = "";
//   String Numeros = "";

//   SPIFFS.begin();
//   File fTexto = SPIFFS.open("/sms_text.txt", "r");
//   if (fTexto)
//     Texto = fTexto.readStringUntil('*');
//   fTexto.close();

//   fTexto = SPIFFS.open("/sms_numb.txt", "r");
//   if (fTexto)
//     Numeros = fTexto.readStringUntil('*');
//   fTexto.close();
//   SPIFFS.end();

//   if (Numeros.length() > 8 && Texto.length() > 4)
//   {
//     String data = "user=fcleal&password=201277&destinatario=" + Numeros + "&msg=" + Texto;

//     slogln("envia sms");
//     slogln(data);

//     slogln("Send data...");
//     if (cliente.connect("www.facilitamovel.com.br", 80))
//     {
//       slogln("Connected to the server..");
//       cliente.println("POST /api/multipleSend.ft HTTP/1.1");
//       //        cliente.println("Authorization: key=" + serve + "");
//       cliente.println("Content-Type: application/x-www-form-urlencoded");
//       cliente.println("Host: www.facilitamovel.com.br");
//       cliente.print("Content-Length: ");
//       cliente.println(data.length());
//       cliente.println();
//       cliente.println(data);
//     }
//     slogln("Enviado sms...Aguardando confirmacao..");
//     while (cliente.available())
//     {
//       char c = cliente.read();
//       //        (!DEBUG_ON) ?:   Serial.print(c);
//     }
//     slogln("SMS Enviado!");
//     cliente.flush();
//     //(!DEBUG_ON) ?:   Serial.println(data);

//     msgDisparada[numSensor] = true;
//   }
//   cliente.stop();
// }

// void gravasms()
// {

//   gRequest->send(200, sdefTextHtml, sdefOK);
//   String Senha = gRequest->arg("k");
//   String Texto = gRequest->arg("t");
//   String Numeros = gRequest->arg("n");

//   if (Senha == "kdi9e")
//   {
//     SPIFFS.begin();
//     File fTexto = SPIFFS.open("/sms_text.txt", "w");
//     // if (!fTexto)
//     // {
//     //   SPIFFS.format();
//     //   File fTexto = SPIFFS.open("/sms_text.txt", "w");
//     // }
//     fTexto.println(Texto);
//     fTexto.close();

//     fTexto = SPIFFS.open("/sms_numb.txt", "w");
//     // if (!fTexto)
//     // {
//     //   SPIFFS.format();
//     //   fTexto = SPIFFS.open("/sms_numb.txt", "w");
//     // }
//     fTexto.println(Numeros);
//     fTexto.close();
//     SPIFFS.end();

//     slogln(Numeros);
//   }
// }

// void gravasms2(String Texto, String Numeros)
// {
//   SPIFFS.begin();
//   File fTexto = SPIFFS.open("/sms_text.txt", "w");
//   // if (!fTexto)
//   // {
//   //   SPIFFS.format();
//   //   File fTexto = SPIFFS.open("/sms_text.txt", "w");
//   // }
//   fTexto.println(Texto);
//   fTexto.close();

//   fTexto = SPIFFS.open("/sms_numb.txt", "w");
//   // if (!fTexto)
//   // {
//   //   SPIFFS.format();
//   //   fTexto = SPIFFS.open("/sms_numb.txt", "w");
//   // }
//   fTexto.println(Numeros);
//   fTexto.close();
//   SPIFFS.end();
// }

// void consultasms()
// {
//   String Texto = "";
//   String Numeros = "";
//   String Senha = gRequest->arg("k");

//   if (Senha == "kdi9e")
//   {
//     SPIFFS.begin();

//     File fTexto = SPIFFS.open("/sms_text.txt", "r");
//     if (fTexto)
//       Texto = fTexto.readStringUntil('*');
//     fTexto.close();

//     fTexto = SPIFFS.open("/sms_numb.txt", "r");
//     if (fTexto)
//       Numeros = fTexto.readStringUntil('*');
//     fTexto.close();

//     SPIFFS.end();

//     if (Texto == "" && Numeros == "")
//     {
//       gravasms2("*", "*");
//       slogln("SMS: " + Texto);
//     }

//     gRequest->send(200, sdefTextHtml, Texto + "|" + Numeros);
//   }
// }

// String consultasms2()
// {
//   String Texto = "";
//   String Numeros = "";

//   SPIFFS.begin();

//   File fTexto = SPIFFS.open("/sms_text.txt", "r");
//   if (fTexto)
//     Texto = fTexto.readStringUntil('*');
//   fTexto.close();

//   fTexto = SPIFFS.open("/sms_numb.txt", "r");
//   if (fTexto)
//     Numeros = fTexto.readStringUntil('*');
//   fTexto.close();

//   SPIFFS.end();

//   return Texto + "|" + Numeros;
// }
