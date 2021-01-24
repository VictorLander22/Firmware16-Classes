void reiniciar()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");
  delay(1000);
  ESP.restart();
}

String wifiPadrao()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "r");
  String texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  Serial.println("wifipadrao: " + texto);

  return texto;
}

String pegaSSID()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaPassword()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/pass.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaIP()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/ip.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaMask()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/mask.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaGateway()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/gateway.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

void setWifiPadrao(String valor)
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  f.println(valor + "|");

  f.close();
  SPIFFS.end();
}

void configuracao2()
{
  /*
  String ssid = "";
  String pass = "";
  String ip = "";
  String mask = "";
  String gateway = "";*/

  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "r");
  String ssid = f.readStringUntil('|');
  f.close();

  f = SPIFFS.open("/pass.txt", "r");
  String pass = f.readStringUntil('|');
  f.close();

  f = SPIFFS.open("/ip.txt", "r");
  String ip = f.readStringUntil('|');
  ip.replace(",", ".");
  f.close();

  f = SPIFFS.open("/mask.txt", "r");
  String mask = f.readStringUntil('|');
  mask.replace(",", ".");
  f.close();

  f = SPIFFS.open("/gateway.txt", "r");
  String gateway = f.readStringUntil('|');
  gateway.replace(",", ".");
  f.close();

  SPIFFS.end();
  configuracao();
}

void wifireset()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ESP resetado");
  wifireset2();
}

void wifireset2()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  String req = "1|";

  f.println(req);
  f.close();

  // volta senha padrao do wifi
  f = SPIFFS.open("/senhaap.txt", "w");

  f.println("12345678|");
  f.close();

  // senha API padrao
  f = SPIFFS.open("/apipass.txt", "w");
  f.println("25d55ad283aa400af464c76d713c07ad|");
  f.close();

  // HTTP usuario
  f = SPIFFS.open("/httpuser.txt", "w");
  f.println("keepin|");
  f.close();

  // HTTP senha
  f = SPIFFS.open("/httppass.txt", "w");
  f.println("keepin|");
  f.close();

  SPIFFS.end();

  ConfigEN(); // configura as entradas como normal

  ESP.restart();
}

void gravawifi()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //SSID
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/ssid.txt", "w");
  }

  String req = server.arg("txtnomerede");

  f.println(req + "|");
  f.close();

  //SENHA
  f = SPIFFS.open("/pass.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/pass.txt", "w");
  }

  req = server.arg("txtsenha");

  f.println(req + "|");
  f.close();

  // IP

  f = SPIFFS.open("/ip.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/ip.txt", "w");
  }

  req = server.arg("txtip");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  //Mask

  f = SPIFFS.open("/mask.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/mask.txt", "w");
  }

  req = server.arg("txtmascara");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  //gateway

  f = SPIFFS.open("/gateway.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/gateway.txt", "w");
  }

  req = server.arg("txtgateway");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  ///fim

  f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  req = "0|";

  f.println(req + "|");
  f.close();

  SPIFFS.end();

  String conRetorno = server.arg("ret");

  if (conRetorno == "1")
  {
    server.send(200, "text/html", "ok");
    gravahtml();
  }
  else
  {
    gravahtml();
    configuracao();
  }
}

int retornaPorta(int vporta)
{
  if ((vporta >= 1) && (vporta <= 16))
  {
    return vporta - 1;
  }
  else
  {
    return -1;
  }
}

void IniciaRTC()
{
  // rtc

  Serial.println();

  RtcDateTime now;

  now = carregaHora();
}

RtcDateTime carregaHora()
{
  Rtc.get_time();

  RtcDateTime dt2(Rtc.year, Rtc.month, Rtc.day, Rtc.hour, Rtc.minute, Rtc.second);

  return dt2;
}

void valorwifi()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  int32_t rssi;
  rssi = WiFi.RSSI();

  server.send(200, "text/html", String(rssi));
}

void fmodelo()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "2");
}

String lerMemoria()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "r");
  String texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  return texto;
}

void Memoria()
{

  String retorno = "1"; //lerMemoria();
  if (retorno == "1")
  {

    Rtc.get_chip1();
    Rtc.get_chip2();

    chip1.write8(Rtc.chip1);
    chip2.write8(Rtc.chip2);

    //    Serial.println("Chip1: ");
    //    Serial.println(Rtc.chip1);
    //    Serial.println("Chip2: ");
    //    Serial.println(Rtc.chip2);
  }
  else
  {
    chip1.write8(255);
    chip2.write8(255);
  }
}

void fMemoria()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String valorm = server.arg("m");
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "w");
  f.println(valorm + "|");
  f.close();
  TipoMemoria = "1"; //lerMemoria();

  if (TipoMemoria == "1")
  {
    /*
    f = SPIFFS.open("/dm1.txt", "w");
    f.println(String(chip1.read8())+"|");
    f.close();

    f = SPIFFS.open("/dm2.txt", "w");
    f.println(String(chip2.read8())+"|");
    f.close();
    */
    Rtc.chip1 = String(chip1.read8()).toInt();
    Rtc.chip2 = String(chip2.read8()).toInt();
    ;
    Rtc.set_chip1();
    Rtc.set_chip2();
  }
  else
  {
    Rtc.chip1 = 255;
    Rtc.chip2 = 255;
    Rtc.set_chip1();
    Rtc.set_chip2();
  }

  SPIFFS.end();
  server.send(200, "text/html", "ok");
}

void lerConfiguracao()
{
  // garante que o led do IR estará apagado
  IRsend irsend(16, true);
  irsend.begin();
  digitalWrite(16, LOW);

  // pega o tempo registrado
  SPIFFS.begin();

  //senha padrao wifi (Access Point)
  File f = SPIFFS.open("/senhaap.txt", "r");
  String texto2 = f.readStringUntil('|');
  f.close();
  vSenhaAP = texto2;

  if (vSenhaAP == "")
  {
    vSenhaAP = "12345678";

    f = SPIFFS.open("/senhaap.txt", "w");

    f.println(vSenhaAP + "|");
    f.close();
  }

  // Flag API
  f = SPIFFS.open("/alowapi.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "1")
  {
    AlowApi = true;
  }
  else if (texto2 == "0")
  {
    AlowApi = false;
  }
  else
  {
    AlowApi = false;
    f = SPIFFS.open("/alowapi.txt", "w");
    f.println("1|");
    f.close();
  }

  //SenhaAPI
  f = SPIFFS.open("/apipass.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "")
  {
    ApiPass = "25d55ad283aa400af464c76d713c07ad";
    f = SPIFFS.open("/apipass.txt", "w");
    f.println("25d55ad283aa400af464c76d713c07ad|");
    f.close();
  }
  else
  {
    ApiPass = texto2;
  }

  // Cloud
  f = SPIFFS.open("/cloud.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "")
  {
    usaCloud = false;
    f = SPIFFS.open("/cloud.txt", "w");
    f.println("0|");
    f.close();
  }
  else if (texto2 == "1")
  {
    usaCloud = true;
  }
  else
  {
    usaCloud = false;
  }

  SPIFFS.end();
  //AlowApi = true;
}

void GravaCloud()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String func = server.arg("f");

  if (vSenha == Senha)
  {
    String req = server.arg("v");
    if (func == "w")
    {
      SPIFFS.begin();

      File f = SPIFFS.open("/cloud.txt", "w");
      f.println(req + "|");
      f.close();
      SPIFFS.end();

      if (req == "1")
      {
        usaCloud = true;
      }
      else
      {
        usaCloud = false;
      }
      server.send(200, "text/html", "1");
    }
    else if (func == "r")
    {
      if (usaCloud == true)
      {
        server.send(200, "text/html", "1");
      }
      else
      {
        server.send(200, "text/html", "0");
      }
    }
    else
    {
      server.send(200, "text/html", "1");
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void dirarquivos()
{
  String arquivos = "";
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  SPIFFS.begin();
  Serial.println("Consultar sistema de arquivos");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    arquivos += dir.fileName();
    //Serial.print(dir.fileName());
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      arquivos += f.size();
      //Serial.println(f.size());
      f.close();
    }
    arquivos += "<BR>";
  }
  SPIFFS.end();

  arquivos += "*";

  server.send(200, "text/html", arquivos);
}

/*
void ConfigAuth() 
{
  
  SPIFFS.begin();
  
  //Senha HTTP
  File f = SPIFFS.open("/httppass.txt", "r");
  String texto2 = "";
  texto2 = f.readStringUntil('|');
  f.close();
   
  //const char* www_password2;

  if (texto2 == "" || texto2 == " ") 
  {
    www_password2 = "keepin";
    f = SPIFFS.open("/httppass.txt", "w");
    f.println("keepin|");
    f.close();         
  }
  else{
    www_password2 = texto2;
  }
  

  // Usuario HTTP
  f = SPIFFS.open("/httpuser.txt", "r");
  texto2 = "";
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "" || texto2 == " ") 
  {  
    www_username2 = "keepin";
    f = SPIFFS.open("/httpuser.txt", "w");
    f.println("keepin|");
    f.close();         
  }
  else{
    www_username2 = texto2;
  }

  Serial.println("username: " + String(www_username2));
  Serial.println("password: " + String(www_password2));
 
  SPIFFS.end();    

}
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Download()
{ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String path = server.arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  //String path = "/httpuser.txt";
  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    Serial.println("Arquivo existe");

    File download = SPIFFS.open(path, "r");
    //Serial.println(download);
    //size_t sent = server.streamFile(file, "text/html");
    //file.close();

    if (download)
    {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename=" + path);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    }
  }
  else
  {
    Serial.println("Arquivo não existe");
  }
  SPIFFS.end();
}

void File_Upload()
{
  Serial.println("File upload stage-1");
  //append_page_header();
  String webfile = "<h3>Select File to Upload</h3>";
  webfile += "<FORM action='/fupload' method='post' enctype='multipart/form-data'>";
  webfile += "<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>";
  webfile += "<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>";
  webfile += "<a href='/'>[Back]</a><br><br>";
  //append_page_footer();
  Serial.println("File upload stage-2");
  server.send(200, "text/html", webfile);
}

void handleFileUpload()
{ // upload a new file to the Filing system

  Serial.println("File upload stage-3");
  HTTPUpload &uploadfile = server.upload();

  if (uploadfile.status == UPLOAD_FILE_START)
  {
    Serial.println("File upload stage-4");
    String filename = uploadfile.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    Serial.print("Upload File Name: ");
    Serial.println(filename);

    SPIFFS.begin();

    SPIFFS.remove(filename); // Remove a previous version, otherwise data is appended the file again

    UploadFile = SPIFFS.open(filename, "a"); // Open the file for writing in SPIFFS (create it, if doesn't exist)
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    Serial.println("File upload stage-5");
    if (UploadFile)
    {
      //SPIFFS.begin();

      UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
    }
  }
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      Serial.print("Upload Size: ");
      Serial.println(uploadfile.totalSize);

      //append_page_header();
      String webfile = "<h3>File was successfully uploaded</h3>";
      webfile += "<h2>Uploaded File Name: ";
      webfile += uploadfile.filename + "</h2>";
      webfile += "<h2>File Size: OK";
      //webfile += uploadfile.totalSize + "</h2><br>";
      //append_page_footer();
      server.send(200, "text/html", webfile);
      //
      SPIFFS.end();
    }
  }
  else
  {
    Serial.println(uploadfile.totalSize);
    SPIFFS.end();
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void File_Delete()
{ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String path = server.arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    Serial.println("Arquivo existe");
    if (SPIFFS.remove(path))
    {
      Serial.println("Removido");
      server.send(200, "text/html", "Removido");
    }
  }
  else
  {
    Serial.println("Arquivo não existe");
    server.send(200, "text/html", "Não existe");
  }
  SPIFFS.end();
}