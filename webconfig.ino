void ConfigurarWebServer(void)
{
  server.on("/", handleHtmlConfig);
  server.on("/exec", ExecuteFunction);
  server.on(
      "/fupload", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200); },
      onUpload);
  server.onNotFound(ExecuteFunction);
  server.begin();
  (!DEBUG_ON) ?: Serial.println("HTTP server started");
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
    String functionName = (gRequest->hasArg("fn")) ? gRequest->arg("fn") : gRequest->url();

    (!DEBUG_ON) ?: Serial.println(functionName);

    for (size_t i = 0; i < gRequest->args(); i++)
    {
      (!DEBUG_ON) ?: Serial.println("[" + (String)i + "] " + gRequest->getParam(i)->name() + " : " + gRequest->getParam(i)->value());
    }

    if (functionName == "espbackup")
      AsyncBackupEsp();
    else if (functionName == "esprestore")
      AsyncRestoreEsp();
    else if (functionName == "espformat")
      AsyncFormatEsp();
    else if (functionName == "filedir")
      FileDir();
    else if (functionName == "filedownload")
      FileDownload();
    else if (functionName == "filedelete")
      FileDelete();
    else if (functionName == "fileupload")
      FileUpload();
    else if (functionName == "api")
      api();
    else if (functionName == "chipmac")
      GetChipMac();
    else if (functionName == "/controle")
      controle();
    else if (functionName == "/gravarwifi")
      gravawifi();
    else if (functionName == "/gravasenhawifi")
      gravasenhawifi();
    else if (functionName == "/gravasenhahttp")
      gravasenhahttp();
    else if (functionName == "/situacao")
      situacao();
    else if (functionName == "/chipid")
      retornachip();
    else if (functionName == "/chamaddns")
      chamaddns();
    else if (functionName == "/consultaagenda")
      conagenda();
    else if (functionName == "/gravaragenda")
      gravaragenda();
    else if (functionName == "/atualizahora")
      atualizahora();
    else if (functionName == "/lersensores")
      lersensores();
    else if (functionName == "/consultasensor")
      consensor();
    else if (functionName == "/gravadevice")
      gravadevice();
    else if (functionName == "/buscadevice")
      buscadevice();
    else if (functionName == "/executeupdate")
      executeupdate();
    else if (functionName == "/executeupdatebeta")
      executeupdateBeta();
    else if (functionName == "/versao")
      versao();
    else if (functionName == "/link")
      linkversao();
    else if (functionName == "/link")
      linkversaoBeta();
    else if (functionName == "/ultimodisparo")
      ultimodisp();
    else if (functionName == "/buscaNotificar")
      buscaNotificar();
    else if (functionName == "/gravanot")
      gravanot();
    else if (functionName == "/gravasms")
      gravasms();
    else if (functionName == "/consultasms")
      consultasms();
    else if (functionName == "/wifi")
      valorwifi();
    else if (functionName == "/listawifi")
      WifiNetworkScan();
    else if (functionName == "/listawifi2")
      listawifi2();
    else if (functionName == "/getir")
      getIR();
    else if (functionName == "/habir")
      habir();
    else if (functionName == "/habrf")
      habRF();
    else if (functionName == "/getrf")
      getRF();
    else if (functionName == "/gravarf")
      gravarf();
    else if (functionName == "/ultimodisparorf")
      ultimodisprf();
    else if (functionName == "/sendrf")
      sendRFp();
    else if (functionName == "/modelo")
      fmodelo();
    else if (functionName == "/memoria")
      fMemoria();
    else if (functionName == "/apiativo")
      apiativo();
    else if (functionName == "/apiconfig")
      apiconfig();
    else if (functionName == "/alterasenhapi")
      alterasenhapi();
    else if (functionName == "/gravacena")
      gravacena();
    else if (functionName == "/log")
      readlog();
    else if (functionName == "/gravacloud")
      GravaCloud();
    else if (functionName == "/valida")
      valida();
    else if (functionName == "/reset")
      wifireset();
    else if (functionName == "/about")
      about();
    else if (functionName == "/gravasensor")
      AsyncSaveInputConfig();
    else if (functionName == "/reiniciar")
      reiniciar();
    else if (functionName == "/sendir")
      AsyncSendIR();
    else
      handleNotFound();
  }
}

void handleHtmlConfig(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String defaultPage(FPSTR(webDefaultPage));
  defaultPage.replace("#ipfixo#", "true");
  defaultPage.replace("#utc#", String(DevSet.utcConfig));
  defaultPage.replace("#ssid#", DevSet.wifiSSID);
  defaultPage.replace("#pwd#", DevSet.wifiPwd);
  defaultPage.replace("#ip#", DevSet.numberToIpString(DevSet.wifiIP));
  defaultPage.replace("#msk#", DevSet.numberToIpString(DevSet.wifiMSK));
  defaultPage.replace("#gtw#", DevSet.numberToIpString(DevSet.wifiGTW));

  request->send_P(200, "text/html", defaultPage.c_str());
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
  if (DEBUG_ON)
  {
    (!DEBUG_ON) ?: Serial.println(F("New WIFI Settings"));
    DevSet.showVariables();
  }
}

void FileDir()
{
  String arquivos = "";
  SPIFFS.begin();
  (!DEBUG_ON) ?: Serial.println(F("Consultar sistema de arquivos"));
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
    (!DEBUG_ON) ?: Serial.println(F("Arquivo existe"));
    gRequest->send(SPIFFS, path, String(), true);
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo não existe"));
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
    (!DEBUG_ON) ?: Serial.println(F("Arquivo existe"));
    if (SPIFFS.remove(path))
    {
      (!DEBUG_ON) ?: Serial.println(F("Removido"));
      gRequest->send(200, "text/html", F("Removido"));
    }
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo não existe"));
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
    (!DEBUG_ON) ?: Serial.printf("UploadStart: %s\n", filename.c_str());
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
    (!DEBUG_ON) ?: Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      request->send(200, "text/html", F("<h3>File was successfully uploaded</h3>"));
      //  SPIFFS.end();
    }

    SPIFFS.end();
  }
}

void AsyncBackupEsp()
{
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
      (!DEBUG_ON) ?: Serial.println(path);
      http.setTimeout(2000);
      http.setReuse(true);

      File f = SPIFFS.open(path, "r");
      http.begin(cliente, uri);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("dirname", gchipId);
      http.addHeader("filename", path);

      int httpCode = http.sendRequest("POST", &f, f.size());

      (!DEBUG_ON) ?: Serial.println(uri);
      (!DEBUG_ON) ?: Serial.println(httpCode);

      if (httpCode >= 200 && httpCode < 300)
      {
        String payload = http.getString();
        (!DEBUG_ON) ?: Serial.println(payload);
      }

      http.end();
      f.close();
    }
  }
  SPIFFS.end();
}

void AsyncRestoreEsp()
{
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

  (!DEBUG_ON) ?: Serial.println(payload);
  (!DEBUG_ON) ?: Serial.println(payload.length());

  DynamicJsonDocument dirlist(payload.length() * 2);
  auto error = deserializeJson(dirlist, payload);

  if (error)
  {
    (!DEBUG_ON) ?: Serial.print(F("deserializeJson() failed with code "));
    (!DEBUG_ON) ?: Serial.println(error.c_str());
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
        (!DEBUG_ON) ?: Serial.println("/" + file);
        payload = http.getString(); //Get the request response payload
        (!DEBUG_ON) ?: Serial.println(payload);
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

void AsyncFormatEsp()
{
  gRequest->send(200, "text/html", F("Formating..."));

  (!DEBUG_ON) ?: Serial.println(F("Formating"));
  //LittleFS.format();
  SPIFFS.format();
  (!DEBUG_ON) ?: Serial.println(F("Format SUCCESS!"));
}