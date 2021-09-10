void ConfigurarWebServer(void)
{
  //server.on("/", handleHtmlConfig);
  server.onNotFound(ExecuteFunction);
  server.on(
      "/fupload", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200); },
      onUpload);
  server.begin();
  //slogln(F("HTTP server started"));
}

void ExecuteFunction(AsyncWebServerRequest *request)
{
  gRequest = request;
  asyncExecuteFunction = true;
}

void AsyncFunctions()
{
  if (asyncExecuteFunction)
  {
    asyncExecuteFunction = false;

    String functionName = gRequest->url();

    slogln(functionName);

    for (size_t i = 0; i < gRequest->args(); i++)
    {
      slogln("[" + (String)i + "] " + gRequest->getParam(i)->name() + " : " + gRequest->getParam(i)->value());
    }
    if (functionName == F("/"))
      handleHtmlConfig();
    else if (functionName == F("/espbackup"))
      AsyncBackupEsp(true);
    else if (functionName == F("/esprestore"))
      AsyncRestoreEsp(true);
    else if (functionName == F("/espformat"))
      AsyncFormatEsp(true);
    else if (functionName == F("/filedir"))
      FileDir();
    else if (functionName == F("/filedownload"))
      FileDownload();
    else if (functionName == F("/filedelete"))
      FileDelete();
    else if (functionName == F("/fileupload"))
      FileUpload();
    else if (functionName == F("/api"))
      api();
    else if (functionName == F("/chipmac"))
      GetChipMac();
    else if (functionName == F("/controle"))
      controle();
    else if (functionName == F("/gravarwifi"))
      gravawifi();
    else if (functionName == F("/gravasenhawifi"))
      gravasenhawifi();
    else if (functionName == F("/gravasenhahttp"))
      gravasenhahttp();
    else if (functionName == F("/situacao"))
      situacao();
    else if (functionName == F("/chipid"))
      retornachip();
    // else if (functionName == F("/chamaddns"))
    //   chamaddns();
    else if (functionName == F("/consultaagenda"))
      conagenda();
    else if (functionName == F("/gravaragenda"))
      gravaragenda();
    else if (functionName == F("/atualizahora"))
      atualizahora();
    else if (functionName == F("/lersensores"))
      lersensores();
    else if (functionName == F("/consultasensor"))
      consensor();
    else if (functionName == F("/gravadevice"))
      gravadevice();
    else if (functionName == F("/buscadevice"))
      buscadevice();
    else if (functionName == F("/executeupdate"))
      executeupdate();
    else if (functionName == F("/executeupdatebeta"))
      executeupdateBeta(true);
    else if (functionName == F("/versao"))
      versao();
    // else if (functionName == F("/link"))
    //   linkversao();
    // else if (functionName == F("/link"))
    //   linkversaoBeta();
    else if (functionName == F("/ultimodisparo"))
      ultimodisp();
    else if (functionName == F("/buscaNotificar"))
      buscaNotificar();
    else if (functionName == F("/gravanot"))
      gravanot();
    //sms else if (functionName == F("/gravasms"))
    //sms gravasms();
    //sms else if (functionName == F("/consultasms"))
    //sms consultasms();
    else if (functionName == F("/wifi"))
      valorwifi();
    else if (functionName == F("/listawifi"))
      WifiNetworkScan();
    // else if (functionName == F("/listawifi2"))
    //   listawifi2();
    else if (functionName == F("/getir"))
      getIR();
    else if (functionName == F("/habir"))
      habir();
    else if (functionName == F("/habrf"))
      habRF();
    else if (functionName == F("/getrf"))
      getRF();
    else if (functionName == F("/gravarf"))
      gravarf();
    else if (functionName == F("/ultimodisparorf"))
      ultimodisprf();
    else if (functionName == F("/sendrf"))
      sendRFp();
    else if (functionName == F("/modelo"))
      fmodelo();
    else if (functionName == F("/memoria"))
      fMemoria();
    else if (functionName == F("/apiativo"))
      apiativo();
    else if (functionName == F("/apiconfig"))
      apiconfig();
    else if (functionName == F("/alterasenhapi"))
      alterasenhapi();
    else if (functionName == F("/gravacena"))
      gravacena();
    // else if (functionName == F("/log"))
    //   readlog();
    else if (functionName == F("/gravacloud"))
      GravaCloud();
    else if (functionName == F("/valida"))
      valida();
    else if (functionName == F("/reset"))
      wifireset();
    else if (functionName == F("/about"))
      about();
    else if (functionName == F("/gravasensor"))
      AsyncSaveInputConfig();
    else if (functionName == F("/reiniciar"))
      reiniciar();
    else if (functionName == F("/sendir"))
      AsyncSendIR();
    else
      handleNotFound();
  }
}

void handleHtmlConfig()
{
  if (!gRequest->authenticate(DevSet.httpUser.c_str(), DevSet.httpPwd.c_str()))
    return gRequest->requestAuthentication();

  String defaultPage(FPSTR(webDefaultPage));
  defaultPage.replace("#ipfixo#", "true");
  defaultPage.replace("#utc#", String(DevSet.utcConfig));
  defaultPage.replace("#ssid#", DevSet.wifiSSID);
  defaultPage.replace("#pwd#", DevSet.wifiPwd);
  defaultPage.replace("#ip#", DevSet.numberToIpString(DevSet.wifiIP));
  defaultPage.replace("#msk#", DevSet.numberToIpString(DevSet.wifiMSK));
  defaultPage.replace("#gtw#", DevSet.numberToIpString(DevSet.wifiGTW));
  gRequest->send(200, "text/html", defaultPage.c_str());
}

void about()
{
  gRequest->send_P(200, "text/html", webAbout);
}

void reiniciar()
{
  String restartPage(FPSTR(webRestart));
  restartPage.replace("#oldip#", CurrentIP());
  restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
  gRequest->send_P(200, "text/html", restartPage.c_str());

  delay(300);
  ESP.restart();
}

void gravawifi()
{
  gRequest->send(200, "text/html", "<html>ok<meta charset='UTF-8'><script>location.replace(\"http://" + CurrentIP() + "\")</script></html>");

  String wifiSSID = gRequest->arg("txtnomerede");
  String wifiPWD = gRequest->arg("txtsenha");
  const char *wifiIP = gRequest->arg("txtip").c_str();
  const char *wifiMSK = gRequest->arg("txtmascara").c_str();
  const char *wifiGTW = gRequest->arg("txtgateway").c_str();
  bitWrite(DevSet.mode, 2, 0); //wifiPadrao
  DevSet.wifiSSID = wifiSSID;
  DevSet.wifiPwd = wifiPWD;
  DevSet.wifiIP = DevSet.ipStringToNumber(wifiIP);
  DevSet.wifiMSK = DevSet.ipStringToNumber(wifiMSK);
  DevSet.wifiGTW = DevSet.ipStringToNumber(wifiGTW);
  DevSet.setWifi();
  slogln(F("New WIFI Settings"));
  DevSet.showVariables();
}

void FileDir()
{
  String arquivos = "";
  SPIFFS.begin();
  slogln(F("Consultar sistema de arquivos"));
  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    arquivos += dir.fileName() + " ";
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      arquivos += f.size();
      f.close();
    }
    arquivos += "<BR>";
  }
  SPIFFS.end();

  arquivos += "*";

  gRequest->send(200, "text/html", arquivos);
}

void FileDownload()
{
  String path = gRequest->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  bool fileExist = SPIFFS.exists(path);

  if (fileExist)
  {
    slogln(F("Arquivo existe"));
    gRequest->send(SPIFFS, path, String(), true);
  }
  else
  {
    slogln(F("Arquivo não existe"));
    gRequest->send(200, "text/html", F("File not found"));
    SPIFFS.end();
  }
}

void FileDelete()
{
  String path = gRequest->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    slogln(F("Arquivo existe"));
    if (SPIFFS.remove(path))
    {
      slogln(F("Removido"));
      gRequest->send(200, "text/html", F("Removido"));
    }
  }
  else
  {
    slogln(F("Arquivo não existe"));
    gRequest->send(200, "text/html", F("File not found"));
  }
  SPIFFS.end();
}

void FileUpload()
{
  gRequest->send_P(200, "text/html", webUpload);
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    slogln("UploadStart: " + filename);
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    SPIFFS.begin();
    SPIFFS.remove(filename);                 // Remove a previous version, otherwise data is appended the file again
    UploadFile = SPIFFS.open(filename, "a"); // Open the file for writing in SPIFFS (create it, if doesn't exist)
  }

  //Serial.printf("%s", (const char *)data);
  UploadFile.write(data, len); // Write the received bytes to the file

  if (final)
  {
    slogln("UploadEnd: " + filename + "(" + index + len + ")");
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      request->send(200, "text/html", F("<h3>File was successfully uploaded</h3>"));
      //  SPIFFS.end();
    }

    SPIFFS.end();
  }
}

void AsyncBackupEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, "text/html", F("Backup"));

  WiFiClient cliente;
  HTTPClient http;
  String uri = cloudServer + "postfile";
  String path = "";

  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    path = dir.fileName();
    if (!path.startsWith("/"))
      path = "/" + path;
    bool fileExist = SPIFFS.exists(path);
    if (fileExist)
    {
      slogln(path);
      http.setTimeout(2000);
      http.setReuse(true);

      File f = SPIFFS.open(path, "r");
      http.begin(cliente, uri);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("dirname", gchipId);
      http.addHeader("filename", path);

      int httpCode = http.sendRequest("POST", &f, f.size());

      slogln(uri);
      slogln(httpCode);

      if (httpCode >= 200 && httpCode < 300)
      {
        String payload = http.getString();
        slogln(payload);
      }

      http.end();
      f.close();
    }
  }
  SPIFFS.end();
}

void AsyncRestoreEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, "text/html", F("Restoring"));

  HTTPClient http;
  WiFiClient client;
  //String payload;

  http.setTimeout(2000);
  http.setReuse(true);
  http.begin(client, cloudServer + "dir");

  http.addHeader("dirname", gchipId);
  int httpCode = http.GET();
  String payload = http.getString();
  http.end();

  slogln(payload);
  slogln(payload.length());

  DynamicJsonDocument dirlist(payload.length() * 2);
  auto error = deserializeJson(dirlist, payload);

  if (error)
  {
    slog(F("deserializeJson() failed with code "));
    slogln(error.c_str());
    return;
  }
  else
  {
    SPIFFS.begin();
    for (int i = 0; i < dirlist["file"].size(); i++)
    {
      //Serial.println("AsyncRestoreEsp 1");
      const String file = dirlist["file"][i];
      //Serial.println("/" + dir);

      http.setTimeout(2000);
      http.setReuse(true);

      http.begin(client, cloudServer + "download");
      http.addHeader("dirname", gchipId);
      http.addHeader("filename", file);

      httpCode = http.GET();

      if (httpCode == 200)
      {
        slogln("/" + file);
        payload = http.getString(); //Get the request response payload
        slogln(payload);
        File f = SPIFFS.open("/" + file, "w");
        if (f)
        {
          f.print(payload);
        }
        f.close();
      }
      http.end();
    }
    SPIFFS.end();
    delay(100);
  }
}

void AsyncFormatEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, "text/html", F("Formating..."));

  slogln(F("Formating"));
  //LittleFS.format();
  SPIFFS.format();
  slogln(F("Format SUCCESS!"));
}