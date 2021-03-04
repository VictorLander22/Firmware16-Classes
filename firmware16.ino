#define Placa_Version "2,39"

#include "globalvar.h"

//########################################################################################################################################################
void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Wire.begin(5, 4);
  Wire.setClock(100000L);
  delay(300); //Wait to start I2C transmission
  (!DEBUG_ON) ?: Serial.println();
  IniciaRTC();
  chip1.begin();
  chip2.begin();
  chip3.begin();
  sensor1.begin();
  sensor2.begin();
  ApagaPortas();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000); //esperar para começar.. permite o monitoramento logo no inicio ao desligar a placa
  String Razao = ESP.getResetReason();
  (!DEBUG_ON) ?: Serial.print("Motivo Reset: ");
  (!DEBUG_ON) ?: Serial.println(Razao);

  //Set chip id
  vchipId = ESP.getChipId();
  gchipId = WiFi.macAddress();
  gchipId.replace(":", "");

  (!DEBUG_ON) ?: Serial.println();
  (!DEBUG_ON) ?: Serial.println("Keepin Firmware: " + String(Placa_Version));
  (!DEBUG_ON) ?: Serial.println("Keepin ID: " + vchipId);
  (!DEBUG_ON) ?: Serial.println("Keepin MAC: " + gchipId);

  //Reset mode
  if (digitalRead(buttonState))
  {
    (!DEBUG_ON) ?: Serial.println();
    (!DEBUG_ON) ?: Serial.println(F("Factory reset\n"));
    DevSet.factoryReset();
  }
  else
  {
    (!DEBUG_ON) ?: Serial.println();
    (!DEBUG_ON) ?: Serial.println(F("Simple restart\n"));
  }

  DevSet.verifyEEPROM();
  convertConfig();
  DevSet.getDeviceSettings();
  DevSet.showVariables();

  www_username = DevSet.httpUser.c_str();
  www_password = DevSet.httpPwd.c_str();

  configIR(); //consome 2K da ram 20000

  lerConfiguracao();

  millisAtual = millis();
  lastDebounceTime = millisAtual;
  starTime = millisAtual;
  rfmilis = millisAtual;
  millisWifiLed = millisAtual;

  configRF();

  //verificar se ha necessidade de colocar um delay aqui para evitar dos relés abrirem e fecharem muito rapido
  Memoria();

  scanningWifi = WiFi.scanNetworks();

  //WiFi.scanNetworksAsync(prinScanResult);
  (!DEBUG_ON) ?: Serial.printf("\nAvailable Wifi: %d\n", scanningWifi);

  conectar(); //consome 1K da ram 19000
  // Wait for connection
  (!DEBUG_ON) ?: Serial.print("Connected to ");
  (!DEBUG_ON) ?: Serial.print("IP address: ");
  (!DEBUG_ON) ?: Serial.println(WiFi.localIP());

  ConfigurarWebServer(); //consome 6.2K da ram 13500

  retornaNotificar();

  (!DEBUG_ON) ?: Serial.println("Notificar: " + String(notificar));

  CarregaEntradas();

  MqttSetup(); //consome 2k da ram 11400

  Udp.begin(localUdpPort);
  (!DEBUG_ON) ?: Serial.printf("UDP ativo em IP %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  SetupPing();

  LoopPing();

  timer.attach(30, LoopPing);
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
    leituraUDP();

    trataSensores();

    LoopIR();

    LoopRF();

    checkCena();

    MqttLoop();

    ResetSaidasPulsadas();

    MillisResets();

    //server.handleClient();

    //showDateTime();

    FreeMemory(F("loop()"));
  }
}
