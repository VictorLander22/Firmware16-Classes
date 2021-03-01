void ConfigurarWebServer(void)
{
  server.on("/", handleHtmlConfig);
  //server.on("/grava", grava);
  //server.on("/ler", ler);
  //server.on("/config", configuracao);
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
  server.on("/uploadfile", File_Upload);
  server.on(
      "/fupload", HTTP_POST, []() { server.send(200); }, handleFileUpload);
  server.on("/deletefile", File_Delete);
  //server.on("/cloud", cloud);
  //  server.on("/sendcloud", sendCloud);
  server.on("/testehtml", handleHtmlConfig);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/teste", mostarEEProm);

  server.onNotFound(handleNotFound);

  server.begin();

  (!DEBUG_ON) ?: Serial.println("HTTP server started");
}

// String ipStr(const IPAddress &ip)
// {
//   // Retorna IPAddress em formato "n.n.n.n"
//   String sFn = "";
//   for (byte bFn = 0; bFn < 3; bFn++)
//   {
//     sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
//   }
//   sFn += String(((ip >> 8 * 3)) & 0xFF);
//   return sFn;
// }.

void handleHtmlConfig()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  String defaultPage(FPSTR(webDefaultPage));
  defaultPage.replace("#ipfixo#", "true");
  defaultPage.replace("#utc#", String(DevSet.utcConfig));
  defaultPage.replace("#ssid#", DevSet.wifiSSID);
  defaultPage.replace("#pwd#", DevSet.wifiPwd);
  defaultPage.replace("#ip#", DevSet.numberToIpString(DevSet.wifiIP));
  defaultPage.replace("#msk#", DevSet.numberToIpString(DevSet.wifiMSK));
  defaultPage.replace("#gtw#", DevSet.numberToIpString(DevSet.wifiGTW));

  server.send_P(200, "text/html", defaultPage.c_str());
}

void about()
{
  server.send_P(200, "text/html", webAbout);
}

void reiniciar()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String restartPage(FPSTR(webRestart));
  restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
  server.send_P(200, "text/html", restartPage.c_str());
  delay(500);
  ESP.restart();
}

void gravawifi()
{

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  server.send(200, "text/html", F("<html>ok<meta charset='UTF-8'><script>history.back()</script></html>"));
  String wifiSSID = server.arg("txtnomerede");
  String wifiPWD = server.arg("txtsenha");
  const char *wifiIP = server.arg("txtip").c_str();
  const char *wifiMSK = server.arg("txtmascara").c_str();
  const char *wifiGTW = server.arg("txtgateway").c_str();
  bitWrite(DevSet.mode, 2, 0); //wifiPadrao
  DevSet.wifiSSID = wifiSSID;
  DevSet.wifiPwd = wifiPWD;
  DevSet.wifiIP = DevSet.ipStringToNumber(wifiIP);
  DevSet.wifiMSK = DevSet.ipStringToNumber(wifiMSK);
  DevSet.wifiGTW = DevSet.ipStringToNumber(wifiGTW);
  DevSet.setWifi();
  (!DEBUG_ON) ?: Serial.println(F("New WIFI Settings"));
  DevSet.showVariables();

  //gravahtml();
}

void redirectPage()
{
  server.send(200, "text/html", "<html>ok<meta charset='UTF-8'><script>location.replace(\"http://" + WiFi.localIP().toString() + "\")</script></html>");
}
