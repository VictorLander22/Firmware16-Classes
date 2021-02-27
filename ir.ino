void configIR()
{

  save.rawbuf = new uint16_t[irrecv.getBufSize()];
  if (save.rawbuf == NULL)
  {
    if (DEBUG_ON)
      Serial.printf("Não foi possível alocar %d de tamanho de buffer.\n"
                    "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!",
                    irrecv.getBufSize());
    ESP.restart();
  }
  irrecv.setUnknownThreshold(kMinUnknownSize);
  irrecv.enableIRIn(); // Start the receiver
}
// Display encoding type
//
void encoding(decode_results *results)
{
  switch (results->decode_type)
  {
  default:
  case UNKNOWN:
    if (DEBUG_ON)
      Serial.print("UNKNOWN");
    Modelo = 0;
    break;
  case NEC:
    if (DEBUG_ON)
      Serial.print("NEC");
    Modelo = 1;
    break;
  case NEC_LIKE:
    if (DEBUG_ON)
      Serial.print("NEC (non-strict)");
    Modelo = 2;
    break;
  case SONY:
    if (DEBUG_ON)
      Serial.print("SONY");
    Modelo = 3;
    break;
  case RC5:
    if (DEBUG_ON)
      Serial.print("RC5");
    Modelo = 4;
    break;
  case RC5X:
    if (DEBUG_ON)
      Serial.print("RC5X");
    Modelo = 5;
    break;
  case RC6:
    if (DEBUG_ON)
      Serial.print("RC6");
    Modelo = 6;
    break;
  case RCMM:
    if (DEBUG_ON)
      Serial.print("RCMM");
    Modelo = 7;
    break;
  case DISH:
    if (DEBUG_ON)
      Serial.print("DISH");
    Modelo = 8;
    break;
  case SHARP:
    if (DEBUG_ON)
      Serial.print("SHARP");
    Modelo = 9;
    break;
  case JVC:
    if (DEBUG_ON)
      Serial.print("JVC");
    Modelo = 10;
    break;
  case SANYO:
    if (DEBUG_ON)
      Serial.print("SANYO");
    Modelo = 11;
    break;
  case SANYO_LC7461:
    if (DEBUG_ON)
      Serial.print("SANYO_LC7461");
    Modelo = 12;
    break;
  case MITSUBISHI:
    if (DEBUG_ON)
      Serial.print("MITSUBISHI");
    Modelo = 13;
    break;
  case SAMSUNG:
    if (DEBUG_ON)
      Serial.print("SAMSUNG");
    Modelo = 14;
    break;
  case LG:
    if (DEBUG_ON)
      Serial.print("LG");
    Modelo = 15;
    break;
  case WHYNTER:
    if (DEBUG_ON)
      Serial.print("WHYNTER");
    Modelo = 16;
    break;
  case AIWA_RC_T501:
    if (DEBUG_ON)
      Serial.print("AIWA_RC_T501");
    Modelo = 17;
    break;
  case PANASONIC:
    if (DEBUG_ON)
      Serial.print("PANASONIC");
    Modelo = 18;
    break;
  case DENON:
    if (DEBUG_ON)
      Serial.print("DENON");
    Modelo = 19;
    break;
  case COOLIX:
    if (DEBUG_ON)
      Serial.print("COOLIX");
    Modelo = 20;
    break;
  case GREE:
    if (DEBUG_ON)
      Serial.print("GREE");
    Modelo = 21;
    break;
  }
  if (results->repeat)
    if (DEBUG_ON)
      Serial.print(" (Repeat)");
}

// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
  if (results->overflow)
    if (DEBUG_ON)
      Serial.printf("WARNING: IR code too big for buffer (>= %d). "
                    "These results shouldn't be trusted until this is resolved. "
                    "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                    CAPTURE_BUFFER_SIZE);

  // Show Encoding standard
  if (DEBUG_ON)
    Serial.print("Encoding  : ");
  encoding(results);
  if (DEBUG_ON)
    Serial.println("");

  // Show Code & length
  if (DEBUG_ON)
    Serial.print("Code      : ");
  serialPrintUint64(results->value, 16);

  if (DEBUG_ON)
    Serial.print(" (");
  //if (Modelo == 3 || Modelo == 1 || Modelo == 14)
  if (Modelo > 0 && Modelo < 22)
  {
    tamanho = String(results->bits, DEC).toInt();
  }
  //  if (DEBUG_ON) Serial.print(results->bits, DEC);
  //  if (DEBUG_ON) Serial.println(" bits)");
}

uint16_t getCookedLength(decode_results *results)
{
  uint16_t length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++)
  {
    uint32_t usecs = results->rawbuf[i] * RAWTICK;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    length += (usecs / UINT16_MAX) * 2;
  }
  return length;
}

// Dump out the decode_results structure.
//
void dumpRaw(decode_results *results)
{
  // Print Raw data
  if (DEBUG_ON)
    Serial.print("Timing[");
  if (DEBUG_ON)
    Serial.print(results->rawlen - 1, DEC);
  if (DEBUG_ON)
    Serial.println("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    if (i % 100 == 0)
      yield(); // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0)
    { // even
      if (DEBUG_ON)
        Serial.print("-");
    }
    else
    { // odd
      if (DEBUG_ON)
        Serial.print("   +");
    }
    if (DEBUG_ON)
      Serial.printf("%6d", results->rawbuf[i] * RAWTICK);
    if (i < results->rawlen - 1)
      if (DEBUG_ON)
        Serial.print(", "); // ',' not needed for last one
    if (!(i % 8))
      if (DEBUG_ON)
        Serial.println("");
  }
  if (DEBUG_ON)
    Serial.println(""); // Newline
}

// Dump out the decode_results structure.
//

void dumpCode(decode_results *results)
{

  // Start declaration
  String codigoIR2 = "";
  if (DEBUG_ON)
    Serial.print("uint16_t "); // variable type
  if (DEBUG_ON)
    Serial.print("rawData["); // array name
  tamanho = String(getCookedLength(results), DEC).toInt();
  if (DEBUG_ON)
    Serial.print(getCookedLength(results), DEC); // array size
  codigoIR2 = "{";
  if (DEBUG_ON)
    Serial.print("] = {"); // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * RAWTICK;
         usecs > UINT16_MAX;
         usecs -= UINT16_MAX)
    {
      if (DEBUG_ON)
        Serial.printf("%d, 0", UINT16_MAX);
      codigoIR2 += printf("%d, 0", UINT16_MAX);
    }
    if (DEBUG_ON)
      Serial.print(usecs, DEC);
    codigoIR2 += String(usecs, DEC);
    if (i < results->rawlen - 1)
    {
      if (DEBUG_ON)
        Serial.print(", "); // ',' not needed on last one
      codigoIR2 += ", ";
    }
    if (i % 2 == 0)
    {
      if (DEBUG_ON)
        Serial.print(" "); // Extra if it was even.
      codigoIR2 += " ";
    }
  }

  // End declaration
  if (DEBUG_ON)
    Serial.print("};"); //
  codigoIR2 += "}";
  //  if (DEBUG_ON) Serial.println("tamanho");
  // if (DEBUG_ON) Serial.println(String(tamanho));
  //if (DEBUG_ON) Serial.println("codigoIR2");
  //if (DEBUG_ON) Serial.println(codigoIR2);

  // Comment
  if (DEBUG_ON)
    Serial.print("  // ");
  encoding(results);
  if (DEBUG_ON)
    Serial.print(" ");
  serialPrintUint64(results->value, HEX);
  //if (Modelo == 3 || Modelo == 1 || Modelo == 14){
  if (Modelo >= 0 && Modelo < 22)
  {
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
    codigoIR = "" + uint64ToString(results->value, 16) + "";
  }
  // if (DEBUG_ON) Serial.println("CodigoIR: " + codigoIR) ;

  if (DEBUG_ON)
    Serial.println("Modelo: " + String(Modelo));
  if (Modelo == 0)
  {
    SPIFFS.begin();
    File rFile = SPIFFS.open("/ir_" + codigoIR + ".cfg", "w");
    rFile.println(codigoIR2);
    rFile.close();
    SPIFFS.end();

    if (DEBUG_ON)
      Serial.print("arquivo: ");
    if (DEBUG_ON)
      Serial.println(codigoIR2);
  }

  // Newline
  if (DEBUG_ON)
    Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN)
  {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0)
    {
      if (DEBUG_ON)
        Serial.print("uint32_t address = 0x");
      if (DEBUG_ON)
        Serial.print(results->address, HEX);
      if (DEBUG_ON)
        Serial.println(";");
      if (DEBUG_ON)
        Serial.print("uint32_t command = 0x");
      if (DEBUG_ON)
        Serial.print(results->command, HEX);
      if (DEBUG_ON)
        Serial.println(";");
    }

    // All protocols have data
    if (DEBUG_ON)
      Serial.print("uint64_t data = 0x");
    serialPrintUint64(results->value, 16);
    if (Modelo == 19)
    {
      codigoIR = "" + uint64ToString(results->value, 16) + "";
    }
    else if (Modelo == 21)
    {
      codigoIR = "" + resultToHexidecimal(results) + "";
      //codigoIR = codigoIR.substring(2);
      tamanho = uint64ToString(results->bits).toInt();

      /*      if (DEBUG_ON) Serial.println("");
      if (DEBUG_ON) Serial.print("Codigo 64: ");
      if (DEBUG_ON) Serial.println(codigoIR);

      if (DEBUG_ON) Serial.print("Tamanho: ");
      if (DEBUG_ON) Serial.println(tamanho);*/
    }
    if (DEBUG_ON)
      Serial.println(";");
  }
}

void getIR()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", String(tamanho) + "|" + String(Modelo) + "|" + codigoIR + "*");
  tamanho = -1;
  codigoIR = "-1";
}

void sendir()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");

  String S = server.arg("s");
  int QtdeBit = 12;
  int PortaIRS = server.arg("p").toInt();
  int vModelo = server.arg("m").toInt();
  int q = server.arg("q").toInt();

  String Codigo = server.arg("c");
  String Codigo2 = server.arg("c2");

  //  if (DEBUG_ON) Serial.println(Codigo);
  //rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);
  //uint16_t rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);

  if (DEBUG_ON)
    Serial.println(Codigo + Codigo2);

  if (S == Senha && QtdeBit > 0)
  {
    sendIRCMD(Codigo, Codigo2, QtdeBit, PortaIRS, vModelo, q);
    Codigo = "";
    Codigo2 = "";
    //PortaIRS = retornaPorIRS(PortaIRS);

    if (DEBUG_ON)
      Serial.println("Enviado IR");
  }
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
    if (DEBUG_ON)
      Serial.println("Modelo: " + String(vModelo));
    if (DEBUG_ON)
      Serial.print("tamanho: ");
    if (DEBUG_ON)
      Serial.println(String(QtdeBit));
    if (DEBUG_ON)
      Serial.print("Codigo1: ");
    if (DEBUG_ON)
      Serial.println(Codigo);
    if (DEBUG_ON)
      Serial.print("Codigo2: ");
    if (DEBUG_ON)
      Serial.println(Codigo2);
    if (vModelo == 1) // NEC
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
      irsend.sendNEC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 2) // NEC (non-strict)
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
      irsend.sendNEC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 3) //Sony
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SONY
      irsend.sendSony(vCodigo, QtdeBit, 2);
      //#endif
    }
    else if (vModelo == 4) //RC5
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    }
    else if (vModelo == 5) //RC5X
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      irsend.sendRC5(vCodigo, QtdeBit, 2);
    }
    else if (vModelo == 6) //RC6
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendRC6(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 7) // RCMM
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendRCMM(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 8) // DISH
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendDISH(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 9) // SHARP
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SHARP
      irsend.sendSharp(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 10) // JVC
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_JVC
      irsend.sendJVC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 13) // MITSUBISHI
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendMitsubishi(vCodigo, QtdeBit);
      //#endif
    }
    else

        if (vModelo == 14) // SAMSUNG
    {
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      //unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SAMSUNG
      irsend.sendSAMSUNG(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 15) // LG
    {
      Codigo = "0x" + Codigo;
      ////if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_LG
      irsend.sendLG(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 16) // WHYNTER
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendWhynter(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 17) // AIWA
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendAiwaRCT501(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 19) // DENON
    {
      Codigo.toUpperCase();
      //Codigo = "2A4C028A0088";
      //unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16);
      //long long unsigned vCodigo = strtoul(Codigo.c_str(), NULL, 16);
      uint64_t vCodigo = getUInt64fromHex(Codigo.c_str());
      irsend.sendDenon(vCodigo, QtdeBit); //kDenon48Bits
      //if (DEBUG_ON) Serial.println("DENON OK");
      //if (DEBUG_ON) Serial.println(vCodigo);
    }
    else if (vModelo == 20) // COOLIX
    {
      Codigo = "0x" + Codigo;
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //#if SEND_RC6
      irsend.sendCOOLIX(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 21) // GREE
    {
      Codigo.toUpperCase();
      uint64_t vCodigo = getUInt64fromHex(Codigo.c_str());
      irsend.sendGree(vCodigo, QtdeBit);
      if (DEBUG_ON)
        Serial.println("");
      if (DEBUG_ON)
        Serial.println("GREE");
      if (DEBUG_ON)
        Serial.println("Qtde Bit");
      if (DEBUG_ON)
        Serial.println(QtdeBit);
    }
    else
    {
      uint16_t rawData[QtdeBit];
      for (int i = 0; i <= QtdeBit - 1; i++)
      {
        rawData[i] = 0;
      }

      int tam2 = 0;
      String CodAtu = "";

      SPIFFS.begin();
      File rFile = SPIFFS.open("/ir_" + Codigo + ".cfg", "r");
      int Qtde;
      Qtde = 0;
      Codigo = "";
      while (rFile.available())
      {
        String linhas = rFile.readStringUntil('\n');
        if (DEBUG_ON)
          Serial.println("linhas: ");
        if (DEBUG_ON)
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
            //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
            tam2++;
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

      //if (DEBUG_ON) Serial.println("Codigo Recebido: " + codig);

      for (int i = 1; i <= Codigo.length(); i++)
      {
        if (Codigo[i] != ',')
        {
          CodAtu += Codigo[i];
        }
        else
        {
          rawData[tam2] = CodAtu.toInt();
          //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
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
          //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
          tam2 ++;
          CodAtu = "";
        }
      }
   */
      //      if (DEBUG_ON) Serial.println("Qtde: " + String(QtdeBit));

      //      for (int i = 0; i <= QtdeBit-1; i++)
      //      {
      //        if (DEBUG_ON) Serial.println(String(rawData[i]));
      //      }

      //#if SEND_RAW
      irsend.sendRaw(rawData, QtdeBit, 38);
      if (DEBUG_ON)
        Serial.println("enviado...\n");
      //#endif

      /*
      Codigo = "0x" + Codigo;
      ///if (DEBUG_ON) Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
        irsend.sendNEC(vCodigo, 32);      
      //#endif

      if (DEBUG_ON) Serial.println("Envio IR: " + String(QtdeBit));
      if (DEBUG_ON) Serial.println(String(vCodigo));
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

  //if (DEBUG_ON) Serial.println("Codigo Recebido: " + codig);

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
      //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
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
      //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
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
  //if (DEBUG_ON) Serial.println("O que está gravado: " + Teste);
  */

//return rawData;
/*}*/

void habir()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  server.send(200, "text/html", "ok");
  irrecv.resume();
  irrecv.resume();
  irrecv.resume();

  enReadIR = true;
}

char hexCharToBin(char c)
{
  if (isdigit(c))
  { // 0 - 9
    return c - '0';
  }
  else if (isxdigit(c))
  { // A-F, a-f
    return (c & 0xF) + 9;
  }
  return -1;
}

unsigned long long hexStrToULL(char *string)
{
  unsigned long long x = 0;
  char c;
  do
  {
    c = hexCharToBin(*string++);
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
  else if (PortIR2 == 2)
  {
    chip3.write(0, HIGH);
    chip3.write(1, LOW);
    chip3.write(2, LOW);
  }
  else if (PortIR2 == 3)
  {
    chip3.write(0, LOW);
    chip3.write(1, HIGH);
    chip3.write(2, LOW);
  }
  else if (PortIR2 == 4)
  {
    chip3.write(0, HIGH);
    chip3.write(1, HIGH);
    chip3.write(2, LOW);
  }
  else if (PortIR2 == 5)
  {
    chip3.write(0, LOW);
    chip3.write(1, LOW);
    chip3.write(2, HIGH);
  }
  else if (PortIR2 == 6)
  {
    chip3.write(0, HIGH);
    chip3.write(1, LOW);
    chip3.write(2, HIGH);
  }
  else if (PortIR2 == 7)
  {
    chip3.write(0, LOW);
    chip3.write(1, HIGH);
    chip3.write(2, HIGH);
  }
  else if (PortIR2 == 8)
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
  ///    if (DEBUG_ON) Serial.println("Modelo: " + String(vModelo));
  ///    if (DEBUG_ON) Serial.println("tamanho");
  ///    if (DEBUG_ON) Serial.println(String(QtdeBit));
  if (vModelo == 1) // NEC
  {
    vCod = "0x" + vCod;
    //    if (DEBUG_ON) Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    // uint64_t vCodigo2 = vCodigo;
    //#if SEND_NEC
    irsend.sendNEC(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 2) // NEC (non-strict)
  {
    vCod = "0x" + vCod;
    ///if (DEBUG_ON) Serial.println(Codigo);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //uint64_t vCodigo2 = vCodigo;
    //#if SEND_NEC
    irsend.sendNEC(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 3) //Sony
  {
    vCod = "0x" + vCod;
    //if (DEBUG_ON) Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    // uint64_t vCodigo2 = vCodigo;
    // #if SEND_SONY
    irsend.sendSony(vCodigo, QtdeBit, 2);
    // #endif
  }
  else if (vModelo == 4) //RC5
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    irsend.sendRC5(vCodigo, QtdeBit, 2);
  }
  else if (vModelo == 5) //RC5X
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    irsend.sendRC5(vCodigo, QtdeBit, 2);
  }
  else if (vModelo == 6) //RC6
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendRC6(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 7) // RCMM
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendRCMM(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 8) // DISH
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendDISH(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 9) // SHARP
  {
    vCod = "0x" + vCod;
    if (DEBUG_ON)
      Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //  uint64_t vCodigo2 = vCodigo;
    //#if SEND_SHARP
    irsend.sendSharp(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 10) // JVC
  {
    vCod = "0x" + vCod;
    if (DEBUG_ON)
      Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    // uint64_t vCodigo2 = vCodigo;
    //#if SEND_JVC
    irsend.sendJVC(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 13) // MITSUBISHI
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendMitsubishi(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 14) // SAMSUNG
  {
    vCod = "0x" + vCod;
    if (DEBUG_ON)
      Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //   uint64_t vCodigo2 = vCodigo;
    //#if SEND_SAMSUNG
    irsend.sendSAMSUNG(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 15) // LG
  {
    vCod = "0x" + vCod;
    if (DEBUG_ON)
      Serial.println(vCod);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //    uint64_t vCodigo2 = vCodigo;
    //#if SEND_LG
    irsend.sendLG(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 16) // WHYNTER
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendWhynter(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 17) // AIWA
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendAiwaRCT501(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 19) // DENON
  {
    //Codigo = Codigo;
    vCod.toUpperCase();
    //irsend.sendDenon((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);  //kDenon48Bits
    uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
    irsend.sendDenon(vCodigo, QtdeBit); //kDenon48Bits
                                        //      if (DEBUG_ON) Serial.println("DENON OK");
  }
  else if (vModelo == 20) // COOLIX
  {
    vCod = "0x" + vCod;
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //#if SEND_RC6
    irsend.sendCOOLIX(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 21) // GREE
  {
    vCod.toUpperCase();
    //irsend.sendGree((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);
    uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
    irsend.sendGree(vCodigo, QtdeBit);
  }
  else
  {
    uint16_t rawData[QtdeBit];
    for (int i = 0; i <= QtdeBit - 1; i++)
    {
      rawData[i] = 0;
    }

    int tam2 = 0;
    String CodAtu = "";

    SPIFFS.begin();
    File rFile = SPIFFS.open("/ir_" + vCod + ".cfg", "r");
    int Qtde;
    Qtde = 0;
    while (rFile.available())
    {
      String linhas = rFile.readStringUntil('\n');
      //if (DEBUG_ON) Serial.print(linhas);

      for (int i = 1; i <= linhas.length(); i++)
      {
        if (linhas[i] != ',' && linhas[i] != '}')
        {
          CodAtu += linhas[i];
        }
        else
        {
          rawData[tam2] = CodAtu.toInt();
          //if (DEBUG_ON) Serial.println(String(CodAtu.toInt()));
          tam2++;
          CodAtu = "";
        }
      }
      Qtde++;
    }

    rFile.close();
    SPIFFS.end();
    //#if SEND_RAW
    irsend.sendRaw(rawData, QtdeBit, 38);
    if (DEBUG_ON)
      Serial.println("enviado");
    //#endif
  }

  digitalWrite(16, LOW);
}

uint64_t getUInt64fromHex(char const *str)
{
  uint64_t result = 0;
  uint16_t offset = 0;
  // Skip any leading '0x' or '0X' prefix.
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    offset = 2;
  for (; isxdigit((unsigned char)str[offset]); offset++)
  {
    char c = str[offset];
    result *= 16;
    if (isdigit(c))
      result += c - '0'; // '0' .. '9'
    else if (isupper(c))
      result += c - 'A' + 10; // 'A' .. 'F'
    else
      result += c - 'a' + 10; // 'a' .. 'f'
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

void LoopIR()
{
  if (enReadIR)
  {
    if (irrecv.decode(&results))
    {
      if (DEBUG_ON)
        Serial.print(resultToHumanReadableBasic(&results));
      dumpCode(&results);
      dumpInfo(&results);
      if (DEBUG_ON)
        Serial.println(""); // Blank line between entries
      //tone(Buzzer, 4000, 800);
      chip3.write(Buzzer, HIGH);
      delay(300);
      chip3.write(Buzzer, LOW);
      //noTone(Buzzer);
      enReadIR = false;
      irrecv.resume();
    }
  }
}