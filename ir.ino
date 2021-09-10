void configIR()
{

  save.rawbuf = new uint16_t[irrecv.getBufSize()];
  if (save.rawbuf == NULL)
  {
    // Serial.printf("Não foi possível alocar %d de tamanho de buffer.\n"
    //                              "Try a smaller size for CAPTURE_BUFFER_SIZE.\nRebooting!",
    //                              irrecv.getBufSize());
    ESP.restart();
  }
  irrecv.setUnknownThreshold(kMinUnknownSize);
  irrecv.enableIRIn(); // Start the receiver
}
// Display encoding type
//
void encoding(decode_results *results)
{
  slog("\nDecode type: ");
  slogln(results->decode_type);
  switch (results->decode_type)
  {
  default:
  case UNKNOWN:
    //slog("UNKNOWN");
    Modelo = 0;
    break;
  case NEC:
    //slog("NEC");
    Modelo = 1;
    break;
  case NEC_LIKE:
    //slog("NEC (non-strict)");
    Modelo = 2;
    break;
  case SONY:
    //slog("SONY");
    Modelo = 3;
    break;
  case RC5:
    //slog("RC5");
    Modelo = 4;
    break;
  case RC5X:
    //slog("RC5X");
    Modelo = 5;
    break;
  case RC6:
    //slog("RC6");
    Modelo = 6;
    break;
  case RCMM:
    //slog("RCMM");
    Modelo = 7;
    break;
  case DISH:
    //slog("DISH");
    Modelo = 8;
    break;
  case SHARP:
    //slog("SHARP");
    Modelo = 9;
    break;
  case JVC:
    //slog("JVC");
    Modelo = 10;
    break;
  case SANYO:
    //slog("SANYO");
    Modelo = 11;
    break;
  case SANYO_LC7461:
    //slog("SANYO_LC7461");
    Modelo = 12;
    break;
  case MITSUBISHI:
    //slog("MITSUBISHI");
    Modelo = 13;
    break;
  case SAMSUNG:
    //slog("SAMSUNG");
    Modelo = 14;
    break;
  case LG:
    //slog("LG");
    Modelo = 15;
    break;
  case WHYNTER:
    //slog("WHYNTER");
    Modelo = 16;
    break;
  case AIWA_RC_T501:
    //slog("AIWA_RC_T501");
    Modelo = 17;
    break;
  case PANASONIC:
    //slog("PANASONIC");
    Modelo = 18;
    break;
  case DENON:
    //slog("DENON");
    Modelo = 19;
    break;
  case COOLIX:
    //slog("COOLIX");
    Modelo = 20;
    break;
  case GREE:
    //slog("GREE");
    Modelo = 21;
    break;
  }
  if (results->repeat)
    slog(" (Repeat)");
}

// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
  if (results->overflow)
    // Serial.printf("WARNING: IR code too big for buffer (>= %d). "
    //                              "These results shouldn't be trusted until this is resolved. "
    //                              "Edit & increase CAPTURE_BUFFER_SIZE.\n",
    //                              CAPTURE_BUFFER_SIZE);

    // Show Encoding standard
    //slog("Encoding  : ");
    encoding(results);
  //slogln("");

  // Show Code & length
  //slog("Code      : ");
  serialPrintUint64(results->value, 10);

  //slog(" (");
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
  slog("Timing[");
  slog(String(results->rawlen - 1, DEC));
  slogln("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    if (i % 100 == 0)
      yield(); // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0)
    { // even
      slog("-");
    }
    else
    { // odd
      slog("   +");
    }
    slog(results->rawbuf[i] * RAWTICK);
    if (i < results->rawlen - 1)
      slog(", "); // ',' not needed for last one
    if (!(i % 8))
      slogln("");
  }
  slogln(""); // Newline
}

// Dump out the decode_results structure.
//

void dumpCode(decode_results *results)
{

  // Start declaration
  String codigoIR2 = "";
  slog("uint16_t "); // variable type
  slog("rawData[");  // array name
  tamanho = String(getCookedLength(results), DEC).toInt();
  slog(String(getCookedLength(results), DEC)); // array size
  codigoIR2 = "{";
  slog("] = {"); // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * RAWTICK;
         usecs > UINT16_MAX;
         usecs -= UINT16_MAX)
    {
      Serial.printf("%d, 0", UINT16_MAX);
      codigoIR2 += printf("%d, 0", UINT16_MAX);
    }
    slog(String(usecs, DEC));
    codigoIR2 += String(usecs, DEC);
    if (i < results->rawlen - 1)
    {
      slog(", "); // ',' not needed on last one
      codigoIR2 += ", ";
    }
    if (i % 2 == 0)
    {
      slog(" "); // Extra if it was even.
      codigoIR2 += " ";
    }
  }

  // End declaration
  slog("};"); //
  codigoIR2 += "}";
  //  (!DEBUG_ON) ?:   Serial.println("tamanho");
  // (!DEBUG_ON) ?:   Serial.println(String(tamanho));
  //(!DEBUG_ON) ?:   Serial.println("codigoIR2");
  //(!DEBUG_ON) ?:   Serial.println(codigoIR2);

  // Comment
  //slog("  // ");
  encoding(results);
  //slog(" ");
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
  slogln("CodigoIR: " + codigoIR);

  slogln("Modelo: " + String(Modelo));
  if (Modelo == 0)
  {
    SPIFFS.begin();
    File rFile = SPIFFS.open("/ir_" + codigoIR + ".cfg", "w");
    rFile.println(codigoIR2);
    rFile.close();
    SPIFFS.end();

    slog("arquivo: ");
    slogln(codigoIR2);
  }

  // Newline
  slogln("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN)
  {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0)
    {
      slog("uint32_t address = 0x");
      slog(String(results->address, HEX));
      slogln(";");
      slog("uint32_t command = 0x");
      slog(String(results->command, HEX));
      slogln(";");
    }

    // All protocols have data
    slog("uint64_t data = 0x");
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
    slogln(";");
  }
}

void getIR()
{
  gRequest->send(200, sdefTextHtml, String(tamanho) + "|" + String(Modelo) + "|" + codigoIR + "*");
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
  slogln(F("Enviar IR..."));
  gRequest->send(200, sdefTextHtml, sdefOK);
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
    slogln("Modelo: " + String(vModelo));
    slog("tamanho: ");
    slogln(String(QtdeBit));
    slog("Codigo1: ");
    slogln(Codigo);
    slog("Codigo2: ");
    slogln(Codigo2);
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
      // slogln("é nesta rotina");
      // slogln("GREE");
      // slogln(Codigo);
      // slogln("Qtde Bit");
      // slogln(QtdeBit);
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
        slogln(F("[rawData]"));
        slog(linhas);
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

void habir()
{
  gRequest->send(200, sdefTextHtml, sdefOK);
  irrecv.resume();
  irrecv.resume();
  irrecv.resume();

  enReadIR = true;
  millisIREnabled = millisAtual;
  slogln("IR Receiving Enabled");
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

void LoopIR()
{
  AsyncIRSend();

  if (enReadIR)
  {
    if (irrecv.decode(&results))
    {
      slog(resultToHumanReadableBasic(&results));
      dumpCode(&results);
      dumpInfo(&results);
      slogln(""); // Blank line between entries
      //tone(Buzzer, 4000, 800);
      chip3.write(Buzzer, HIGH);
      delay(300);
      chip3.write(Buzzer, LOW);
      //noTone(Buzzer);
      enReadIR = false;
      slogln("IR Receiving Disabled");
      irrecv.resume();
    }
    else if (millisAtual > millisIREnabled + 30000)
    {
      enReadIR = false;
      slogln("IR Receiving Disabled");
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
  //   slog(i);
  //   slog(": ");
  //   slogln(arr[i]);
  // }
  irsend.sendRaw(arr, bufferSize, 38);
}