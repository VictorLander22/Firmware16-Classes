void configIR()
{

  save.rawbuf = new uint16_t[irrecv.getBufSize()];
  if (save.rawbuf == NULL)
  {
    (!DEBUG_ON) ?: Serial.printf("Não foi possível alocar %d de tamanho de buffer.\n"
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
  (!DEBUG_ON) ?: Serial.print("\nDecode type: ");
  (!DEBUG_ON) ?: Serial.println(results->decode_type);
  switch (results->decode_type)
  {
  default:
  case UNKNOWN:
    //(!DEBUG_ON) ?: Serial.print("UNKNOWN");
    Modelo = 0;
    break;
  case NEC:
    //(!DEBUG_ON) ?: Serial.print("NEC");
    Modelo = 1;
    break;
  case NEC_LIKE:
    //(!DEBUG_ON) ?: Serial.print("NEC (non-strict)");
    Modelo = 2;
    break;
  case SONY:
    //(!DEBUG_ON) ?: Serial.print("SONY");
    Modelo = 3;
    break;
  case RC5:
    //(!DEBUG_ON) ?: Serial.print("RC5");
    Modelo = 4;
    break;
  case RC5X:
    //(!DEBUG_ON) ?: Serial.print("RC5X");
    Modelo = 5;
    break;
  case RC6:
    //(!DEBUG_ON) ?: Serial.print("RC6");
    Modelo = 6;
    break;
  case RCMM:
    //(!DEBUG_ON) ?: Serial.print("RCMM");
    Modelo = 7;
    break;
  case DISH:
    //(!DEBUG_ON) ?: Serial.print("DISH");
    Modelo = 8;
    break;
  case SHARP:
    //(!DEBUG_ON) ?: Serial.print("SHARP");
    Modelo = 9;
    break;
  case JVC:
    //(!DEBUG_ON) ?: Serial.print("JVC");
    Modelo = 10;
    break;
  case SANYO:
    //(!DEBUG_ON) ?: Serial.print("SANYO");
    Modelo = 11;
    break;
  case SANYO_LC7461:
    //(!DEBUG_ON) ?: Serial.print("SANYO_LC7461");
    Modelo = 12;
    break;
  case MITSUBISHI:
    //(!DEBUG_ON) ?: Serial.print("MITSUBISHI");
    Modelo = 13;
    break;
  case SAMSUNG:
    //(!DEBUG_ON) ?: Serial.print("SAMSUNG");
    Modelo = 14;
    break;
  case LG:
    //(!DEBUG_ON) ?: Serial.print("LG");
    Modelo = 15;
    break;
  case WHYNTER:
    //(!DEBUG_ON) ?: Serial.print("WHYNTER");
    Modelo = 16;
    break;
  case AIWA_RC_T501:
    //(!DEBUG_ON) ?: Serial.print("AIWA_RC_T501");
    Modelo = 17;
    break;
  case PANASONIC:
    //(!DEBUG_ON) ?: Serial.print("PANASONIC");
    Modelo = 18;
    break;
  case DENON:
    //(!DEBUG_ON) ?: Serial.print("DENON");
    Modelo = 19;
    break;
  case COOLIX:
    //(!DEBUG_ON) ?: Serial.print("COOLIX");
    Modelo = 20;
    break;
  case GREE:
    //(!DEBUG_ON) ?: Serial.print("GREE");
    Modelo = 21;
    break;
  }
  if (results->repeat)
    (!DEBUG_ON) ?: Serial.print(" (Repeat)");
}

// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
  if (results->overflow)
    (!DEBUG_ON) ?: Serial.printf("WARNING: IR code too big for buffer (>= %d). "
                                 "These results shouldn't be trusted until this is resolved. "
                                 "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                                 CAPTURE_BUFFER_SIZE);

  // Show Encoding standard
  //(!DEBUG_ON) ?: Serial.print("Encoding  : ");
  encoding(results);
  //(!DEBUG_ON) ?: Serial.println("");

  // Show Code & length
  //(!DEBUG_ON) ?: Serial.print("Code      : ");
  serialPrintUint64(results->value, 10);

  //(!DEBUG_ON) ?: Serial.print(" (");
  //if (Modelo == 3 || Modelo == 1 || Modelo == 14)
  if (Modelo > 0 && Modelo < 22)
  {
    tamanho = String(results->bits, DEC).toInt();
  }
  //  (!DEBUG_ON) ?:   Serial.print(results->bits, DEC);
  //  (!DEBUG_ON) ?:   Serial.println(" bits)");
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
  (!DEBUG_ON) ?: Serial.print("Timing[");
  (!DEBUG_ON) ?: Serial.print(results->rawlen - 1, DEC);
  (!DEBUG_ON) ?: Serial.println("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    if (i % 100 == 0)
      yield(); // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0)
    { // even
      (!DEBUG_ON) ?: Serial.print("-");
    }
    else
    { // odd
      (!DEBUG_ON) ?: Serial.print("   +");
    }
    (!DEBUG_ON) ?: Serial.printf("%6d", results->rawbuf[i] * RAWTICK);
    if (i < results->rawlen - 1)
      (!DEBUG_ON) ?: Serial.print(", "); // ',' not needed for last one
    if (!(i % 8))
      (!DEBUG_ON) ?: Serial.println("");
  }
  (!DEBUG_ON) ?: Serial.println(""); // Newline
}

// Dump out the decode_results structure.
//

void dumpCode(decode_results *results)
{

  // Start declaration
  String codigoIR2 = "";
  (!DEBUG_ON) ?: Serial.print("uint16_t "); // variable type
  (!DEBUG_ON) ?: Serial.print("rawData[");  // array name
  tamanho = String(getCookedLength(results), DEC).toInt();
  (!DEBUG_ON) ?: Serial.print(getCookedLength(results), DEC); // array size
  codigoIR2 = "{";
  (!DEBUG_ON) ?: Serial.print("] = {"); // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * RAWTICK;
         usecs > UINT16_MAX;
         usecs -= UINT16_MAX)
    {
      (!DEBUG_ON) ?: Serial.printf("%d, 0", UINT16_MAX);
      codigoIR2 += printf("%d, 0", UINT16_MAX);
    }
    (!DEBUG_ON) ?: Serial.print(usecs, DEC);
    codigoIR2 += String(usecs, DEC);
    if (i < results->rawlen - 1)
    {
      (!DEBUG_ON) ?: Serial.print(", "); // ',' not needed on last one
      codigoIR2 += ", ";
    }
    if (i % 2 == 0)
    {
      (!DEBUG_ON) ?: Serial.print(" "); // Extra if it was even.
      codigoIR2 += " ";
    }
  }

  // End declaration
  (!DEBUG_ON) ?: Serial.print("};"); //
  codigoIR2 += "}";
  //  (!DEBUG_ON) ?:   Serial.println("tamanho");
  // (!DEBUG_ON) ?:   Serial.println(String(tamanho));
  //(!DEBUG_ON) ?:   Serial.println("codigoIR2");
  //(!DEBUG_ON) ?:   Serial.println(codigoIR2);

  // Comment
  //(!DEBUG_ON) ?: Serial.print("  // ");
  encoding(results);
  //(!DEBUG_ON) ?: Serial.print(" ");
  //serialPrintUint64(results->value, HEX);
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
  (!DEBUG_ON) ?: Serial.println("CodigoIR: " + codigoIR);

  (!DEBUG_ON) ?: Serial.println("Modelo: " + String(Modelo));
  if (Modelo == 0)
  {
    SPIFFS.begin();
    File rFile = SPIFFS.open("/ir_" + codigoIR + ".cfg", "w");
    rFile.println(codigoIR2);
    rFile.close();
    SPIFFS.end();

    (!DEBUG_ON) ?: Serial.print("arquivo: ");
    (!DEBUG_ON) ?: Serial.println(codigoIR2);
  }

  // Newline
  (!DEBUG_ON) ?: Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN)
  {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0)
    {
      (!DEBUG_ON) ?: Serial.print("uint32_t address = 0x");
      (!DEBUG_ON) ?: Serial.print(results->address, HEX);
      (!DEBUG_ON) ?: Serial.println(";");
      (!DEBUG_ON) ?: Serial.print("uint32_t command = 0x");
      (!DEBUG_ON) ?: Serial.print(results->command, HEX);
      (!DEBUG_ON) ?: Serial.println(";");
    }

    // All protocols have data
    (!DEBUG_ON) ?: Serial.print("uint64_t data = 0x");
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

      /*      (!DEBUG_ON) ?:   Serial.println("");
      (!DEBUG_ON) ?:   Serial.print("Codigo 64: ");
      (!DEBUG_ON) ?:   Serial.println(codigoIR);

      (!DEBUG_ON) ?:   Serial.print("Tamanho: ");
      (!DEBUG_ON) ?:   Serial.println(tamanho);*/
    }
    (!DEBUG_ON) ?: Serial.println(";");
  }
}

void getIR()
{
  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();

  gRequest->send(200, "text/html", String(tamanho) + "|" + String(Modelo) + "|" + codigoIR + "*");
  tamanho = -1;
  codigoIR = "-1";
}

void AsyncSendIR()
{
  String S = gRequest->arg("s");
  irNumBits = gRequest->arg("q").toInt();
  irModel = gRequest->arg("m").toInt();
  irPort = gRequest->arg("p").toInt();
  irData = gRequest->arg("c") + gRequest->arg("c2");
  if (S == Senha)
    irEnSend = true;
  (!DEBUG_ON) ?: Serial.println(F("Enviar IR..."));
  gRequest->send(200, "text/html", "ok");
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
    (!DEBUG_ON) ?: Serial.println("Modelo: " + String(vModelo));
    (!DEBUG_ON) ?: Serial.print("tamanho: ");
    (!DEBUG_ON) ?: Serial.println(String(QtdeBit));
    (!DEBUG_ON) ?: Serial.print("Codigo1: ");
    (!DEBUG_ON) ?: Serial.println(Codigo);
    (!DEBUG_ON) ?: Serial.print("Codigo2: ");
    (!DEBUG_ON) ?: Serial.println(Codigo2);
    if (vModelo == 1) // NEC
    {
      Codigo = "0x" + Codigo;
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
      irsend.sendNEC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 2) // NEC (non-strict)
    {
      Codigo = "0x" + Codigo;
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
      irsend.sendNEC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 3) //Sony
    {
      Codigo = "0x" + Codigo;
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
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
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SHARP
      irsend.sendSharp(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 10) // JVC
    {
      Codigo = "0x" + Codigo;
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
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
      ///(!DEBUG_ON) ?:   Serial.println(Codigo);
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
      ////(!DEBUG_ON) ?:   Serial.println(Codigo);
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
      //(!DEBUG_ON) ?:   Serial.println("DENON OK");
      //(!DEBUG_ON) ?:   Serial.println(vCodigo);
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

      //serialPrintUint64(vCodigo,10);

      //irsend.sendGree(0x0309205000200080, 64);
      irsend.sendGree(vCodigo, QtdeBit);
      // (!DEBUG_ON) ?: Serial.println("é nesta rotina");
      // (!DEBUG_ON) ?: Serial.println("GREE");
      // (!DEBUG_ON) ?: Serial.println(Codigo);
      // (!DEBUG_ON) ?: Serial.println("Qtde Bit");
      // (!DEBUG_ON) ?: Serial.println(QtdeBit);
    }
    else if (vModelo == 23) // Raw
    {
      IRSendRaw((char *)Codigo.c_str(), QtdeBit);
    }
    else
    {
      String linhas = "";
      SPIFFS.begin();
      File rFile = SPIFFS.open("/ir_" + Codigo + ".cfg", "r");
      if (rFile.available())
      {
        linhas = rFile.readStringUntil('\n');
        (!DEBUG_ON) ?: Serial.println(F("[rawData]"));
        (!DEBUG_ON) ?: Serial.print(linhas);
      }
      rFile.close();
      SPIFFS.end();

      IRSendRaw((char *)linhas.c_str(), QtdeBit);
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

  //(!DEBUG_ON) ?:   Serial.println("Codigo Recebido: " + codig);

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
      //(!DEBUG_ON) ?:   Serial.println(String(CodAtu.toInt()));
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
      //(!DEBUG_ON) ?:   Serial.println(String(CodAtu.toInt()));
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
  //(!DEBUG_ON) ?:   Serial.println("O que está gravado: " + Teste);
  */

//return rawData;
/*}*/

void habir()
{

  // if (!gRequest->authenticate(www_username, www_password))
  //   return gRequest->requestAuthentication();
  gRequest->send(200, "text/html", "ok");
  irrecv.resume();
  irrecv.resume();
  irrecv.resume();

  enReadIR = true;
  millisIREnabled = millisAtual;
  (!DEBUG_ON) ?: Serial.println("IR Receiving Enabled");
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

// void sendirAPI(int vQt, int vMd, String vCod, String vCod2, int vPt)
// {
//   int QtdeBit = vQt;
//   int PortaIRS = vPt;
//   int vModelo = vMd;
//   //PortaIRS = retornaPorIRS(PortaIRS);
//   configuraPortaIR(PortaIRS);

//   //rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);
//   //uint16_t rawData[QtdeBit] = strtol(Codigo.c_str(), NULL, 10);

//   IRsend irsend(16, true);
//   irsend.begin();
//   (!DEBUG_ON) ?: Serial.println("Modelo: " + String(vModelo));
//   (!DEBUG_ON) ?: Serial.println("tamanho");
//   (!DEBUG_ON) ?: Serial.println(String(QtdeBit));
//   if (vModelo == 1) // NEC
//   {
//     vCod = "0x" + vCod;
//     //    (!DEBUG_ON) ?:   Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     // uint64_t vCodigo2 = vCodigo;
//     //#if SEND_NEC
//     irsend.sendNEC(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 2) // NEC (non-strict)
//   {
//     vCod = "0x" + vCod;
//     ///(!DEBUG_ON) ?:   Serial.println(Codigo);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //uint64_t vCodigo2 = vCodigo;
//     //#if SEND_NEC
//     irsend.sendNEC(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 3) //Sony
//   {
//     vCod = "0x" + vCod;
//     //(!DEBUG_ON) ?:   Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     // uint64_t vCodigo2 = vCodigo;
//     // #if SEND_SONY
//     irsend.sendSony(vCodigo, QtdeBit, 2);
//     // #endif
//   }
//   else if (vModelo == 4) //RC5
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     irsend.sendRC5(vCodigo, QtdeBit, 2);
//   }
//   else if (vModelo == 5) //RC5X
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     irsend.sendRC5(vCodigo, QtdeBit, 2);
//   }
//   else if (vModelo == 6) //RC6
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendRC6(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 7) // RCMM
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendRCMM(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 8) // DISH
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendDISH(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 9) // SHARP
//   {
//     vCod = "0x" + vCod;
//     (!DEBUG_ON) ?: Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //  uint64_t vCodigo2 = vCodigo;
//     //#if SEND_SHARP
//     irsend.sendSharp(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 10) // JVC
//   {
//     vCod = "0x" + vCod;
//     (!DEBUG_ON) ?: Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     // uint64_t vCodigo2 = vCodigo;
//     //#if SEND_JVC
//     irsend.sendJVC(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 13) // MITSUBISHI
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendMitsubishi(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 14) // SAMSUNG
//   {
//     vCod = "0x" + vCod;
//     (!DEBUG_ON) ?: Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //   uint64_t vCodigo2 = vCodigo;
//     //#if SEND_SAMSUNG
//     irsend.sendSAMSUNG(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 15) // LG
//   {
//     vCod = "0x" + vCod;
//     (!DEBUG_ON) ?: Serial.println(vCod);
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //    uint64_t vCodigo2 = vCodigo;
//     //#if SEND_LG
//     irsend.sendLG(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 16) // WHYNTER
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendWhynter(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 17) // AIWA
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendAiwaRCT501(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 19) // DENON
//   {
//     //Codigo = Codigo;
//     vCod.toUpperCase();
//     //irsend.sendDenon((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);  //kDenon48Bits
//     uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
//     irsend.sendDenon(vCodigo, QtdeBit); //kDenon48Bits
//                                         //      (!DEBUG_ON) ?:   Serial.println("DENON OK");
//   }
//   else if (vModelo == 20) // COOLIX
//   {
//     vCod = "0x" + vCod;
//     unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
//     //#if SEND_RC6
//     irsend.sendCOOLIX(vCodigo, QtdeBit);
//     //#endif
//   }
//   else if (vModelo == 21) // GREE
//   {
//     vCod.toUpperCase();
//     //irsend.sendGree((long long unsigned)getUInt64fromHex(vCod.c_str()), QtdeBit);
//     uint64_t vCodigo = getUInt64fromHex(vCod.c_str());
//     irsend.sendGree(vCodigo, QtdeBit);
//   }
//   else if (vModelo == 23) // Raw
//   {
//     char *str = (char *)vCod.c_str();

//     const size_t bufferSize = irNumBits;
//     uint16_t arr[bufferSize];

//     char *p = strtok(str, ",{} ");
//     size_t index = 0;

//     while (p != nullptr && index < bufferSize)
//     {
//       arr[index++] = atoi(p);
//       p = strtok(NULL, ",{} ");
//     }

//     for (size_t i = 0; i < index; i++)
//     {
//       Serial.print(i);
//       Serial.print(":  ");
//       Serial.println(arr[i]);
//     }
//     irsend.sendRaw(arr, bufferSize, 38);
//   }
//   else
//   {
//     uint16_t rawData[QtdeBit];
//     for (int i = 0; i <= QtdeBit - 1; i++)
//     {
//       rawData[i] = 0;
//     }

//     int tam2 = 0;
//     String CodAtu = "";

//     SPIFFS.begin();
//     File rFile = SPIFFS.open("/ir_" + vCod + ".cfg", "r");
//     int Qtde;
//     Qtde = 0;
//     while (rFile.available())
//     {
//       String linhas = rFile.readStringUntil('\n');
//       //(!DEBUG_ON) ?:   Serial.print(linhas);

//       for (int i = 1; i <= linhas.length(); i++)
//       {
//         if (linhas[i] != ',' && linhas[i] != '}')
//         {
//           CodAtu += linhas[i];
//         }
//         else
//         {
//           rawData[tam2] = CodAtu.toInt();
//           //(!DEBUG_ON) ?:   Serial.println(String(CodAtu.toInt()));
//           tam2++;
//           CodAtu = "";
//         }
//       }
//       Qtde++;
//     }

//     rFile.close();
//     SPIFFS.end();
//     //#if SEND_RAW
//     irsend.sendRaw(rawData, QtdeBit, 38);
//     (!DEBUG_ON) ?: Serial.println("enviado");
//     //#endif
//   }

//   digitalWrite(16, LOW);
// }

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
  AsyncIRSend();

  if (enReadIR)
  {
    if (irrecv.decode(&results))
    {
      (!DEBUG_ON) ?: Serial.print(resultToHumanReadableBasic(&results));
      dumpCode(&results);
      dumpInfo(&results);
      (!DEBUG_ON) ?: Serial.println(""); // Blank line between entries
      //tone(Buzzer, 4000, 800);
      chip3.write(Buzzer, HIGH);
      delay(300);
      chip3.write(Buzzer, LOW);
      //noTone(Buzzer);
      enReadIR = false;
      (!DEBUG_ON) ?: Serial.println("IR Receiving Disabled");
      irrecv.resume();
    }
    else if (millisAtual > millisIREnabled + 30000)
    {
      enReadIR = false;
      (!DEBUG_ON) ?: Serial.println("IR Receiving Disabled");
      irrecv.resume();
    }
  }
}

void IRSendRaw(char *str, const size_t bufferSize)
{
  uint16_t arr[bufferSize];

  char *p = strtok(str, ",{} ");
  size_t index = 0;

  while (p != nullptr && index < bufferSize)
  {
    arr[index++] = atoi(p);
    p = strtok(NULL, ",{} ");
  }
  // for (size_t i = 0; i < index; i++)
  // {
  //   (!DEBUG_ON) ?: Serial.print(i);
  //   (!DEBUG_ON) ?: Serial.print(": ");
  //   (!DEBUG_ON) ?: Serial.println(arr[i]);
  // }
  irsend.sendRaw(arr, bufferSize, 38);
}