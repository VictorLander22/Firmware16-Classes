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
  //configuracao();
}

void wifireset()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ESP resetado");
  DevSet.factoryReset();
  ESP.restart();
}

//   String req = "1|";

//   f.println(req);
//   f.close();

//   // volta senha padrao do wifi
//   f = SPIFFS.open("/senhaap.txt", "w");

//   f.println("12345678|");
//   f.close();

//   // senha API padrao
//   f = SPIFFS.open("/apipass.txt", "w");
//   f.println("25d55ad283aa400af464c76d713c07ad|");
//   f.close();

//   // HTTP usuario
//   f = SPIFFS.open("/httpuser.txt", "w");
//   f.println("keepin|");
//   f.close();

//   // HTTP senha
//   f = SPIFFS.open("/httppass.txt", "w");
//   f.println("keepin|");
//   f.close();

//   SPIFFS.end();

//   ConfigEN(); // configura as entradas como normal

//   //ESP.restart();
// }

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
  if (!memRtc.getRtcBatStatus())
  {
    Rtc.year = 2000;
    Rtc.month = 1;
    Rtc.day = 1;
    Rtc.hour = 0;
    Rtc.minute = 0;
    Rtc.second = 0;
    Rtc.set_time();

    memRtc.setBateryMemStatus();
    memRtc.outValues = 255 << 8 | 255;
    memRtc.setOutputs();

    Serial.println(F("Set Date"));
    Rtc.get_time();
    Serial.printf("%02d/%02d/%04d %02d:%02d:%02d", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
    Serial.println();
  }

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
    Serial.printf("\nSet outputs ON: %d", memRtc.getOutputs());
    Serial.println();
    uint16_t outputs = memRtc.outValues;
    chip1.write8(outputs & 0xff);
    chip2.write8((outputs >> 8) & 0xff);
  }
  else
  {
    Serial.printf("\nSet outputs OFF");
    Serial.println();
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

  server.send(200, "text/html", "ok");

  if (server.arg("m") == "1")
  {
    bitWrite(DevSet.mode, 0, true);
  }
  else
  {
    bitWrite(DevSet.mode, 0, false);
  }
  DevSet.setMode();

  if bitRead (DevSet.mode, 0)
  {
    memRtc.outValues = chip2.read8() << 8 | chip1.read8();
    memRtc.setOutputs();
  }
  else
  {
    memRtc.outValues = 255 << 8 | 255;
    memRtc.setOutputs();
  }
}

void lerConfiguracao()
{
  // garante que o led do IR estará apagado
  IRsend irsend(16, true);
  irsend.begin();
  digitalWrite(16, LOW);

  vSenhaAP = DevSet.apWifiPwd;
  ApiPass = DevSet.apiPwd;
  AlowApi = bitRead(DevSet.mode, 0);
  usaCloud = bitRead(DevSet.mode, 1);
  vConfigWIFI = bitRead(DevSet.mode, 2);
  TipoMemoria = bitRead(DevSet.mode, 3);
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
      server.send(200, "text/html", "1");

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