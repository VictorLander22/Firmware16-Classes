
void convertConfig()
{ //converter configurações antigas para salvar na EEPROM
  File f;
  String ret;

  SPIFFS.begin();

  if (SPIFFS.exists("/index.html"))
  {

    if (SPIFFS.exists("/wifipadrao.txt"))
    {
      f = SPIFFS.open("/wifipadrao.txt", "r");
      ret = f.readStringUntil('|');
      bool wifipadrao = (ret == "0") ? false : true;
      f.close();
      SPIFFS.remove("/wifipadrao.txt");
      bitWrite(DevSet.mode, 2, wifipadrao); //b0:AllowApi, b1:UsaCloud, b2:wifiPadrao, b3:TipoMemoria ..
    }

    if (SPIFFS.exists("/ssid.txt"))
    {
      f = SPIFFS.open("/ssid.txt", "r");
      ret = f.readStringUntil('|');
      f.close();
      SPIFFS.remove("/ssid.txt");
      DevSet.wifiSSID = ret; //Limit 35 bytes;
    }

    if (SPIFFS.exists("/pass.txt"))
    {
      f = SPIFFS.open("/pass.txt", "r");
      ret = f.readStringUntil('|');
      f.close();
      SPIFFS.remove("/pass.txt");
      DevSet.wifiPwd = ret; //Limit 35 bytes;
    }

    if (SPIFFS.exists("/ip.txt"))
    {
      f = SPIFFS.open("/ip.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/ip.txt");
      DevSet.wifiIP = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/mask.txt"))
    {
      f = SPIFFS.open("/mask.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/mask.txt");
      DevSet.wifiMSK = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/gateway.txt"))
    {
      f = SPIFFS.open("/gateway.txt", "r");
      ret = f.readStringUntil('|');
      ret.replace(",", ".");
      f.close();
      SPIFFS.remove("/gateway.txt");
      DevSet.wifiGTW = DevSet.ipStringToNumber(ret.c_str());
    }

    if (SPIFFS.exists("/httpuser.txt"))
    {
      SPIFFS.remove("/httpuser.txt");
      DevSet.httpUser = "keepin";
    }

    if (SPIFFS.exists("/httppass.txt"))
    {
      SPIFFS.remove("/httppass.txt");
      DevSet.httpPwd = "keepin";
    }

    if (SPIFFS.exists("/apipass.txt"))
      SPIFFS.remove("/apipass.txt");

    if (SPIFFS.exists("/alowapi.txt"))
      SPIFFS.remove("/alowapi.txt");

    if (SPIFFS.exists("/senhaap.txt"))
      SPIFFS.remove("/senhaap.txt");

    if (SPIFFS.exists("/cloud.txt"))
      SPIFFS.remove("/cloud.txt");

    DevSet.setMode();
    DevSet.setWifi();
    DevSet.setApWifiPwd();
    DevSet.setApiPwd();
    DevSet.setHttpSeg();

    SPIFFS.remove("/index.html");
  }

  SPIFFS.end();
}

void wifireset()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ESP resetado");
  DevSet.factoryReset();
  ESP.restart();
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

    (!DEBUG_ON) ?: Serial.println(F("Set Date"));
    Rtc.get_time();
    (!DEBUG_ON) ?: Serial.printf("%02d/%02d/%04d %02d:%02d:%02d", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
    (!DEBUG_ON) ?: Serial.println();
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

    (!DEBUG_ON) ?: Serial.printf("\nSet outputs ON: %d", memRtc.getOutputs());
    (!DEBUG_ON) ?: Serial.println();
    uint16_t outputs = memRtc.getOutputs();
    chip1.write8(outputs & 0xff);
    chip2.write8((outputs >> 8) & 0xff);
  }
  else
  {
    (!DEBUG_ON) ?: Serial.printf("\nSet outputs OFF");
    (!DEBUG_ON) ?: Serial.println();
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

  if (server.arg("s") == Senha)
  {
    if (server.arg("f") == "w")
    {
      server.send(200, "text/html", "1");

      usaCloud = (server.arg("v") == "1") ? true : false;

      bitWrite(DevSet.mode, 1, usaCloud);
      DevSet.setMode();
      DevSet.showVariables();
    }
    else
    {
      server.send(200, "text/html", (usaCloud) ? "1" : "0");
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
  (!DEBUG_ON) ?: Serial.println("Consultar sistema de arquivos");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    arquivos += dir.fileName();
    //(!DEBUG_ON) ?:   Serial.print(dir.fileName());
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      arquivos += f.size();
      //(!DEBUG_ON) ?:   Serial.println(f.size());
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
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String path = server.arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    (!DEBUG_ON) ?: Serial.println("Arquivo existe");

    File download = SPIFFS.open(path, "r");

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
    (!DEBUG_ON) ?: Serial.println("Arquivo não existe");
  }
  SPIFFS.end();
}

void File_Upload()
{
  (!DEBUG_ON) ?: Serial.println("File upload stage-1");
  String webfile = "<h3>Select File to Upload</h3>";
  webfile += "<FORM action='/fupload' method='post' enctype='multipart/form-data'>";
  webfile += "<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>";
  webfile += "<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>";
  webfile += "<a href='/'>[Back]</a><br><br>";
  (!DEBUG_ON) ?: Serial.println("File upload stage-2");
  server.send(200, "text/html", webfile);
}

void handleFileUpload()
{ // upload a new file to the Filing system

  (!DEBUG_ON) ?: Serial.println("File upload stage-3");
  HTTPUpload &uploadfile = server.upload();

  if (uploadfile.status == UPLOAD_FILE_START)
  {
    (!DEBUG_ON) ?: Serial.println("File upload stage-4");
    String filename = uploadfile.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    (!DEBUG_ON) ?: Serial.print("Upload File Name: ");
    (!DEBUG_ON) ?: Serial.println(filename);

    SPIFFS.begin();

    SPIFFS.remove(filename); // Remove a previous version, otherwise data is appended the file again

    UploadFile = SPIFFS.open(filename, "a"); // Open the file for writing in SPIFFS (create it, if doesn't exist)
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    (!DEBUG_ON) ?: Serial.println("File upload stage-5");
    if (UploadFile)
    {
      UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
    }
  }
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      (!DEBUG_ON) ?: Serial.print("Upload Size: ");
      (!DEBUG_ON) ?: Serial.println(uploadfile.totalSize);

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
    (!DEBUG_ON) ?: Serial.println(uploadfile.totalSize);
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
    (!DEBUG_ON) ?: Serial.println("Arquivo existe");
    if (SPIFFS.remove(path))
    {
      (!DEBUG_ON) ?: Serial.println("Removido");
      server.send(200, "text/html", "Removido");
    }
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println("Arquivo não existe");
    server.send(200, "text/html", "Não existe");
  }
  SPIFFS.end();
}
