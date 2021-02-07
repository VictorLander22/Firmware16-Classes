
#include <user_interface.h>
//#ifndef UNIT_TEST
#include <Arduino.h>
//#endif
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WiFiUdp.h>
//#include <ESP8266WiFiMesh.h>
#include <pgmspace.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDateTime.h>
#include <PCF8583.h>
#include <pcf8574_esp.h>
#include <ESP8266httpUpdate.h>
//#include <ArduinoOTA.h>
#include <Seguranca.h>
//IR
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
//RF
#include <RCSwitch.h>
//FIREBASE
//#include <FirebaseArduino.h>
//#include <FirebaseCloudMessaging.h>
#include <ArduinoJson.h>

//Declarações Globais - Leo

bool g_pulsoHabilita[16];
unsigned long g_tempoInicioPulso[16];
unsigned long millisAtual;
unsigned long millisDebug;
unsigned long millisMqttReconnect;

File UploadFile;

//String CloudAddress = "http://keepin.com.br/api/"";
//Const CloudAddress = "http://192.168.15.16:4000/";

//Fim Declarações Globais - Leo

Seguranca seg;
String usuario1 = seg.retornaUsuario();
String senha1 = seg.retornaSenha();

const char *www_username = usuario1.c_str();
const char *www_password = senha1.c_str();

String TipoMemoria = "1";
String vListaWifi = "";

String vssid = "";
String vpass = "";
String vip = "";
String vmask = "";
String vgateway = "";

String vConfigWIFI = "";
long lastWifiTime;
//long lastPulso;
int tipoWifiAtual;

//MySQL_Connection conn(&cliente);

//int tempoatual;
IPAddress DNS1(8, 8, 8, 8);
IPAddress DNS2(4, 4, 4, 4);
String vSenhaAP = "12345678";

TwoWire portawire;
PCF857x chip1(0x21, &portawire);
PCF857x chip2(0x22, &portawire);
PCF857x chip3(0x25, &portawire);
PCF857x sensor1(0x23, &portawire);
PCF857x sensor2(0x24, &portawire);

String s2Sensor1 = "";
String s2Sensor2 = "";

int buttonState = 13;
int counter = 0;
int counterRTC = 0;
boolean AgendaAlterada = true;
String Agendas[6] = {"", "", "", "", "", ""};

boolean SensorAlterado = true;
String Sensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
String nomeSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
boolean msgDisparada[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
String ultimoDisparo = "";
boolean notificar = false;
boolean enviarsms = false;
boolean estadoAtual[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean ultimoEstado[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

//debounce
long lastDebounceTime = 0;
long debounceDelay = 150;
long rfDelay = 1000;

boolean DeviceAlterado = true;
//String Devices[20];

//RtcDateTime ultimaconsulta;
int Minuto = -1;
int Segundo = -1;
bool ConsultouCloud = false;

RtcDateTime HorarioAtual;

int correct_address = 0;
PCF8583 Rtc(0xA0);

IPAddress IpDispositivo;

unsigned int request_i = 0;
unsigned int response_i = 0;

//String manageRequest(String request);

//ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

//String mesh_sIP = "";
//String mesh_Porta = "";
//String mesh_funcao = "";
//String mesh_ChipId = "";
//String mesh_Tipo = "";
//String mesh_Senha = "";

//int LedWiFI = 14;
//int LedVerde = 12;
//int LedAmarelo = 15; //14;

const int LedWiFI = 4;
const int LedVerde = 5;
const int LedAmarelo = 6; //14;
const int LedGeral = 7;

//UDP
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[255];
//FIM UDP

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

int contadorled = 0;

unsigned long starTime = 0;   // Use unsigned long when dealing with millisAtual
unsigned long interval = 500; // 1000 millis = 1 second
//unsigned long Contador = 0;

//Dispositivos
String Senha = "kdi9e";

//   IR   //
uint16_t RECV_PIN = 14;
uint16_t CAPTURE_BUFFER_SIZE = 1024;
const uint16_t kMinUnknownSize = 12;
//#define TIMEOUT 15U
#if DECODE_AC
const uint8_t TIMEOUT = 50;
#else
const uint8_t TIMEOUT = 15;
#endif
//#define TIMEOUT 15U
IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
decode_results results;
irparams_t save;
int tamanho = -1;
String codigoIR = "-1";
//uint16_t rawData[500] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
IRsend irsend(16);
boolean enReadIR = false;
int Modelo = 0;
//   IR   //

//   RF  //
RCSwitch mySwitch = RCSwitch();
RCSwitch sSendRF = RCSwitch();
int rxRF = 12;
int txRF = 15;
//boolean enReadRF = false;
int tamanhoRF = -1;
int gProtocoloRF = 1;
String codigoRF = "-1";
boolean SensorRFAlterado = true;
String SensoresRF[30] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
boolean sensorRFDisparado = false;
int numSensorDisparado = -1;
String ultimoDisparoRF = "";
boolean estadoAtualRF[30] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean ultimoEstadoRF[30] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean msgDisparadaRF[30] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
int numSensorMSG = 0;
long rfmilis = 0;
//   RF  //
int Buzzer = 3;

// ----         Dados Firebase      ----- //
//#define FIREBASE_HOST "automacao-6cdcc.firebaseio.com"
//#define FIREBASE_AUTH "m7WSNbRjPJck9YhLE9AdaFKRtnG6U7ENDiUA3IUF"
//int vchipId = 0;
String vchipId;
bool chipAtivo = true;
int nCiclos = 0;
// ----         Fim Dados Firebase      ----- //

// --- API ///
bool AlowApi = false;
String ApiPass = "12345678";
int memorialivre = 0;

//   CENAS   //
bool cenaExecucao = false;
String ArqCena = "";
int cenaPAtual = 0;
int cenaPTotal = 0;
long lastCnTime = -1;

//   CLOUD ///
bool usaCloud = false;
char *msgMqtt;
bool newMqttMsg;