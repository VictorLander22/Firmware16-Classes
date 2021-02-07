/*
void mesh() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
//if(!server.authenticate(www_username, www_password))
    //return server.requestAuthentication();
  
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();

    String sIP = server.arg("ip");
    String Senha = server.arg("s");
    int Porta = server.arg("p").toInt();
    String funcao = server.arg("f");
    int IdChip = server.arg("c").toInt();
    String Tipo = server.arg("t");
    IPAddress Destino;



  server.send(200, "text/plain", String("LED is now "));
  WiFi.disconnect(); 
  conectarmesh();
      
  Serial.println("");
  if ((state != 1) and (state != 0)){
    /* Scan for other nodes and send them a message */
/*
    char request[60];
    char MeshNode[60];

    sprintf(MeshNode, "Mesh_Node%d", state);
    
    Serial.println("Entrou...");  
    Serial.print("Tentdo encontrar ");
    Serial.println(MeshNode);
    server.send(200, "text/plain", String("LED is now ") + ((state)));


    
    String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip)+"|"+Tipo+"|"+Senha+"|";
    
    sprintf(request, Texto.c_str(), request_i++, ESP.getChipId());
    
    //mesh_node.attemptScan(request);
    mesh_node.attemptScanNodeSend(MeshNode,request);
    delay(1000);
    
    Serial.println("Saiu..");
        
    Serial.println(WiFi.status());
    //if (WiFi.status()== 3) {
      WiFi.disconnect();
      Serial.println(WiFi.status());
    //};
    //if (WiFi.status()!= WL_CONNECTED){
    // Connect to WiFi network
    server.send(200, "text/plain", String("LED is now ") + ((state)));
    conectar();
    //ESP.restart();
  }
  else if (state == 1){
    /* Scan for other nodes and send them a message */
/*
    char request[60];
    Serial.println("Entrou...");
    Serial.println("Tentdo encontrar *");
    server.send(200, "text/plain", String("LED is now ") + ((state)));

    String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip)+"|"+Tipo+"|";
    
    sprintf(request, Texto.c_str(), request_i++, ESP.getChipId());
    mesh_node.attemptScan(request);
    //mesh_node.attemptScanNodeSend("Mesh_Node1026892",request);
    delay(1000);
    
    Serial.println("Saiu..");
    

    
    Serial.println(WiFi.status());
    //if (WiFi.status()== 3) {
      WiFi.disconnect();
      Serial.println(WiFi.status());
    //};
    //if (WiFi.status()!= WL_CONNECTED){
      // Connect to WiFi network
    server.send(200, "text/plain", String("LED is now ") + ((state)));
    conectar();
    //ESP.restart();
   };

  server.send(200, "text/plain", String("LED is now ") + ((state)));

}


void meshconsulta() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  //if(!server.authenticate(www_username, www_password))
    //return server.requestAuthentication();
  
  // get the value of request argument "state" and convert it to an int

  String sIP = server.arg("ip");
  String Senha = server.arg("s");
  int Porta = server.arg("p").toInt();
  String funcao = server.arg("f");
  int IdChip = server.arg("c").toInt();
  String Tipo = server.arg("t");

  if ((IdChip == mesh_ChipId.toInt()) && (mesh_Tipo=="R"))
  {
    server.send(200, "text/plain", mesh_funcao);
  }
  else
  {
    server.send(200, "text/plain", "-1");
  }
}

*/
void conectar()
{
  WiFi.disconnect();
  WiFi.softAPdisconnect();

  String retorno = wifiPadrao();
  vConfigWIFI = retorno;

  Serial.println("Valor de retorno: " + retorno);
  if (retorno == "0")
  {
    WiFi.mode(WIFI_STA);
    listawifi();
    tipoWifiAtual = 1;
    String ssid_STR = pegaSSID();
    const char *ssid = ssid_STR.c_str();

    String password_STR = pegaPassword();
    const char *password = password_STR.c_str();

    String ip_STR = pegaIP();
    const char *ipStr = ip_STR.c_str();
    byte ip2[4];
    parseBytes(ipStr, ',', ip2, 4, 10);
    IPAddress ip(ip2);

    String mask_STR = pegaMask();
    const char *maskStr = mask_STR.c_str();
    byte mask2[4];
    parseBytes(maskStr, ',', mask2, 4, 10);
    IPAddress subnet(mask2); // set gateway to match your network

    String gateway_STR = pegaGateway();
    const char *gatewayStr = gateway_STR.c_str();
    byte gateway2[4];
    parseBytes(gatewayStr, ',', gateway2, 4, 10);
    IPAddress gateway(gateway2);

    IPAddress dns(8, 8, 8, 8);

    Serial.println(ip);
    Serial.println(gateway);
    Serial.println(subnet);
    Serial.println(ssid);
    Serial.println(password);

    IpDispositivo = ip;

    local_IP = ip;

    WiFi.config(ip, gateway, subnet, DNS1, DNS2);
    WiFi.begin(ssid, password);
    Serial.println("");
    counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      if (counter > 20)
      {
        WiFi.mode(WIFI_AP);
        tipoWifiAtual = 2;
        listawifi();
        chip3.write(LedWiFI, HIGH);
        Serial.println("Rede não localizada");
        IPAddress local_IP(192, 168, 4, 1);
        IPAddress gateway(192, 168, 4, 1);
        IPAddress subnet(255, 255, 255, 0);

        Serial.print("Setting soft-AP configuration ... ");
        Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

        Serial.print("Setting soft-AP ... ");
        Serial.println("idencitifcador");
        //int chipId = ESP.getChipId();
        String NomeRede = "KEEPIN_" + vchipId;
        Serial.println(NomeRede);
        const char *nRede = NomeRede.c_str();
        Serial.println(nRede);

        //Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

        if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
        {
          wifireset2();
        }

        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());
        IpDispositivo = local_IP;

        break;
      }
      else
      {
        chip3.write(LedWiFI, LOW);
        ///mesh_node.begin();
      }

      delay(500);
      Serial.print(".");
      counter++;

      int ValorbuttonState = digitalRead(buttonState);

      if (ValorbuttonState == HIGH)
      {
        Serial.print("resetando");
        WiFi.disconnect();
        wifireset2();
        ESP.restart();
      }
    }
    //Verifica o nivel do sinal wifi
    //int32_t rssi = getRSSI(SSID);
  }
  else
  {
    WiFi.mode(WIFI_AP);
    tipoWifiAtual = 2;
    chip3.write(LedWiFI, HIGH);

    Serial.println();

    listawifi();
    Serial.println(vListaWifi);

    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);

    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println("idencitifcador");
    //int chipId = ESP.getChipId();
    String NomeRede = "KEEPIN_" + vchipId;
    Serial.println(NomeRede);
    const char *nRede = NomeRede.c_str();
    Serial.println(nRede);

    if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
    {
      wifireset2();
    }

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    IpDispositivo = local_IP;
  }
}

/*
void conectarmesh()
{
  WiFi.disconnect();    
  
  String retorno = wifiPadrao();

          Serial.println("Rede não localizada");    
          IPAddress local_IP(192,168,4,1);
          IPAddress gateway(192,168,4,1);
          IPAddress subnet(255,255,0,0);

          IPAddress mesh_ip(0,0,0,0);
          IPAddress mesh_gateway(0,0,0,0);
          IPAddress mesh_subnet(0,0,0,0);
          
          
          WiFi.config(mesh_ip, mesh_gateway, mesh_subnet);


          Serial.print("Setting soft-AP configuration ... ");
          Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
      
          Serial.print("Setting soft-AP ... ");
          Serial.println("idencitifcador");
          int chipId = ESP.getChipId();
          String NomeRede = "ESP_" + String(chipId);
          Serial.println(NomeRede);
          const char *nRede = NomeRede.c_str();
          Serial.println(nRede);
          
          Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");
      
          Serial.print("Soft-AP IP address = ");
          Serial.println(WiFi.softAPIP());  
          IpDispositivo = local_IP;
  
}
*/

/*
String manageRequest(String request)
{
  mesh_sIP = "";
  mesh_Porta = "";
  mesh_funcao = "";
  mesh_ChipId = "";
  mesh_Tipo = "";
  mesh_Senha = "";  
  /* Print out received message */
/*
  Serial.print("received: ");
  Serial.println(request);

    int i2 = 0;
   
    for (int i = 0; i < 255; i++) 
    {
      if (request[i] != '|' && i2 <= 5)
      { 
        if (i2 == 0)
        {       
          mesh_sIP += request[i];
        }

        if (i2 == 1)
        {       
          mesh_Porta += request[i];
        }
        if (i2 == 2)
        {       
          mesh_funcao += request[i];
        }
        if (i2 == 3)
        {       
          mesh_ChipId += request[i];
        }    
        if (i2 == 4)
        {       
          mesh_Tipo += request[i];
        }        
        if (i2 == 5)
        {       
          mesh_Senha += request[i];
        }        
            
      }      
      else
      {
        i2++;
      }
    }
    Serial.println("");
    Serial.println("IP: " + mesh_sIP);
    Serial.println("Porta: " + mesh_Porta);
    Serial.println("Funcao: " + mesh_funcao);
    Serial.println("ChipId: " + mesh_ChipId);
    Serial.println("Tipo: " + mesh_Tipo);

    String Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";

  /* return a string to send back */
/*
  char response[60];

  if (mesh_Senha == "kdi9e")
  {

    mesh_Porta = String(retornaPorta(mesh_Porta.toInt()));
    if (mesh_Porta != "")
      //pinMode(mesh_Porta.toInt(), OUTPUT);          
    
//    if (mesh_Porta.toInt() == 1)
//    {
//      mesh_Porta = "14";
//    }

      if ((mesh_Tipo == "E") && (mesh_ChipId.toInt() == ESP.getChipId()))
      {
        if (mesh_funcao=="true")
        {
          //digitalWrite(mesh_Porta.toInt(), 1);
          LigaDesliga(mesh_Porta.toInt(), HIGH, "", 0);
          mesh_funcao = "true";
          mesh_Tipo = "R";
          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";
          Serial.println("led ligado UDP");        
          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());
          return response;
          
        }
        else 
        {
          //digitalWrite(mesh_Porta.toInt(), 0);
          LigaDesliga(mesh_Porta.toInt(), LOW, "", 0);
          Serial.println("led 1 desligado UDP");      
          mesh_Tipo = "R";
          mesh_funcao = "false";
          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";
          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());
          return response;          
        }
      }
      else
      if ((mesh_Tipo == "C") && (mesh_ChipId.toInt() == ESP.getChipId())){
        if (LePorta(mesh_Porta.toInt()) == HIGH) {
          mesh_funcao = "true";
          mesh_Tipo = "R";
          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";
          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());
          return response;          
        }
//        else
        //{
//          mesh_Tipo = "R";
  //        mesh_funcao = "false";
    //      Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";
      //    sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());
        //  return response;          
        //}
      }
   }
}
*/
void listawifi()
{

  Serial.println("consultou");
  int numberOfNetworks = WiFi.scanNetworks();
  vListaWifi = "";

  for (int i = 0; i < numberOfNetworks; i++)
  {
    vListaWifi += WiFi.SSID(i);
    vListaWifi += "|";
  }
  vListaWifi += "*";
  server.send(200, "text/html", vListaWifi);
}

void listawifi2()
{

  server.send(200, "text/html", vListaWifi);
}

void gravasenhawifi()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String req = server.arg("v");
  String CurrentPass = server.arg("a");

  if (vSenha == Senha)
  {
    if (req == "")
    {
      req = "12345678"; // se não houver registro, vai para o padrão
    }

    if (vSenhaAP == CurrentPass)
    {

      SPIFFS.begin();
      File f = SPIFFS.open("/senhaap.txt", "w");

      f.println(req + "|");
      f.close();

      SPIFFS.end();
      vSenhaAP = req;
      server.send(200, "text/html", "ok");
      Serial.println("Alterado: " + vSenhaAP);
    }
    else
    {
      server.send(200, "text/html", "-1");
      Serial.println("Senha invalida: Atual = " + vSenhaAP + " Informada: " + CurrentPass);
    }
  }
  else
  {
    Serial.println("erro de senha de comunicacao: Senha Registrada: " + Senha + " senha enviada: " + vSenha);
    server.send(200, "text/html", "-1");
  }
}

void gravasenhahttp()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = server.arg("s");
  String vreq = server.arg("v");
  String CurrentPass = server.arg("a");
  String vUsuarioAntigo = server.arg("ua");
  String vUsuariohttp = server.arg("u");

  if (vSenha == Senha)
  {
    if (vreq == "" || vreq == " ")
    {
      vreq = "keepin"; // se não houver registro, vai para o padrão
    }

    if (vUsuariohttp == "" || vUsuariohttp == " ")
    {
      vUsuariohttp = "keepin"; // se não houver usuario vai para o padrao
    }

    if (senha1 == CurrentPass && vUsuarioAntigo == usuario1)
    {

      seg.gravar(vUsuariohttp, vreq);
      /*
        SPIFFS.begin();
        //senha
        File f = SPIFFS.open("/httppass.txt", "w");
        
        f.println(vreq+"|");
//        f.println("keepin|");
        f.close();

        //Usuario
        f = SPIFFS.open("/httpuser.txt", "w");
        
//        f.println("keepin|");
        f.println(vUsuariohttp+"|");
        f.close();  

        Serial.println("req: " + vreq);
        Serial.println("usuario: " + vUsuariohttp);
        SPIFFS.end();    
        */
      Serial.println("Reiniciando sistema depois de alterar http senha");
      server.send(200, "text/html", "ok");
      delay(500);
      ESP.restart();
    }
    else
    {
      server.send(200, "text/html", "-1");
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void LoopReconexao()
{
  // reconexao
  if ((vConfigWIFI == "0" && WiFi.status() != WL_CONNECTED) || (vConfigWIFI == "0" && tipoWifiAtual == 2))
  {
    if ((millisAtual - lastWifiTime) >= 300000)
    {
      conectar();
      lastWifiTime = millisAtual;
    }
  }

  // valida IP Reconexao
  if ((vConfigWIFI == "0" && tipoWifiAtual != 2 && WiFi.localIP() != local_IP))
  {
    conectar();
  }
}