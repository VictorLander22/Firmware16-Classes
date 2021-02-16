void configuracao() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();

//  WiFiClient client = server.client();

/*
*/
  SPIFFS.begin();
  //SPIFFS.remove("/index.html");
  File f = SPIFFS.open("/index.html", "r");
  //Serial.println(f.readStringUntil('#fimarq#'));

  if (f.size() < 100)
  {
    SPIFFS.end();
    gravahtml();
  }

  server.streamFile(f, "text/html");
//  server.send(200, "text/html", String(f.size()));

  f.close();
  SPIFFS.end();
}

String htmlconfig1 ()
{
  String buf = "<!DOCTYPE html>";
  buf += "<html lang=\"pt-br\">";
  buf += "<head>";
  buf += "    <meta charset=\"utf-8\">";
  buf += "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
  buf += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  buf += "    <title>KeepIN</title>";
  buf += "    <style type=\"text/css\">";
  return buf;
}

String htmlconfig2()
{
  String buf = "	</style>";
  buf += "    <style type=\"text/css\">";
  return buf;
}

String htmlconfig3()
{

  String buf = "	</style>";
  buf += "</head>";
  buf += "<body>";
  buf += "    <center>";
  buf += "        <h3><strong>Configuração de Rede </strong></h3>";
  buf += "    </center>";
  buf += "    <div class=\"container\">";
  buf += "        <div class=\"row\">";
  buf += "            <div class=\"one column\"></div>";
  buf += "            <div class=\"ten columns\">";
//  buf += "                <h5><strong>Informe as configurações de rede</strong></h5>";
  buf += "                <form action=\"gravarwifi\" method=\"GET\">";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtnomerede\">Nome da rede wifi:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtnomerede\" name=\"txtnomerede\" value=\""+vssid+"\" placeholder=\"Nome da rede (SSID)\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtsenha\">Senha da rede wifi:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtsenha\" name=\"txtsenha\" value=\""+vpass+"\" placeholder=\"Senha da rede wifi\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtip\">Ip:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtip\" name=\"txtip\" value=\""+vip+"\" placeholder=\"Endereço de IP\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtmascara\">Mascara de sub-rede:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtmascara\" name=\"txtmascara\" value=\""+vmask+"\" placeholder=\"Mascara da sub-rede\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtgateway\">Gateway:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtgateway\" name=\"txtgateway\" value=\""+vgateway+"\" placeholder=\"Endereço do Gateway\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <button type=\"submit\" class=\"button-primary u-full-width\">Gravar</button>";
  buf += "                <a href=\"reiniciar\" class=\"button btnorange u-full-width\">Reiniciar</a>";
  buf += "                <a href=\"reset\" class=\"button btnred u-full-width\">Cfg. Fábrica</a>";
  buf += "                <a href=\"executeupdate?s=kdi9e\" class=\"button btnred u-full-width\">Atualizar Firmware</a>";
  buf += "            </div>";
  buf += "        </div>";
  buf += "        </form>";
  buf += "        <div class=\"one column\"></div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "</body>";
  buf += "</html>"; 

  return buf;
}

void gravahtml()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  //if(!server.authenticate(www_username, www_password))
    //return server.requestAuthentication();
  
  carregaDadosHTML();

  SPIFFS.begin();
  File f = SPIFFS.open("/index.html", "w");
   
  if (!f) {
    SPIFFS .format();
    File f = SPIFFS.open("/index.html", "w");
  }


  f.println(htmlconfig1());
  f.println(vNormalize());
  f.println(htmlconfig2());
  f.println(vskeleton());
  f.println(vskeleton2());
  f.println(vskeleton3());
  f.println(vskeleton4());
  f.println(vskeleton5());
  f.println(htmlconfig3());  
  //f.println("#fimarq#");
  f.close();
  SPIFFS.end();
}

void testes2()
{
  carregaDadosHTML();
  server.send(200, "text/html", htmlconfig3());  
}

void carregaDadosHTML()
{
  DevSet.getDeviceSettings();
  vssid = DevSet.wifiSSID;
  vpass = DevSet.wifiPwd;
  vip = DevSet.numberToIpString(DevSet.wifiIP);
  vmask = DevSet.numberToIpString(DevSet.wifiMSK);
  vgateway = DevSet.numberToIpString(DevSet.wifiGTW);
}