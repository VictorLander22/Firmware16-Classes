void api() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
    
    String vPassApi = server.arg("pw"); 
    vPassApi.toLowerCase();
    
    if (AlowApi == true && vPassApi == ApiPass)
    {
        String action = server.arg("a");
        String apiPort = server.arg("p");
        String apiSource = server.arg("s");
        Serial.println(apiSource);
        // consulta
        if (action == "q")
        {
            //todas portas
            if (apiPort == "a")
            {
                String sDados1 = "";
                String sDados2 = "";
                if (apiSource == "o") // saidas
                {
                    sDados1 = String(chip1.read8(), BIN);
                    sDados2 = String(chip2.read8(), BIN);
                }
                else if (apiSource == "i") // entradas
                {
                    sDados1 = String(sensor1.read8(), BIN);
                    sDados2 = String(sensor2.read8(), BIN);                    
                }
                else
                {
                    sDados1 = "00000000";
                    sDados2 = "00000000";
                }
                
                Serial.println("tamanho");
                Serial.println(sDados1.length());
                while (sDados1.length() < 8)
                {
                    sDados1 = '0' + sDados1;  
                }

                while (sDados2.length() < 8)
                {
                    sDados2 = '0' + sDados2;  
                }

                

                for (int i = 0; i < 8; i++) {
                    if (sDados1[i] == '0') {
                        sDados1[i] = '1';
                    }
                    else {
                        sDados1[i] = '0';
                    }
                }

                for (int i = 0; i < 8; i++) {
                    if (sDados2[i] == '0') {
                        sDados2[i] = '1';
                    }
                    else  {
                        sDados2[i] = '0';
                    }
                }
                
                

                server.send(200, "text/html", sDados2+sDados1);                                         
            }
            else
            {
                if (apiSource == "o")
                {
                    if (LePorta(apiPort.toInt()-1) == HIGH) 
                    {
                        server.send(200, "text/html", "1");
                    } else
                    {
                        server.send(200, "text/html", "0");
                    }                    
                }
                else
                {
                    if (LeSensor(apiPort.toInt()-1) == HIGH) 
                    {
                        server.send(200, "text/html", "1");
                    } else
                    {
                        server.send(200, "text/html", "0");
                    }                                        
                }
            }
        }
        else if (action == "a") // ação
        {
            String valueApi = server.arg("v");
            String typeApi = server.arg("t");
            if (apiPort == "a") // todos
            {
                if (apiSource == "o") // saidas
                {
                    if (valueApi == "0")
                    {
                        if (typeApi == "n")
                        {
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;
                            Rtc.set_chip1();
                            Rtc.set_chip2();
                            chip1.write8(255);
                            chip2.write8(255);                                
                            server.send(200, "text/html", "1");    
                        }                
                        else if (typeApi == "p")
                        {
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;
                            Rtc.set_chip1();
                            Rtc.set_chip2();     
                            chip1.write8(255);
                            chip2.write8(255);                           
                            server.send(200, "text/html", "1");                    
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");                                                
                        }
                    }
                    else
                    if (valueApi == "1")
                    {
                        if (typeApi == "n")
                        {
                            Rtc.chip1 = 0;
                            Rtc.chip2 = 0;
                            Rtc.set_chip1();
                            Rtc.set_chip2();                                
                            chip1.write8(0);
                            chip2.write8(0);
                            server.send(200, "text/html", "1");                    
                        }
                        else if (typeApi == "p")
                        {
                            Rtc.chip1 = 0;
                            Rtc.chip2 = 0;
                            chip1.write8(0);
                            chip2.write8(0);
                            delay(300);
                            chip1.write8(255);
                            chip2.write8(255);
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;                            
                            Rtc.set_chip1();
                            Rtc.set_chip2();                                
                            server.send(200, "text/html", "1");                                                
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
                else
                {
                    server.send(200, "text/html", "-1");
                }
            }
            else // porta
            {
                if (apiSource == "o") // saidas
                {
                    if (typeApi == "n") //normal
                    {
                        if (valueApi == "0") // desliga
                        {
                            LigaDesliga(apiPort.toInt()-1, LOW, "", 0);
                            server.send(200, "text/html", "1");                    
                        }
                        else if (valueApi == "1") // Liga
                        {
                            LigaDesliga(apiPort.toInt()-1, HIGH, "", 0);
                            server.send(200, "text/html", "1");                                                
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");                                                                            
                        }
                    }
                    else if (typeApi == "p") // pulsado
                    {
                        if (valueApi == "0") // desliga
                        {
                            LigaDesliga(apiPort.toInt()-1, LOW, "", 1);
                            server.send(200, "text/html", "1");                    
                        }
                        else if (valueApi == "1") // Liga
                        {
                            LigaDesliga(apiPort.toInt()-1, HIGH, "", 1);
                            server.send(200, "text/html", "1");                                                
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
                else
                {
                    server.send(200, "text/html", "-1");                    
                }

            }
        }
        else
        // Infravermelho
        if (action == "i")
        {
            Serial.println("api infravermelho");
            String vModel1 = server.arg("m1");
            String vModel2 = server.arg("m2");
            String vModel3 = server.arg("m3");
            String vModel4 = server.arg("m4");
            String Comando1 = server.arg("c1");           
            String Comando2 = server.arg("c2");
            String Comando3 = server.arg("c3");
            String Comando4 = server.arg("c4");
            String Comando12 = server.arg("c12");           
            String Comando22 = server.arg("c22");
            String Comando32 = server.arg("c32");
            String Comando42 = server.arg("c42");
            String qtde1 = server.arg("qt1");
            String qtde2 = server.arg("qt2");
            String qtde3 = server.arg("qt3");
            String qtde4 = server.arg("qt4");
            String vp1 = server.arg("p1");
            String vp2 = server.arg("p2");
            String vp3 = server.arg("p3");
            String vp4 = server.arg("p4");

            if (vModel1 != "")            
            {
                sendirAPI(qtde1.toInt(), vModel1.toInt(), Comando1, Comando12, vp1.toInt());
                Serial.println("1");
            }

            if (vModel2 != "")            
            {
                delay(300);
                sendirAPI(qtde2.toInt(), vModel2.toInt(), Comando2, Comando22, vp2.toInt());
                Serial.println("2");
            }
            
            if (vModel3 != "")            
            {
                delay(300);
                sendirAPI(qtde3.toInt(), vModel3.toInt(), Comando3, Comando32, vp3.toInt());
                Serial.println("3");
            }

            if (vModel4 != "")            
            {
                delay(300);
                sendirAPI(qtde4.toInt(), vModel4.toInt(), Comando4, Comando42, vp4.toInt());
            }
            server.send(200, "text/html", "1");                                                                                                    
       }       
        else
        // Cenas
        if (action == "c")
        {
            Serial.println("api cenas");
            String valueApi = server.arg("v");
            triggerCena(valueApi);
            server.send(200, "text/html", "1");
       }
       else  if (action == "l")  // linha de ação 
       {
            for (int i = 1; i <= apiPort.length(); i++ )
            {
                int posicaoPorta = i-1;
                if (posicaoPorta <= 15) {
                    if (apiPort[posicaoPorta] == '0') { 
                        // desliga
                        LigaDesliga(posicaoPorta, LOW, "", 0);
                    } else if (apiPort[posicaoPorta] == '1') {
                        // liga
                        LigaDesliga(posicaoPorta, HIGH, "", 0);
                    }

               }
           }

            server.send(200, "text/html", "1");                                                                                                    
       }        
    }
    else
    {
        server.send(200, "text/html", "-1");     
    }
}

void apiativo() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

    if (AlowApi == true)
    {
        server.send(200, "text/html", "1");         
    }
    else
    {
        server.send(200, "text/html", "0");         
    }
}

void apiconfig() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
    
    String vSenha = server.arg("s");
    String vApi = server.arg("v");

    if (vSenha == Senha)
    {
        SPIFFS.begin();

        File f = SPIFFS.open("/alowapi.txt", "w"); 
        f.println(vApi+"|");
        f.close();          

        SPIFFS.end();  

        if (vApi == "1") {
            AlowApi = true;
        }
        else if (vApi == "0") {
            AlowApi = false;
        }                  

        server.send(200, "text/html", "ok");
    }
    else
    {
        server.send(200, "text/html", "-1");
    }
}

void alterasenhapi() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
    
  String vSenha = String(server.arg("s"));
  String req = server.arg("v");
  String CurrentPass = server.arg("a");

  if (vSenha == Senha)
  {
    if (req == "")
    {
      req = "12345678";  // se não houver registro, vai para o padrão
    } 
    
    MD5Builder md5;
    md5.begin();
    md5.add(req);
    md5.calculate();
    req = md5.toString();



    md5.begin();
    md5.add(CurrentPass);
    md5.calculate();
    CurrentPass = md5.toString();

    if (ApiPass == CurrentPass) {

      SPIFFS.begin();
      File f = SPIFFS.open("/apipass.txt", "w");
      
      f.println(req+"|");
      f.close();
      
      SPIFFS.end();    
      ApiPass = req;
      server.send(200, "text/html", "ok");
    }
    else {
      server.send(200, "text/html", "-1");                   
    }
  }
  else {
    server.send(200, "text/html", "-1");       
  }
    
}