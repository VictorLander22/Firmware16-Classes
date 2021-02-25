#define Placa_Version "2,38"

#include "globalvar.h"

//########################################################################################################################################################
void setup(void)
{
  Serial.begin(115200);
  Wire.begin(5, 4);
  Wire.setClock(100000L);
  delay(100); //Wait to start I2C transmission
  Serial.println();
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
  Serial.print("Motivo Reset: ");
  Serial.println(Razao);

  //Set chip id
  vchipId = ESP.getChipId();
  gchipId = WiFi.macAddress();
  gchipId.replace(":", "");

  Serial.println();
  Serial.println("Keepin Firmware: " + String(Placa_Version));
  Serial.println("Keepin ID: " + vchipId);
  Serial.println("Keepin MAC: " + gchipId);

  //Reset mode
  if (digitalRead(buttonState))
  {
    Serial.println();
    Serial.println(F("Factory reset\n"));
    DevSet.factoryReset();
    //wifireset2();
  }
  else
  {
    Serial.println();
    Serial.println(F("Simple restart\n"));
  }
  DevSet.verifyEEPROM();
  DevSet.getDeviceSettings();
  DevSet.showVariables();

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
  Serial.printf("\nAvailable Wifi: %d\n", scanningWifi);

  conectar(); //consome 1K da ram 19000
  // Wait for connection
  Serial.print("Connected to ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ConfigurarWebServer(); //consome 6.2K da ram 13500

  retornaNotificar();

  Serial.println("Notificar: " + String(notificar));

  CarregaEntradas();

  MqttSetup(); //consome 2k da ram 11400

  Udp.begin(localUdpPort);
  Serial.printf("UDP ativo em IP %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

//########################################################################################################################################################
void loop(void)
{

  //Serial.println(String(Rtc.alarm_minute));
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

    server.handleClient();

    //showDateTime();

    FreeMemory(F("loop()"));
  }
}
