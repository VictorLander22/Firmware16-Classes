void ConfigurarWebServer(void)
{
  //server.on("/", handleHtmlConfig);
  // server.on(
  //     "/", HTTP_GET, [](request) {}, handleHtmlConfig);
  server.on("/", handleHtmlConfig);
  server.on("/gravarwifi", gravawifi);
  server.on("/gravasenhawifi", gravasenhawifi);
  server.on("/gravasenhahttp", gravasenhahttp);
  server.on("/reset", wifireset);
  server.on("/reiniciar", reiniciar);
  server.on("/valida", valida);
  server.on("/controle", controle);
  server.on("/situacao", situacao);
  server.on("/chipid", retornachip);
  server.on("/chipmac", RetornaChipMac);
  server.on("/chamaddns", chamaddns);
  //server.on("/mesh", mesh);
  //server.on("/consultamesh", meshconsulta);
  server.on("/consultaagenda", conagenda);
  server.on("/gravaragenda", gravaragenda);
  server.on("/atualizahora", atualizahora);
  server.on("/lersensores", lersensores);
  server.on("/gravasensor", gravasensor);
  server.on("/consultasensor", consensor);
  server.on("/gravadevice", gravadevice);
  server.on("/buscadevice", buscadevice);
  server.on("/executeupdate", executeupdate);
  server.on("/executeupdatebeta", executeupdateBeta);
  server.on("/versao", versao);
  server.on("/link", linkversao);
  server.on("/link", linkversaoBeta);
  //server.on("/limpadevice", limpadevice);
  server.on("/ultimodisparo", ultimodisp);
  server.on("/buscaNotificar", buscaNotificar);
  server.on("/gravanot", gravanot);
  server.on("/gravasms", gravasms);
  server.on("/consultasms", consultasms);
  server.on("/wifi", valorwifi);
  server.on("/listawifi", WifiNetworkScan);
  server.on("/listawifi2", listawifi2);
  //IR
  server.on("/getir", getIR);
  server.on("/sendir", sendir);
  server.on("/habir", habir);
  //RF
  server.on("/habrf", habRF);
  server.on("/getrf", getRF);
  server.on("/gravarf", gravarf);
  server.on("/ultimodisparorf", ultimodisprf);
  server.on("/sendrf", sendRFp);
  server.on("/modelo", fmodelo);
  server.on("/memoria", fMemoria);
  //server.on("/html", gravahtml);
  //server.on("/teste", testes2);
  server.on("/api", api);
  server.on("/apiativo", apiativo);
  server.on("/apiconfig", apiconfig);
  server.on("/alterasenhapi", alterasenhapi);
  server.on("/about", about);
  server.on("/gravacena", gravacena);
  server.on("/log", readlog);
  server.on("/gravacloud", GravaCloud);
  server.on("/dirarquivos", dirarquivos);
  server.on("/downloadfile", File_Download);
  server.on("/deletefile", File_Delete);
  server.on("/asyncRestart", asyncESPRestart);

  server.on("/uploadfile", File_Upload);
  server.on(
      "/fupload", HTTP_POST, [](AsyncWebServerRequest *request) { request->send(200); }, onUpload);

  server.on("/teste", mostarEEProm);
  server.onNotFound(handleNotFound);
  server.begin();

  (!DEBUG_ON) ?: Serial.println("HTTP server started");
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

void about(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", webAbout);
}

void reiniciar(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String restartPage(FPSTR(webRestart));
  restartPage.replace("#oldip#", WiFi.localIP().toString());
  restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
  Serial.println(restartPage);
  request->send_P(200, "text/html", restartPage.c_str());
  //request->send_P(200, "text/html", restartPage.c_str()); //, processor);
  //request->send(200, "text/html", "<html>ok<meta charset='UTF-8'><script>location.replace(\"http://" + DevSet.numberToIpString(DevSet.wifiIP) + "\")</script></html>");
  //ESP.restart();
}

void gravawifi(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
  request->send(200, "text/html", F("<html>ok<meta charset='UTF-8'><script>history.back()</script></html>"));
  String wifiSSID = request->arg("txtnomerede");
  String wifiPWD = request->arg("txtsenha");
  const char *wifiIP = request->arg("txtip").c_str();
  const char *wifiMSK = request->arg("txtmascara").c_str();
  const char *wifiGTW = request->arg("txtgateway").c_str();
  bitWrite(DevSet.mode, 2, 0); //wifiPadrao
  DevSet.wifiSSID = wifiSSID;
  DevSet.wifiPwd = wifiPWD;
  DevSet.wifiIP = DevSet.ipStringToNumber(wifiIP);
  DevSet.wifiMSK = DevSet.ipStringToNumber(wifiMSK);
  DevSet.wifiGTW = DevSet.ipStringToNumber(wifiGTW);
  DevSet.setWifi();
  (!DEBUG_ON) ?: Serial.println(F("New WIFI Settings"));
  DevSet.showVariables();
}

void redirectPage()
{
  request->send(200, "text/html", "<html>ok<meta charset='UTF-8'><script>location.replace(\"http://" + WiFi.localIP().toString() + "\")</script></html>");
}

void asyncESPRestart(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", "ok");
  ESP.restart();
}

void dirarquivos(AsyncWebServerRequest *request)
{
  String arquivos = "";
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
  SPIFFS.begin();
  (!DEBUG_ON) ?: Serial.println(F("Consultar sistema de arquivos"));
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

  request->send(200, "text/html", arquivos);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Download(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String path = request->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  bool fileExist = SPIFFS.exists(path);

  if (fileExist)
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo existe"));
    request->send(SPIFFS, path, String(), true);
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo não existe"));
    SPIFFS.end();
  }
}

void File_Upload(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", webUpload);
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
      SPIFFS.end();
    }
    else
    {
      //(!DEBUG_ON) ?: Serial.println(uploadfile.totalSize);
      SPIFFS.end();
    }
  }
}
// void handleFileUpload()
// { // upload a new file to the Filing system

//   (!DEBUG_ON) ?: Serial.println("File upload stage-3");
//   HTTPUpload &uploadfile = server.upload();

//   if (uploadfile.status == UPLOAD_FILE_START)
//   {
//     (!DEBUG_ON) ?: Serial.println("File upload stage-4");
//     String filename = uploadfile.filename;
//     if (!filename.startsWith("/"))
//       filename = "/" + filename;
//     (!DEBUG_ON) ?: Serial.print("Upload File Name: ");
//     (!DEBUG_ON) ?: Serial.println(filename);

//     SPIFFS.begin();

//     SPIFFS.remove(filename); // Remove a previous version, otherwise data is appended the file again

//     UploadFile = SPIFFS.open(filename, "a"); // Open the file for writing in SPIFFS (create it, if doesn't exist)
//   }
//   else if (uploadfile.status == UPLOAD_FILE_WRITE)
//   {
//     (!DEBUG_ON) ?: Serial.println("File upload stage-5");
//     if (UploadFile)
//     {
//       UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
//     }
//   }
//   else if (uploadfile.status == UPLOAD_FILE_END)
//   {
//     if (UploadFile) // If the file was successfully created
//     {
//       UploadFile.close(); // Close the file again
//       (!DEBUG_ON) ?: Serial.print("Upload Size: ");
//       (!DEBUG_ON) ?: Serial.println(uploadfile.totalSize);

//       //append_page_header();
//       String webfile = "<h3>File was successfully uploaded</h3>";
//       webfile += "<h2>Uploaded File Name: ";
//       webfile += uploadfile.filename + "</h2>";
//       webfile += "<h2>File Size: OK";
//       //webfile += uploadfile.totalSize + "</h2><br>";
//       //append_page_footer();
//       request->send(200, "text/html", webfile);
//       //
//       SPIFFS.end();
//     }
//   }
//   else
//   {
//     (!DEBUG_ON) ?: Serial.println(uploadfile.totalSize);
//     SPIFFS.end();
//   }
// }

void File_Delete(AsyncWebServerRequest *request)
{ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments

  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  String path = request->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo existe"));
    if (SPIFFS.remove(path))
    {
      (!DEBUG_ON) ?: Serial.println(F("Removido"));
      request->send(200, "text/html", F("Removido"));
    }
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println(F("Arquivo não existe"));
    request->send(200, "text/html", F("Não existe"));
  }
  SPIFFS.end();
}
