#define Placa_Version "2,37"

#include "globalvar.h"

//########################################################################################################################################################
void setup(void)
{
  Serial.begin(115200);
  //ConfigAuth();

  log("");
  log("Keepin Firmware: " + String(Placa_Version));

  vchipId = WiFi.macAddress();
  vchipId.replace(":", "");
  log("Keepin ID: " + vchipId);

  configIR();

  lerConfiguracao();

  millisAtual = millis();
  lastDebounceTime = millisAtual;

  //tempoatual = millisAtual;
  starTime = millisAtual;
  rfmilis = millisAtual;

  configRF();

  pinMode(buttonState, INPUT);

  Wire.begin(5, 4);
  //portawire.begin(5, 4);
  portawire.setClock(100000L);
  chip1.begin();
  chip2.begin();
  chip3.begin();
  sensor1.begin();
  sensor2.begin();

  ApagaPortas();

  IniciaRTC();

  conectar();

  // Wait for connection
  Serial.println("");
  Serial.print("Connected to ");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lastWifiTime = millisAtual;
  //lastPulso = millisAtual;

  //ArduinoOTA.begin();
  ConfigurarWebServer();

  Udp.begin(localUdpPort);
  Serial.printf("UDP ativo em IP %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  //Serial.println("Mesh ativo " + String(ESP.getChipId()));

  String Razao = ESP.getResetReason();

  Serial.println("Motivo Reset: ");
  Serial.println(Razao);

  retornaNotificar();

  Serial.println("Notificar: " + String(notificar));

  TipoMemoria = "1"; //lerMemoria();

  Memoria();
  CarregaEntradas();
  MqttSetup();

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

    //ArduinoOTA.handle();
    server.handleClient();

    //mesh_node.acceptRequest();
    leituraUDP();

    trataSensores();

    LoopIR();

    LoopRF();

    checkCena();

    MqttLoop();

    ResetSaidasPulsadas();
  }
}
