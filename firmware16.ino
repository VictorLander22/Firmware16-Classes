#define Placa_Version "2.41"

#include "globalvar.h"

//########################################################################################################################################################
void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Wire.begin(5, 4);
  Wire.setClock(100000L);
  delay(300); //Wait for start I2C transmission
  (!DEBUG_ON) ?: Serial.println();

  scanI2c();

  chip1.begin();
  chip2.begin();
  chip3.begin();
  sensor1.begin();
  sensor2.begin();
  UpdateDisplay(F("Devices Started"));
  ApagaPortas();
  UpdateDisplay(F("Ports Initialized"));
  IniciaRTC();
  UpdateDisplay(F("RTC Initialized"));
  CheckSPIFFS();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  String Razao = ESP.getResetReason();
  (!DEBUG_ON) ?: Serial.print("Motivo Reset: ");
  (!DEBUG_ON) ?: Serial.println(Razao);
  UpdateDisplay("Rst Mod: " + Razao);
  delay(1000); //esperar para começar.. permite o monitoramento logo no inicio ao desligar a placa

  //Set chip id
  vchipId = ESP.getChipId();
  gchipId = WiFi.macAddress();
  gchipId.replace(":", "");
  UpdateDisplay(F("MAC OK"));
  (!DEBUG_ON) ?: Serial.println();
  (!DEBUG_ON) ?: Serial.println("Keepin Firmware: " + String(Placa_Version));
  (!DEBUG_ON) ?: Serial.println("Keepin ID: " + vchipId);
  (!DEBUG_ON) ?: Serial.println("Keepin MAC: " + gchipId);

  //Reset mode
  if (digitalRead(buttonState))
  {
    (!DEBUG_ON) ?: Serial.println();
    (!DEBUG_ON) ?: Serial.println(F("Factory reset\n"));
    UpdateDisplay(F("Factory Reset"));
    DevSet.factoryReset();
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println();
    (!DEBUG_ON) ?: Serial.println(F("Simple restart\n"));
    UpdateDisplay(F("Simple restart"));
  }

  UpdateDisplay(F("Checking EEPROM"));
  DevSet.verifyEEPROM();
  convertConfig();
  UpdateDisplay(F("Loading settings"));
  DevSet.getDeviceSettings();
  if (DEBUG_ON)
    DevSet.showVariables();

  www_username = DevSet.httpUser.c_str();
  www_password = DevSet.httpPwd.c_str();

  UpdateDisplay(F("Configuring IR"));
  configIR(); //consome 2K da ram 20000

  UpdateDisplay(F("Loading user configuration"));
  lerConfiguracao();

  millisAtual = millis();
  lastDebounceTime = millisAtual;
  starTime = millisAtual;
  rfmilis = millisAtual;
  millisWifiLed = millisAtual;

  UpdateDisplay(F("Configuring RF"));
  configRF();

  //verificar se ha necessidade de colocar um delay aqui para evitar dos relés abrirem e fecharem muito rapido
  UpdateDisplay(F("Loading Outputs"));
  Memoria();

  UpdateDisplay(F("Getting Networks"));
  scanningWifi = WiFi.scanNetworks();

  //WiFi.scanNetworksAsync(prinScanResult);
  (!DEBUG_ON) ?: Serial.printf("\nAvailable Wifi: %d\n", scanningWifi);
  UpdateDisplay(F("Connecting Wifi"));
  conectar(); //consome 1K da ram 19000
  // Wait for connection
  (!DEBUG_ON) ?: Serial.print(F("Connected... IP address: "));
  (!DEBUG_ON) ?: Serial.println(WiFi.localIP());

  UpdateDisplay(F("Configuring WebServer"));
  ConfigurarWebServer(); //consome 6.2K da ram 13500

  retornaNotificar();

  (!DEBUG_ON) ?: Serial.println("Notificar: " + String(notificar));

  UpdateDisplay(F("Loading Inputs"));
  CarregaEntradas();
  UpdateDisplay(F("Configuring MQTT"));
  MqttSetup(); //consome 2k da ram 11400
  UpdateDisplay(F("Configuring UDP"));
  SetupUDP();

  UpdateDisplay(F("Checking Internet"));
  SetupPing();

  LoopPing();

  timer.attach(30, LoopPing);

  dispText[0] = "INITIALING KEEPIN";
  UpdateDisplay(F("Setup finished!!!"));
}

//########################################################################################################################################################
void loop(void)
{

  //(!DEBUG_ON) ?:   Serial.println(String(Rtc.alarm_minute));
  nCiclos++;

  millisAtual = millis();

  if (chipAtivo == true) //Verifica se o chip é reconhecido com um chip válido
  {
    LoopLedStatus();

    LoopReconexao();

    HorarioAtual = carregaHora();

    LoopCloud();

    LoopAgenda();

    LoopLedRunning();

    LoopResetFabrica();

    //mesh_node.acceptRequest();
    //leituraUDP();

    trataSensores();

    LoopIR();

    LoopRF();

    checkCena();

    MqttLoop();

    MillisResets();

    ResetSaidasPulsadas();

    ExecuteUpdate();

    //cloud1();

    LoopDisplay();

    FreeMemory(F("loop()"));
  }
}
