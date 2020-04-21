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

#define Placa_Version "2,31"

Seguranca seg;
String usuario1 = seg.retornaUsuario();
String senha1 = seg.retornaSenha();

const char* www_username = usuario1.c_str();
const char* www_password = senha1.c_str();


String TipoMemoria = "0";
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
IPAddress DNS1(8,8,8,8);
IPAddress DNS2(4,4,4,4);
String vSenhaAP = "12345678";


TwoWire portawire;
PCF857x chip1(0x21, &portawire);
PCF857x chip2(0x22, &portawire);
PCF857x chip3(0x25, &portawire);
PCF857x sensor1(0x23, &portawire);
PCF857x sensor2(0x24, &portawire);

String s2Sensor1 = "";
String s2Sensor2 = "";

int buttonState  = 13;
int counter = 0;  
int counterRTC = 0;
boolean AgendaAlterada = true;
String Agendas[6] = { "", "", "", "", "", "" };

boolean SensorAlterado = true;
String Sensores[16] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
String nomeSensores[16] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
boolean msgDisparada[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
String ultimoDisparo = "";
boolean notificar = false;
boolean enviarsms = false;
boolean estadoAtual[16] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean ultimoEstado[16] =  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };

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

int correct_address = 0;
PCF8583 Rtc (0xA0); 


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

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);


ESP8266WebServer server(80);

int contadorled = 0;

unsigned long starTime = 0; // Use unsigned long when dealing with millis()
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
String codigoRF = "-1";
boolean SensorRFAlterado = true;
String SensoresRF[30] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
boolean sensorRFDisparado = false;
int numSensorDisparado = -1;
String ultimoDisparoRF = "";
boolean estadoAtualRF[30] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean ultimoEstadoRF[30] =  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean msgDisparadaRF[30] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
int numSensorMSG = 0;
long rfmilis = 0;
//   RF  //
int Buzzer = 3;

// ----         Dados Firebase      ----- //
//#define FIREBASE_HOST "automacao-6cdcc.firebaseio.com"
//#define FIREBASE_AUTH "m7WSNbRjPJck9YhLE9AdaFKRtnG6U7ENDiUA3IUF"
int vchipId = 0;
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

void setup(void){
  Serial.begin(115200);
  //ConfigAuth();
   
  configIR();

  lerConfiguracao();

  lastDebounceTime = millis();

  //pinMode(LedWiFI, OUTPUT);
  //pinMode(LedVerde, OUTPUT);
  //pinMode(LedAmarelo, OUTPUT);

  //digitalWrite(LedWiFI, LOW);  
  //digitalWrite(LedVerde, LOW);  
  //digitalWrite(LedAmarelo, LOW); 

  //tempoatual = millis();  
  starTime = millis();  
  rfmilis = millis();  

  configRF();  
  
  //pinMode(buttonState, OUTPUT);
  //digitalWrite(buttonState, LOW);
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

  //seg.gravar("keepin", "keepin");

 
  //verificaArquivos(); // limpa o arquivo da agenda.
  
// para sensor ultrasonico
  /// pinMode(led, OUTPUT);
  ///pinMode(led2, INPUT);
  ///pinMode(BUILTIN_LED, OUTPUT);

  
  
  conectar();  

  // Wait for connection
  Serial.println("");
  Serial.print("Connected to ");
///  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }

  lastWifiTime = millis();
  //lastPulso = millis();  
  
  //ArduinoOTA.begin();

  server.on("/", configuracao);
  server.on ( "/grava", grava );
  server.on ( "/ler", ler );
  server.on ( "/config", configuracao);
  server.on ( "/gravarwifi", gravawifi );
  server.on("/gravasenhawifi", gravasenhawifi);
  server.on("/gravasenhahttp", gravasenhahttp);
  server.on ( "/reset", wifireset );
  server.on ( "/reiniciar", reiniciar );
  server.on ( "/valida", valida );
  server.on ( "/controle", controle );
  server.on ( "/situacao", situacao );
  server.on ( "/chipid", retornachip );
  server.on ( "/chamaddns", chamaddns );
  //server.on("/mesh", mesh);
  //server.on("/consultamesh", meshconsulta);
  server.on("/consultaagenda", conagenda);
  server.on("/gravaragenda", gravaragenda);
  server.on("/atualizahora", atualizahora);
  server.on("/lersensores", lersensores);
  server.on("/gravasensor", gravasensor);
  server.on("/consultasensor", consensor);
  server.on("/gravadevice", gravadevice);
  server.on("/buscadevice", buscadevice);
  server.on("/executeupdate", executeupdate);
  server.on("/executeupdatebeta", executeupdateBeta);
  server.on("/versao", versao);
  server.on("/link", linkversao);
  server.on("/link", linkversaoBeta);
  //server.on("/limpadevice", limpadevice);
  server.on("/ultimodisparo", ultimodisp);
  server.on("/buscaNotificar", buscaNotificar);
  server.on("/gravanot", gravanot);
  server.on("/gravasms", gravasms);
  server.on("/consultasms", consultasms);
  server.on("/wifi", valorwifi);
  server.on("/listawifi", listawifi);
  server.on("/listawifi2", listawifi2);
  //IR
  server.on( "/getir", getIR);  
  server.on("/sendir", sendir);  
  server.on("/habir", habir);  
//RF
  server.on("/habrf", habRF);  
  server.on( "/getrf", getRF);
  server.on("/gravarf", gravarf);  
  server.on("/ultimodisparorf", ultimodisprf);
  server.on("/sendrf", sendRFp);
  server.on("/modelo", fmodelo);
  server.on("/memoria", fMemoria);
  server.on("/html", gravahtml);
  //server.on("/teste", testes2);
  server.on("/api", api);
  server.on("/apiativo", apiativo);
  server.on("/apiconfig", apiconfig);
  server.on("/alterasenhapi", alterasenhapi);
  server.on("/about", about);
  server.on("/gravacena", gravacena);
  server.on("/log", readlog);
  server.on("/gravacloud", GravaCloud);
  //server.on("/cloud", cloud);
//  server.on("/sendcloud", sendCloud);
  
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Udp.begin(localUdpPort); 
  Serial.printf("UDP ativo em IP %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  
  //Serial.println("Mesh ativo " + String(ESP.getChipId()));

  String Razao = ESP.getResetReason();

  Serial.println("Motivo Reset: ");
  Serial.println(Razao);
  
  retornaNotificar;
  
  Serial.println("Notificar: " + String(notificar));

  TipoMemoria = lerMemoria();

  Memoria();
  CarregaEntradas();

  

 /*
  Rtc.chip1 = 225;
  Rtc.chip2 = 130;
  Rtc.set_chip1();
  Rtc.set_chip2();
  Rtc.get_chip1();
  Rtc.get_chip2();
  Serial.println(String(Rtc.chip1));
  Serial.println(String(Rtc.chip2));
  */


  // Inicializa Fireabase
  vchipId = ESP.getChipId();
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.setString("chip_", "ok");
  //Firebase.setString("chip_" + String(vchipId)+"/chip", String(vchipId));      
 // set string value
//  Firebase.setString("message", "hello world");
  // handle error
  //if (Firebase.failed()) {
      //Serial.print("setting /message failed:");
      //Serial.println(Firebase.error());  
      //return;
  //}
  //delay(1000);

}

void loop(void){
  
  //Serial.println(chip3.read8());
  //Serial.println(String(Rtc.alarm_minute));
  nCiclos++;
  if (chipAtivo == true) {

    if (chip3.read(LedWiFI) == LOW)
    {
      int32_t rssi;
    
//      chip3.write(LedAmarelo, LOW);      
      if (WiFi.status() == WL_CONNECTED)
      {
        if (vConfigWIFI == "0") 
        {      
          lastWifiTime = millis();
        }
        rssi = WiFi.RSSI();
        //Serial.println(String(rssi));
    
        if (rssi >= -65)
        {
          chip3.write(LedVerde, LOW);
          chip3.write(LedAmarelo, HIGH);
        }
        else
        if (rssi < -65 && rssi >= -100)
        {
          chip3.write(LedVerde, HIGH);
          chip3.write(LedAmarelo, LOW);      
        }
        else
        {
          chip3.write(LedVerde, HIGH);
          chip3.write(LedAmarelo, HIGH);            
        }
      }   
    }
  
  // reconexao
      if ((vConfigWIFI == "0" && WiFi.status() != WL_CONNECTED) || (vConfigWIFI == "0" && tipoWifiAtual == 2))
      {
        if ((millis() - lastWifiTime) >= 300000) 
        {
          conectar();
          lastWifiTime = millis();
        }
      }
  

  // valida IP Reconexao
      if ((vConfigWIFI == "0" && tipoWifiAtual != 2 && WiFi.localIP() != local_IP))
      {
        conectar();
      }

    //Serial.println("Tempo: " + String(millis() - tempoatual));
  
    
    //Serial.println(String(sensor1.read8(), BIN));
    //Serial.println(sensor1.read(0));
    RtcDateTime HorarioAtual;
    HorarioAtual = carregaHora();

    // só acessa o cloud se estiver marcado para usar.
    if (usaCloud == true)
    {
      if (HorarioAtual.Second() % 5 == 0 && ConsultouCloud == false)
      {
        ConsultouCloud = true;
  //      Serial.println("Segundo Atual: " + String(HorarioAtual.Second()));
        sendCloud();
      } else if (HorarioAtual.Second() % 5 != 0 && ConsultouCloud == true) {
        ConsultouCloud = false;
      }
    }

    if (HorarioAtual.Minute() != Minuto)
    {
      Serial.println("Ciclos por segundos: " + String(nCiclos/60));
      Serial.println("Ciclos por minuto: " + String(nCiclos));
      memorialivre = system_get_free_heap_size();
      Serial.println("memoria livre: " + String(memorialivre));
     
     /* // verifica memoria utilizada no spiff e a memoria total
      SPIFFS.begin();
      FSInfo fs_info;
      SPIFFS.info(fs_info);
      SPIFFS.end();

      Serial.println(fs_info.usedBytes);
      Serial.println(fs_info.totalBytes);
    */

      nCiclos = 0;
      Minuto = HorarioAtual.Minute();
  
      char time[50];
      sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d",  HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());
      Serial.println(time);
  
      agenda();    
    }
  
  //  if (HorarioAtual.Second() != Segundo)
    if (Segundo == -1 || (millis() - Segundo) > 200)
    {
      Segundo = millis();
      //Segundo = HorarioAtual.Second();   
      chip3.write(LedGeral, !chip3.read(LedGeral));
    }
   
   int ValorbuttonState = digitalRead(buttonState);
    
    if (ValorbuttonState == HIGH) {
      if (resetIntPulsado() == true)
      {
        ConfigEP();
        //Serial.println("reset pulsado");
      }
      else if (resetIntNormal() == true)
      {
        ConfigEN();
        //Serial.println("reset normal");
      }
      else
      {
        //Serial.print("reset ligado: ");
        //Serial.println(digitalRead(buttonState));
        wifireset2();
        ESP.restart();
      }
    }

    //ArduinoOTA.handle();  
    server.handleClient();
  
    //mesh_node.acceptRequest();
    leituraUDP();
  
    trataSensores();
  
  
    // IR
    if (enReadIR)
    {
      if (irrecv.decode(&results)) 
      { 
        Serial.print(resultToHumanReadableBasic(&results));      
        dumpCode(&results);          
        dumpInfo(&results); 
        Serial.println("");           // Blank line between entries
        //tone(Buzzer, 4000, 800);
        chip3.write(Buzzer, HIGH);
        delay(300);    
        chip3.write(Buzzer, LOW);
        //noTone(Buzzer);
        enReadIR = false;
        irrecv.resume();
      }
    }
  
    // FIM IR
  
  //RF
  //  if (enReadRF)
  //  {
      if (mySwitch.available()) 
      {  
        //Serial.println("sinal de radio detectado");
  //      output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
        if (millis() - rfmilis >= 10000 || millis() - rfmilis < 0)
        {
          rfmilis = millis();
          for (int i = 0; i < 30; i++)
          {
            ultimoEstadoRF[i] = LOW;
          }
          
        }
        int value = mySwitch.getReceivedValue();
      
        if (value == 0) 
        {
          Serial.print("Unknown encoding");
        } 
        else 
        {
      //    tone(Buzzer, 4000, 800);      
          Serial.print("Received ");
          codigoRF = mySwitch.getReceivedValue();
          ultimoDisparoRF = codigoRF;        
          Serial.print( mySwitch.getReceivedValue() );
          Serial.print(" / ");
          Serial.print( mySwitch.getReceivedBitlength() );
          tamanhoRF = mySwitch.getReceivedBitlength();
          Serial.print("bit ");
          Serial.print("Protocol: ");
          Serial.println( mySwitch.getReceivedProtocol() );
          trataRF();
          
        }
        mySwitch.resetAvailable();
  
        delay(300);    
    //    noTone(Buzzer);
      //  enReadRF = false;     
      }    
  
  
  //  }
  //FIM RF  
  

  checkCena();


/* teste para pegar o id do dispositivo (1 - android 2- iphone para enviar mensagem push notification)
  String reg = "";
  for (int i = 0; i <= 9; i++)
  {
    reg = String(Devices[i]);

    if (reg.length() > 10)
    {  
      Serial.println(reg.substring(0,1));
      Serial.println(reg.substring(1));
      Serial.println(reg);
      delay(20000);
    }
  }
  */
  }
  // firebase - Caso o valor seja falso, o sistema será bloqueado;
 // String textValorSeguranca = String(Firebase.getString("chip_" + String(vchipId)+"/chip"));
  // Serial.println(Firebase.getString("chip"));
  //Serial.println(String(vchipId));
  //if (textValorSeguranca == "")
  //{
    //Serial.println("Retorno texto: " + textValorPortao);
    //Firebase.setBool("chip_" + String(vchipId)+"/ativo", true);    
    //Firebase.setString("chip_" + String(vchipId)+"/chip", String(vchipId));
  //}
  //else
  //{
    //chipAtivo = Firebase.getBool("chip_" + String(vchipId)+"/ativo");    
  //}

//  Serial.println("memoria livre:");
//  Serial.println(system_get_free_heap_size());
  
}
