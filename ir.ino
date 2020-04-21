void configIR()
{

  save.rawbuf = new uint16_t[irrecv.getBufSize()];
  if (save.rawbuf == NULL) {  
    Serial.printf("Não foi possível alocar %d de tamanho de buffer.\n"
                  "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!",
                  irrecv.getBufSize());
    ESP.restart();
  }
  irrecv.setUnknownThreshold(kMinUnknownSize);
  irrecv.enableIRIn();  // Start the receiver
}
// Display encoding type
//
void encoding(decode_results *results) {
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");           Modelo = 0;      break;
    case NEC:          Serial.print("NEC");               Modelo = 1;      break;
    case NEC_LIKE:     Serial.print("NEC (non-strict)");  Modelo = 2;      break;
    case SONY:         Serial.print("SONY");              Modelo = 3;      break;
    case RC5:          Serial.print("RC5");               Modelo = 4;      break;
    case RC5X:         Serial.print("RC5X");              Modelo = 5;      break;
    case RC6:          Serial.print("RC6");               Modelo = 6;      break;
    case RCMM:         Serial.print("RCMM");              Modelo = 7;      break;
    case DISH:         Serial.print("DISH");              Modelo = 8;      break;
    case SHARP:        Serial.print("SHARP");             Modelo = 9;      break;
    case JVC:          Serial.print("JVC");               Modelo = 10;      break;
    case SANYO:        Serial.print("SANYO");             Modelo = 11;      break;
    case SANYO_LC7461: Serial.print("SANYO_LC7461");      Modelo = 12;      break;
    case MITSUBISHI:   Serial.print("MITSUBISHI");        Modelo = 13;      break;
    case SAMSUNG:      Serial.print("SAMSUNG");           Modelo = 14;      break;
    case LG:           Serial.print("LG");                Modelo = 15;      break;
    case WHYNTER:      Serial.print("WHYNTER");           Modelo = 16;      break;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");      Modelo = 17;      break;
    case PANASONIC:    Serial.print("PANASONIC");         Modelo = 18;      break;
    case DENON:        Serial.print("DENON");             Modelo = 19;      break;
    case COOLIX:       Serial.print("COOLIX");            Modelo = 20;      break;
    case GREE:         Serial.print("GREE");              Modelo = 21;      break;
  }
  if (results->repeat) Serial.print(" (Repeat)");


}

// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results) {
  if (results->overflow)
    Serial.printf("WARNING: IR code too big for buffer (>= %d). "
                  "These results shouldn't be trusted until this is resolved. "
                  "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                  CAPTURE_BUFFER_SIZE);

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  serialPrintUint64(results->value, 16);

  Serial.print(" (");
  //if (Modelo == 3 || Modelo == 1 || Modelo == 14)
  if (Modelo > 0 && Modelo < 22)
  {
    tamanho = String(results->bits, DEC).toInt();
  }
//  Serial.print(results->bits, DEC);
//  Serial.println(" bits)");
}

uint16_t getCookedLength(decode_results *results) {
  uint16_t length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++) {
    uint32_t usecs = results->rawbuf[i] * RAWTICK;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    length += (usecs / UINT16_MAX) * 2;
  }
  return length;
}

// Dump out the decode_results structure.
//
void dumpRaw(decode_results *results) {
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen - 1, DEC);
  Serial.println("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0) {  // even
      Serial.print("-");
    } else {  // odd
      Serial.print("   +");
    }
    Serial.printf("%6d", results->rawbuf[i] * RAWTICK);
    if (i < results->rawlen - 1)
      Serial.print(", ");  // ',' not needed for last one
    if (!(i % 8)) Serial.println("");
  }
  Serial.println("");  // Newline
}

// Dump out the decode_results structure.
//


void dumpCode(decode_results *results) {

  // Start declaration
  String codigoIR2 = "";
  Serial.print("uint16_t ");               // variable type
  Serial.print("rawData[");                // array name
  tamanho = String(getCookedLength(results), DEC).toInt();
  Serial.print(getCookedLength(results), DEC);  // array size
  codigoIR2 = "{";
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++) {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * RAWTICK;
         usecs > UINT16_MAX;
         usecs -= UINT16_MAX)
    {
      Serial.printf("%d, 0", UINT16_MAX);
      codigoIR2 += printf("%d, 0", UINT16_MAX);
    }
    Serial.print(usecs, DEC);
    codigoIR2 += String(usecs, DEC);
    if (i < results->rawlen - 1)
    {
      Serial.print(", ");  // ',' not needed on last one
      codigoIR2 +=  ", ";
    }
    if (i % 2 == 0)
    {
      Serial.print(" ");  // Extra if it was even.
      codigoIR2 += " ";
    }
  }

  // End declaration
  Serial.print("};");  //
  codigoIR2 += "}";
//  Serial.println("tamanho");
 // Serial.println(String(tamanho));
  //Serial.println("codigoIR2");
  //Serial.println(codigoIR2);

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  serialPrintUint64(results->value, HEX);
  //if (Modelo == 3 || Modelo == 1 || Modelo == 14){
  if (Modelo >= 0 && Modelo < 22){    
/*
    unsigned long long1 = (unsigned long)((results->value & 0xFFFF0000) >> 16 );
    unsigned long long2 = (unsigned long)((results->value & 0x0000FFFF));

    String slong1 = String(long1, HEX);
    String slong2 = String(long2, HEX);

    while (slong1.length() < 4)
    {
      slong1 = '0' + slong1;
    }

    while (slong2.length() < 4)
    {
      slong2 = '0' + slong2;
    }

    String hex = slong1 + slong2;
    codigoIR = "" + hex + "" ;
  */
    codigoIR = "" + uint64ToString(results->value, 16) + "" ;

  }
 // Serial.println("CodigoIR: " + codigoIR) ;

  Serial.println("Modelo: " + String(Modelo));
  if (Modelo == 0)
  {
      SPIFFS.begin();
      File rFile = SPIFFS.open("/ir_"+codigoIR+".cfg","w");
      rFile.println(codigoIR2);
      rFile.close();
      SPIFFS.end();

      Serial.print("arquivo: ");
      Serial.println(codigoIR2);
  }


  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0) {
      Serial.print("uint32_t address = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
      Serial.print("uint32_t command = 0x");
      Serial.print(results->command, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("uint64_t data = 0x");
    serialPrintUint64(results->value, 16);
    if (Modelo == 19) {
      codigoIR = "" + uint64ToString(results->value, 16) + "" ;
    } else if (Modelo == 21) {
      codigoIR = "" + resultToHexidecimal(results) + "" ;
      //codigoIR = codigoIR.substring(2);
      tamanho = uint64ToString(results->bits).toInt();

/*      Serial.println("");
      Serial.print("Codigo 64: ");
      Serial.println(codigoIR);

      Serial.print("Tamanho: ");
      Serial.println(tamanho);*/
    }
    Serial.println(";");
  }

}

void getIR()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  
  server.send(200, "text/html", String(tamanho) + "|" + String(Modelo) + "|" + codigoIR + "*");  
  tamanho = -1;
  codigoIR = "-1";
}

void sendir()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  
  String S = server.arg("s");
  int QtdeBit = 12;
  int PortaIRS = server.arg("p").toInt();
  int vModelo = server.arg("m").toInt();
  int q = server.arg("q").toInt();

  String Codigo = server.arg("c");
  String Codigo2 = server.arg("c2");
  
//  Serial.println(Codigo);
  //rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);
  //uint16_t rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);

  Serial.println(Codigo+Codigo2);
  
  if (S == Senha && QtdeBit > 0) 
  {
      sendIRCMD(Codigo, Codigo2, QtdeBit, PortaIRS, vModelo, q);
      Codigo = "";
      Codigo2 = "";
    //PortaIRS = retornaPorIRS(PortaIRS);   

    Serial.println("Enviado IR");
  }
  server.send(200, "text/html", "ok");    
}

void sendIRCMD(String Codigo, String Codigo2, int QtdeBit, int PortaIRS, int vModelo, int q)
{
  //logData("ComandoIR - Porta: " + String(PortaIRS) + " - Modelo: " + String(vModelo));

  //PortaIRS = retornaPorIRS(PortaIRS);
  QtdeBit = 12;
  configuraPortaIR(PortaIRS);
  if (q > 0)
    QtdeBit = q;

  if (QtdeBit > 0) 
  {
    IRsend irsend(16, true);
    irsend.begin();
    Serial.println("Modelo: " + String(vModelo));
    Serial.print("tamanho: ");
    Serial.println(String(QtdeBit));
    Serial.print("Codigo1: ");
    Serial.println(Codigo);
    Serial.print("Codigo2: ");
    Serial.println(Codigo2);
    if (vModelo == 1) // NEC
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 2) // NEC (non-strict)
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 3)   //Sony
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SONY
        irsend.sendSony(vCodigo, QtdeBit, 2);
      //#endif
    } else
    if (vModelo == 4)   //RC5
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    } else
    if (vModelo == 5)   //RC5X
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    } else
    if (vModelo == 6) //RC6
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendRC6(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 7) // RCMM
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendRCMM(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 8) // DISH
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendDISH(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 9) // SHARP
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SHARP
        irsend.sendSharp(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 10) // JVC
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_JVC
        irsend.sendJVC(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 13) // MITSUBISHI
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendMitsubishi(vCodigo, QtdeBit);      
      //#endif
    }else

    if (vModelo == 14) // SAMSUNG
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      //unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SAMSUNG
        irsend.sendSAMSUNG(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 15) // LG
    {
      Codigo = "0x" + Codigo;
      ////Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_LG
        irsend.sendLG(vCodigo, QtdeBit);            
      //#endif
    }   
    else
    if (vModelo == 16) // WHYNTER
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendWhynter(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 17) // AIWA
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendAiwaRCT501(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 19) // DENON
    {
      Codigo.toUpperCase();
      //Codigo = "2A4C028A0088";
      //unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16);
      //long long unsigned vCodigo = strtoul(Codigo.c_str(), NULL, 16);
      uint64_t vCodigo = getUInt64fromHex(Codigo.c_str());
      irsend.sendDenon(vCodigo, QtdeBit);  //kDenon48Bits
      //Serial.println("DENON OK");
      //Serial.println(vCodigo);
    }else
    if (vModelo == 20) // COOLIX
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendCOOLIX(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 21) // GREE
    {      
      Codigo.toUpperCase();
      uint64_t vCodigo = getUInt64fromHex(Codigo.c_str());
      irsend.sendGree(vCodigo, QtdeBit);
      Serial.println("");
      Serial.println("GREE");
      Serial.println("Qtde Bit");
      Serial.println(QtdeBit);
    }    
    else
    {
      uint16_t rawData[QtdeBit];
      for (int i = 0; i <= QtdeBit-1; i++)
      {
        rawData[i] = 0;  
      }

      int tam2 = 0;
      String CodAtu = "";

      SPIFFS.begin();
      File rFile = SPIFFS.open("/ir_"+Codigo+".cfg", "r");
      int Qtde;
      Qtde = 0;
      Codigo = "";
      while(rFile.available()) {
          String linhas = rFile.readStringUntil('\n');
          Serial.println("linhas: ");
          Serial.print(linhas);
          
          for (int i = 1; i <= linhas.length(); i++)
          {
            if (linhas[i] != ',' && linhas[i] != '}')
            {
              CodAtu += linhas[i];
            }
            else
            {
              rawData[tam2] = CodAtu.toInt();
              //Serial.println(String(CodAtu.toInt()));
              tam2 ++;
              CodAtu = "";
            }
 
          }
          Qtde++;
      }

      rFile.close();
      SPIFFS.end();
      //converteRAW(Codigo, Codigo2, QtdeBit);
/*
      Codigo += ",";

      //Serial.println("Codigo Recebido: " + codig);

      for (int i = 1; i <= Codigo.length(); i++)
      {
        if (Codigo[i] != ',')
        {
          CodAtu += Codigo[i];
        }
        else
        {
          rawData[tam2] = CodAtu.toInt();
          //Serial.println(String(CodAtu.toInt()));
          tam2 ++;
          CodAtu = "";
        }
      }

      CodAtu = "";
      for (int i = 1; i <= Codigo2.length(); i++)
      {
        if (Codigo2[i] != ',')
        {
          CodAtu += Codigo2[i];
        }
        else
        {
          rawData[tam2] = CodAtu.toInt();
          //Serial.println(String(CodAtu.toInt()));
          tam2 ++;
          CodAtu = "";
        }
      }
   */
//      Serial.println("Qtde: " + String(QtdeBit));

//      for (int i = 0; i <= QtdeBit-1; i++) 
//      {
//        Serial.println(String(rawData[i]));
//      }
  
      //#if SEND_RAW
      irsend.sendRaw(rawData, QtdeBit, 38);         
      Serial.println("enviado...");
      //#endif

/*
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, 32);      
      //#endif

      Serial.println("Envio IR: " + String(QtdeBit));
      Serial.println(String(vCodigo));
*/
    }

    Codigo = "";
    Codigo2 = "";
    digitalWrite(16, LOW);
  
  }

}

int retornaPorIRS(int PortaIRS)
{
  if (PortaIRS == 1)
  {
    return 16;  
  }
  else if (PortaIRS == 2)
  {
    return 12;
  }
  else if (PortaIRS == 3)
  {
    return 13;
  }
  else if (PortaIRS == 4)
  {
    return 15;
  }
  else
    return 16;
}

/*
uint16_t converteRAW(String codig, String codig2, int tam)
{
  //uint16_t rawData[tam];
  //limpa registros
  for (int i = 0; i <= tam-1; i++)
  {
    rawData[i] = 0;  
  }

  int tam2 = 0;
  codig += ",";

  //Serial.println("Codigo Recebido: " + codig);

  String CodAtu = "";
  for (int i = 1; i <= codig.length(); i++)
  {
    if (codig[i] != ',')
    {
      CodAtu += codig[i];
    }
    else
    {
      rawData[tam2] = CodAtu.toInt();
      //Serial.println(String(CodAtu.toInt()));
      tam2 ++;
      CodAtu = "";
    }
  }

  CodAtu = "";
  for (int i = 1; i <= codig2.length(); i++)
  {
    if (codig2[i] != ',')
    {
      CodAtu += codig2[i];
    }
    else
    {
      rawData[tam2] = CodAtu.toInt();
      //Serial.println(String(CodAtu.toInt()));
      tam2 ++;
      CodAtu = "";
    }
  }
  
/*  
  String Teste = "{";

  for (int i = 0; i< tam; i++)
  {
    Teste += String(rawData[i]) + ",";
  }
  Teste += "}";
  //Serial.println("O que está gravado: " + Teste);
  */

  //return rawData;
/*}*/

void habir()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  irrecv.resume();
  irrecv.resume();
  irrecv.resume();
  
  enReadIR = true;
  server.send(200, "text/html", "ok");  
}

char hexCharToBin(char c) {
 if (isdigit(c)) {  // 0 - 9
   return c - '0';
 } else if (isxdigit(c)) { // A-F, a-f
   return (c & 0xF) + 9;
 }
 return -1;
}

unsigned long long hexStrToULL(char* string)
{
  unsigned long long x =0;
  char c;
  do {
    c =hexCharToBin(*string++);
    if (c < 0)
      break;
    x = (x << 4) | c;
 } while (1);
 return x;
}

void configuraPortaIR(int PortIR2) 
{
  if (PortIR2 == 1)
  {
    chip3.write(0, LOW);        
    chip3.write(1, LOW);        
    chip3.write(2, LOW);        
  }
  else
  if (PortIR2 == 2)
  {
    chip3.write(0, HIGH);        
    chip3.write(1, LOW);        
    chip3.write(2, LOW);        
  }
  else
  if (PortIR2 == 3)
  {
    chip3.write(0, LOW);        
    chip3.write(1, HIGH);        
    chip3.write(2, LOW);        
  }
  else
  if (PortIR2 == 4)
  {
    chip3.write(0, HIGH);        
    chip3.write(1, HIGH);        
    chip3.write(2, LOW);        
  }
  else
  if (PortIR2 == 5)
  {
    chip3.write(0, LOW);        
    chip3.write(1, LOW);        
    chip3.write(2, HIGH);        
  }
  else
  if (PortIR2 == 6)
  {
    chip3.write(0, HIGH);        
    chip3.write(1, LOW);        
    chip3.write(2, HIGH);        
  }
  else
  if (PortIR2 == 7)
  {
    chip3.write(0, LOW);        
    chip3.write(1, HIGH);        
    chip3.write(2, HIGH);        
  }
  else
  if (PortIR2 == 8)
  {
    chip3.write(0, HIGH);        
    chip3.write(1, HIGH);        
    chip3.write(2, HIGH);        
  }
  
}

void sendirAPI(int vQt, int vMd, String vCod, String vCod2, int vPt)
{
  int QtdeBit = vQt;
  int PortaIRS = vPt;
  int vModelo = vMd;
  //PortaIRS = retornaPorIRS(PortaIRS);
  configuraPortaIR(PortaIRS);

  //rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);
  //uint16_t rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);

    IRsend irsend(16, true);
    irsend.begin();
///    Serial.println("Modelo: " + String(vModelo));
///    Serial.println("tamanho");
///    Serial.println(String(QtdeBit));
    if (vModelo == 1) // NEC
    {
      vCod = "0x" + vCod;
  //    Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
     // uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 2) // NEC (non-strict)
    {
      vCod = "0x" + vCod;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 3)   //Sony
    {
      vCod = "0x" + vCod;
      //Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
     // uint64_t vCodigo2 = vCodigo;
     // #if SEND_SONY
        irsend.sendSony(vCodigo, QtdeBit, 2);
     // #endif
    } else
    if (vModelo == 4)   //RC5
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    } else
    if (vModelo == 5)   //RC5X
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    } else
    if (vModelo == 6) //RC6
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendRC6(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 7) // RCMM
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendRCMM(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 8) // DISH
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendDISH(vCodigo, QtdeBit);      
      //#endif
    }else    
    if (vModelo == 9) // SHARP
    {
      vCod = "0x" + vCod;
      Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //  uint64_t vCodigo2 = vCodigo;
      //#if SEND_SHARP
        irsend.sendSharp(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 10) // JVC
    {
      vCod = "0x" + vCod;
      Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
     // uint64_t vCodigo2 = vCodigo;
      //#if SEND_JVC
        irsend.sendJVC(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 13) // MITSUBISHI
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendMitsubishi(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 14) // SAMSUNG
    {
      vCod = "0x" + vCod;
      Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
   //   uint64_t vCodigo2 = vCodigo;
      //#if SEND_SAMSUNG
        irsend.sendSAMSUNG(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 15) // LG
    {
      vCod = "0x" + vCod;
      Serial.println(vCod);
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
  //    uint64_t vCodigo2 = vCodigo;
      //#if SEND_LG
        irsend.sendLG(vCodigo, QtdeBit);            
      //#endif
    }
    else
    if (vModelo == 16) // WHYNTER
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendWhynter(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 17) // AIWA
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendAiwaRCT501(vCodigo, QtdeBit);      
      //#endif
    }else
    if (vModelo == 19) // DENON
    {
      //Codigo = Codigo;
      vCod.toUpperCase();
      //irsend.sendDenon((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);  //kDenon48Bits
      uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
      irsend.sendDenon(vCodigo, QtdeBit);  //kDenon48Bits
//      Serial.println("DENON OK");
    }else
    if (vModelo == 20) // COOLIX
    {
      vCod = "0x" + vCod;
      unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
      //#if SEND_RC6
        irsend.sendCOOLIX(vCodigo, QtdeBit);      
      //#endif
    }
    else
    if (vModelo == 21) // GREE
    {
      vCod.toUpperCase();
      //irsend.sendGree((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);
      uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
      irsend.sendGree(vCodigo, QtdeBit);

    }    
    else
    {
      uint16_t rawData[QtdeBit];
      for (int i = 0; i <= QtdeBit-1; i++)
      {
        rawData[i] = 0;  
      }

      int tam2 = 0;
      String CodAtu = "";

      SPIFFS.begin();
      File rFile = SPIFFS.open("/ir_"+vCod+".cfg", "r");
      int Qtde;
      Qtde = 0;
      while(rFile.available()) {
          String linhas = rFile.readStringUntil('\n');
          //Serial.print(linhas);
          
          for (int i = 1; i <= linhas.length(); i++)
          {
            if (linhas[i] != ',' && linhas[i] != '}')
            {
              CodAtu += linhas[i];
            }
            else
            {
              rawData[tam2] = CodAtu.toInt();
              //Serial.println(String(CodAtu.toInt()));
              tam2 ++;
              CodAtu = "";
            }
 
          }
          Qtde++;
      }

      rFile.close();
      SPIFFS.end();
      //#if SEND_RAW
      irsend.sendRaw(rawData, QtdeBit, 38);         
      Serial.println("enviado");
      //#endif
    }

    digitalWrite(16, LOW);
 
}

uint64_t getUInt64fromHex(char const *str) {
  uint64_t result = 0;
  uint16_t offset = 0;
  // Skip any leading '0x' or '0X' prefix.
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) offset = 2;
  for (; isxdigit((unsigned char)str[offset]); offset++) {
    char c = str[offset];
    result *= 16;
    if (isdigit(c))
      result += c - '0';  // '0' .. '9'
    else if (isupper(c))
      result += c - 'A' + 10;  // 'A' .. 'F'
    else
      result += c - 'a' + 10;  // 'a' .. 'f'
  }
  return result;
}


//uint64_t getUInt64fromHex(const char *str)  //char const *str
//{
//    uint64_t accumulator = NULL;
//    for (size_t i = 0 ; isxdigit((unsigned char)str[i]) ; ++i)
//    {
//        char c = str[i];
//        accumulator *= 16;
//        if (isdigit(c)) /* '0' .. '9'*/
//            accumulator += c - '0';
//        else if (isupper(c)) /* 'A' .. 'F'*/
//            accumulator += c - 'A' + 10;
//        else /* 'a' .. 'f'*/
//            accumulator += c - 'a' + 10;
//    }
//
//    return accumulator;
//}