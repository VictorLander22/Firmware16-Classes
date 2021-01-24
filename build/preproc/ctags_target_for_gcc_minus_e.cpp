# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino"
# 2 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//#ifndef UNIT_TEST
# 4 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//#endif
# 6 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 7 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 8 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 9 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 10 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//#include <ESP8266WiFiMesh.h>
# 12 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 13 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 14 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 15 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 16 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 17 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//#include <ArduinoOTA.h>
# 19 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//IR
# 21 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 22 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 23 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
# 24 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//RF
# 26 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2
//FIREBASE
//#include <FirebaseArduino.h>
//#include <FirebaseCloudMessaging.h>
# 30 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino" 2



//Declarações Globais - Leo

bool g_pulsoHabilita[16];
unsigned long g_tempoInicioPulso[16];
unsigned long millisAtual;
unsigned long millisDebug;

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
boolean estadoAtual[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
boolean ultimoEstado[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

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

unsigned long starTime = 0; // Use unsigned long when dealing with millisAtual
unsigned long interval = 500; // 1000 millis = 1 second
//unsigned long Contador = 0;

//Dispositivos
String Senha = "kdi9e";

//   IR   //
uint16_t RECV_PIN = 14;
uint16_t CAPTURE_BUFFER_SIZE = 1024;
const uint16_t kMinUnknownSize = 12;
//#define TIMEOUT 15U

const uint8_t TIMEOUT = 50;



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
boolean estadoAtualRF[30] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
boolean ultimoEstadoRF[30] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
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

void setup(void)
{
  Serial.begin(115200);
  //ConfigAuth();

  log("");
  log("Keepin Firmware: " + String("2,36"));

  vchipId = WiFi.macAddress();
  vchipId.replace(":","");
  log("Keepin ID: " + vchipId);

  configIR();

  lerConfiguracao();

  millisAtual = millis();
  lastDebounceTime = millisAtual;

  //pinMode(LedWiFI, OUTPUT);
  //pinMode(LedVerde, OUTPUT);
  //pinMode(LedAmarelo, OUTPUT);

  //digitalWrite(LedWiFI, LOW);
  //digitalWrite(LedVerde, LOW);
  //digitalWrite(LedAmarelo, LOW);

  //tempoatual = millisAtual;
  starTime = millisAtual;
  rfmilis = millisAtual;

  configRF();

  //pinMode(buttonState, OUTPUT);
  //digitalWrite(buttonState, LOW);
  pinMode(buttonState, 0x00);

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

  retornaNotificar;

  Serial.println("Notificar: " + String(notificar));

  TipoMemoria = "1"; //lerMemoria();

  Memoria();
  CarregaEntradas();

}

void loop(void)
{

  //Serial.println(chip3.read8());
  //Serial.println(String(Rtc.alarm_minute));
  nCiclos++;

  millisAtual = millis();

  if (chipAtivo == true)
  {

    if (chip3.read(LedWiFI) == 0x0)
    {
      int32_t rssi;

      //      chip3.write(LedAmarelo, LOW);
      if (WiFi.status() == WL_CONNECTED)
      {
        if (vConfigWIFI == "0")
        {
          lastWifiTime = millisAtual;
        }
        rssi = WiFi.RSSI();
        //Serial.println(String(rssi));

        if (rssi >= -65)
        {
          chip3.write(LedVerde, 0x0);
          chip3.write(LedAmarelo, 0x1);
        }
        else if (rssi < -65 && rssi >= -100)
        {
          chip3.write(LedVerde, 0x1);
          chip3.write(LedAmarelo, 0x0);
        }
        else
        {
          chip3.write(LedVerde, 0x1);
          chip3.write(LedAmarelo, 0x1);
        }
      }
    }

    // reconexao
    if ((vConfigWIFI == "0" && WiFi.status() != WL_CONNECTED) || (vConfigWIFI == "0" && tipoWifiAtual == 2))
    {
      if ((millisAtual - lastWifiTime) >= 300000)
      {
        conectar();
        lastWifiTime = millisAtual;
      }
    }

    // valida IP Reconexao
    if ((vConfigWIFI == "0" && tipoWifiAtual != 2 && WiFi.localIP() != local_IP))
    {
      conectar();
    }

    //Serial.println("Tempo: " + String(millisAtual - tempoatual));

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
      }
      else if (HorarioAtual.Second() % 5 != 0 && ConsultouCloud == true)
      {
        ConsultouCloud = false;
      }
    }

    if (HorarioAtual.Minute() != Minuto)
    {
      Serial.println("Ciclos por segundos: " + String(nCiclos / 60));
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
# 415 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino"
      nCiclos = 0;
      Minuto = HorarioAtual.Minute();

      char time[50];
      sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());
      Serial.println(time);

      agenda();
    }

    //  if (HorarioAtual.Second() != Segundo)
    if (Segundo == -1 || (millisAtual - Segundo) > 200)
    {
      Segundo = millisAtual;
      //Segundo = HorarioAtual.Second();
      chip3.write(LedGeral, !chip3.read(LedGeral));
    }

    int ValorbuttonState = digitalRead(buttonState);

    if (ValorbuttonState == 0x1)
    {
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
        Serial.println(""); // Blank line between entries
        //tone(Buzzer, 4000, 800);
        chip3.write(Buzzer, 0x1);
        delay(300);
        chip3.write(Buzzer, 0x0);
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
      if (millisAtual - rfmilis >= 10000 || millisAtual - rfmilis < 0)
      {
        rfmilis = millisAtual;
        for (int i = 0; i < 30; i++)
        {
          ultimoEstadoRF[i] = 0x0;
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
        Serial.print(mySwitch.getReceivedValue());
        Serial.print(" / ");
        Serial.print(mySwitch.getReceivedBitlength());
        tamanhoRF = mySwitch.getReceivedBitlength();
        Serial.print("bit ");
        Serial.print("Protocol: ");
        Serial.println(mySwitch.getReceivedProtocol());
        gProtocoloRF = mySwitch.getReceivedProtocol();
        trataRF();
      }
      mySwitch.resetAvailable();

      //delay(300);
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
# 550 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\firmware16.ino"
    //Logica para resete de entrada pulsada
    for (int iPorta = 0; iPorta <= 15; iPorta++)
    {
      if (g_pulsoHabilita[iPorta])
      {
        // proteção no caso de variavel estourar
        if (millisAtual < g_tempoInicioPulso[iPorta])
        {
          g_tempoInicioPulso[iPorta] = 0;
        }
        else if (millisAtual > g_tempoInicioPulso[iPorta] + 500)
        {
          g_pulsoHabilita[iPorta] = false;
          if (iPorta < 8)
          {
            chip1.write(iPorta, 0x1);
          }
          else
          {
            chip2.write(iPorta - 8, 0x1);
          }
        }
      }
    }
  }

}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\about.ino"
void about() {
    server.send(200, "text/html", "<html><body><h1>Desenvolvido por:</h1><p>Fernando Leal</p><p>Julio Valadares</p><p>Leonardo Aquino</p><p><a href=\"http://www.keepin.com.br\">www.keepin.com.br</a></p></body></html>");
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\agenda.ino"
// CONNECTIONS:
// DS3231 SDA --> SDA
// DS3231 SCL --> SCL
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND


void agenda ()
{

    Serial.println("Consultado agenda...");

    verificaAgenda();
}

void verificaArquivos() {

  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "w");

  if (!f) {
    SPIFFS.format();
    File f = SPIFFS.open("/agenda.txt", "w");
  }
  f.close();
  SPIFFS.end();
}

void verificaAgenda() {
  for (int i = 1; i <= 6; i++)
  {
    if (AgendaAlterada) {
      AgendaAlterada = false;
      //consultaAgenda(i);
      consultaAgenda2();
    }
    //String texto = consultaAgenda(i);
    String texto = Agendas[i-1];
    texto.trim();

    String Data = "";
    String HoraI = "";
    String HoraF = "";
    String Seg = "";
    String Ter = "";
    String Qua = "";
    String Qui = "";
    String Sex = "";
    String Sab = "";
    String Dom = "";
    String Tipo = "";
    String Porta = "";
    String Ativo = "";
    String ECena = "";
    String CenaLiga = "";
    String CenaDesliga = "";

    int i3 = 1;
    int qtdeTexto = texto.length();
    //separa dados nas variaveis
    for (int i2 = 0; i2 <= qtdeTexto-1; i2++)
    {
      if (texto[i2] != '|' && i3 <= 16 && texto != "")
      {
        if (i3 == 1)
        {
          Data += texto[i2];
        }

        if (i3 == 2)
        {
          HoraI += texto[i2];
        }

        if (i3 == 3)
        {
          HoraF += texto[i2];
        }

        if (i3 == 4)
        {
          Seg += texto[i2];
        }

        if (i3 == 5)
        {
          Ter += texto[i2];
        }

        if (i3 == 6)
        {
          Qua += texto[i2];
        }

        if (i3 == 7)
        {
          Qui += texto[i2];
        }

        if (i3 == 8)
        {
          Sex += texto[i2];
        }

        if (i3 == 9)
        {
          Sab += texto[i2];
        }

        if (i3 == 10)
        {
          Dom += texto[i2];
        }

        if (i3 == 11)
        {
          Tipo += texto[i2];
        }

        if (i3 == 12)
        {
          Porta += texto[i2];
        }

        if (i3 == 13)
        {
          Ativo += texto[i2];
        }

        if (i3 == 14)
        {
          ECena += texto[i2];
        }

        if (i3 == 15)
        {
          CenaLiga += texto[i2];
        }

        if (i3 == 16)
        {
          CenaDesliga += texto[i2];
        }
      }
      else
      {
        i3++;
      }

    } // fim separar dados

/*     Serial.println(texto);

    Serial.println("Agenda Ativa: " + Ativo);

    Serial.println("Agenda é Cena: " + ECena);

    Serial.println("Agenda Cena Liga: " + CenaLiga);

    Serial.println("Agenda Cena Desliga: " + CenaDesliga);

 */
# 159 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\agenda.ino"
    if (i == 3) {
      //Serial.println("Ativo: " + Ativo + " Tipo: " + Tipo);
    }

    // Verifica se a agenda está ativa
    if (Ativo == "1")
    {
      //pinMode(retornaPorta(Porta.toInt()), OUTPUT);               
      RtcDateTime now;
      now = carregaHora();
      // Se tipo é 1
      String HoraAgora = "";
      if (now.Hour() < 10)
        HoraAgora += "0" + String(now.Hour()) + ":";
      else
        HoraAgora += String(now.Hour()) + ":";

      if (now.Minute() < 10)
        HoraAgora += "0" + String(now.Minute());
      else
        HoraAgora += String(now.Minute());

      String DataAgora = "";
      if (now.Day() < 10) {
        DataAgora += "0" + String(now.Day()) + "/";
      }
      else
      {
        DataAgora += String(now.Day()) + "/";
      }
      if (now.Month() < 10) {
        DataAgora += "0" + String(now.Month()) + "/";
      }
      else
      {
        DataAgora += String(now.Month()) + "/";
      }
      DataAgora += String(now.Year());
      String DiaSemanaAgora = String(now.DayOfWeek());
      int EstadoPorta = 0x0;
      if (Porta != "") {
        EstadoPorta = LePorta(retornaPorta(Porta.toInt()));
      }

      String ValorAgendaDesligada = Data+ "|" + HoraI + "|" +HoraF+ "|" +Seg+ "|" +Ter+ "|" +Qua+ "|" +Qui+ "|" +Sex+ "|" +Sab+ "|" +Dom+ "|" +Tipo+ "|" + Porta + "|0|"+ ECena + "|" + CenaLiga + "|" + CenaDesliga + "|" ;
      //Serial.println(HoraAgora);
      if (Tipo == "1") // apenas uma vez
      {
        //Serial.println("Agenda tipo 1 encontrada");
        if ((HoraAgora == HoraI) && (ECena == "1"))
        {
          triggerCena(CenaLiga);
        } else {
          if ((HoraAgora == HoraI) && (EstadoPorta == 0x0))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x1, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1")) {
          if (CenaDesliga != ""){
            triggerCena(CenaDesliga);
          }
          Agendas[i-1] = ValorAgendaDesligada;
          Serial.println(Agendas[i-1]);
          gravaragenda2();

        } else {
          if ((HoraAgora == HoraF) && (EstadoPorta == 0x1))
          {
            // deslita e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x0, "", 0);

            Agendas[i-1] = ValorAgendaDesligada;
            Serial.println(Agendas[i-1]);
            gravaragenda2();
          }
        }
      }
      if (Tipo == "2") // todos os dias
      {
        //Serial.println("Agenda tipo 2 encontrada");
        if ((HoraAgora == HoraI) && (ECena == "1")) {
          triggerCena(CenaLiga);
        } else {
          if ((HoraAgora == HoraI) && (EstadoPorta == 0x0))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x1, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1")) {
          if (CenaDesliga != ""){
            triggerCena(CenaDesliga);
          }
        } else {
          if ((HoraAgora == HoraF) && (EstadoPorta == 0x1))
          {
            // desgita
            //digitalWrite(retornaPorta(Porta.toInt()), 0);         
            LigaDesliga(retornaPorta(Porta.toInt()), 0x0, "", 0);
          }
        }
      }
      if (Tipo == "3") // data específica
      {
        //Serial.println("Agenda tipo 3 encontrada");
        //Serial.println("Data Agora: " + DataAgora);
        //Serial.println("Data: " + Data);
        if ((HoraAgora == HoraI) && (ECena == "1") && (DataAgora == Data)) {
          triggerCena(CenaLiga);
        } else {

          if ((HoraAgora == HoraI) && (EstadoPorta == 0x0) && (DataAgora == Data))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x1, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1") && (DataAgora == Data)) {
          if (CenaDesliga != ""){
            triggerCena(CenaDesliga);
          }
          Agendas[i-1] = ValorAgendaDesligada;
          gravaragenda2();
        } else {
          if ((HoraAgora == HoraF) && (EstadoPorta == 0x1) && (DataAgora == Data))
          {
            // desliga e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x0, "", 0);

            Agendas[i-1] = ValorAgendaDesligada;
            gravaragenda2();
          }
        }
      }

      if (Tipo == "4") // dia da semana
      {
        boolean EDiaSemana = 0;

        if ((Seg == "1") && (DiaSemanaAgora == "1"))
        {
          EDiaSemana = true;
        }
        else
        if ((Ter == "1") && (DiaSemanaAgora == "2"))
        {
          EDiaSemana = true;
        }
        else
        if ((Qua == "1") && (DiaSemanaAgora == "3"))
        {
          EDiaSemana = true;
        }
        else
        if ((Qui == "1") && (DiaSemanaAgora == "4"))
        {
          EDiaSemana = true;
        }
        else
        if ((Sex == "1") && (DiaSemanaAgora == "5"))
        {
          EDiaSemana = true;
        }
        else
        if ((Sab == "1") && (DiaSemanaAgora == "6"))
        {
          EDiaSemana = true;
        }
        else
        if ((Dom == "1") && (DiaSemanaAgora == "0"))
        {
          EDiaSemana = true;
        }
        else
        {
          EDiaSemana = false;
        }

        //Serial.println("Agenda tipo 4 encontrada");
        //Serial.println("Valor EdiaSemana = " + String(EDiaSemana));                                                                                                                                                  
        //Serial.println("Valor DiaSemanaAgora = " + String(DiaSemanaAgora));                                                                                                                                                  
        //Serial.println("Porta: " + Porta);
        //domingo
        //if (EDiaSemana == 1)
          //Serial.println("Vai rodar");

        if ((HoraAgora == HoraI) && (ECena == "1") && (EDiaSemana == 1) ) {
          triggerCena(CenaLiga);
        } else {
          if ((HoraAgora == HoraI) && (EstadoPorta == 0x0) && (EDiaSemana == 1) )
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x1, "", 0);
          }
        }
        if ((HoraAgora == HoraF) && (ECena == "1") && (EDiaSemana == 1) ) {
          if (CenaDesliga != ""){
            triggerCena(CenaDesliga);
          }
        } else {
          if ((HoraAgora == HoraF) && (EstadoPorta == 0x1) && (EDiaSemana == 1) )
          {
            // desliga e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), 0x0, "", 0);
          }
        }
      }


    }
  }
}

void atualizahora() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  int vHora = server.arg("h").toInt();
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    RtcDateTime HoraNova = vHora;

    Rtc.year = HoraNova.Year();
    Rtc.month =HoraNova.Month();
    Rtc.day = HoraNova.Day();
    Rtc.hour = HoraNova.Hour();
    Rtc.minute = HoraNova.Minute();
    Rtc.second = HoraNova.Second();
    Rtc.set_time();


    //Serial.println("Hora atualizada com sucesso!");
    printDateTime(HoraNova);
    server.send(200, "text/html", "ok");
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

String consultaAgenda(int id) {
/*

  SPIFFS.begin();

  File f = SPIFFS.open("/ag"+String(id)+".txt", "r");

  String texto = f.readStringUntil('*');

  f.close();

  SPIFFS.end();  



  //Serial.println("consultando agenda " + String(id));

  //Serial.println(texto);



  return texto;

*/
# 425 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\agenda.ino"
  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "r");
  String texto = f.readStringUntil('*');
  f.close();
  SPIFFS.end();

  //Serial.println("consultando agenda " + String(id));
  //Serial.println(texto);

  return texto;

}

void consultaAgenda2() {
  /*

  SPIFFS.begin();

  for (int i = 1; i <= 6; i++) {

    Agendas[i-1] = "";   

    File f = SPIFFS.open("/ag"+String(i)+".txt", "r");

    Agendas[i-1] = f.readStringUntil('*');

    f.close();



  //Serial.println("consultando agenda " + String(id));

  //Serial.println(texto);

  }

  SPIFFS.end();  

  */
# 452 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\agenda.ino"
  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "r");
  String texto = f.readStringUntil('*');
  texto += '*';
  f.close();
  SPIFFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoAgenda = "";

  if (texto.length() > 13)
  {
    Agendas[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoAgenda += texto[posicao];
      }
      else
      if (texto[posicao] == '|' && contador < 16)
      {
        contador ++;
        textoAgenda += texto[posicao];
      }

      if (contador == 16)
      {
        Agendas[i] = textoAgenda;
        textoAgenda = "";
        contador = 0;
        i++;
      }
      posicao ++;
    }
  }
}


void conagenda() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //int idAgenda = server.arg("ag").toInt();  
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    //Serial.println("consultando agenda");
    String texto = consultaAgenda(0);
    server.send(200, "text/html", texto);
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void gravaragenda() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  AgendaAlterada = true;
  //String idAgenda = server.arg("ag");
  String Valor = server.arg("v");
  String Senha = server.arg("k");

  if (Senha == "kdi9e") {
    SPIFFS.begin();
    File f = SPIFFS.open("/agenda.txt", "w");

    if (!f) {
      SPIFFS .format();
      File f = SPIFFS.open("/agenda.txt", "w");
    }

    f.println(Valor);
    f.close();
    SPIFFS.end();
    //Serial.println("valor salvo na ag"+idAgenda+".txt");
    //Serial.println(Valor);
  }
  server.send(200, "text/html", "ok");
}

void gravaragenda2()
{
  String texto = "";

  texto += Agendas[0] + Agendas[1] + Agendas[2] + Agendas[3] + Agendas[4] + Agendas[5] + '*';
  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "w");

  if (!f) {
    SPIFFS .format();
    File f = SPIFFS.open("/agenda.txt", "w");
  }

  f.println(texto);
  f.close();
  SPIFFS.end();

  //Serial.println("valor salvo agenda");
  //Serial.println(texto);
}



void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring,
            (sizeof(datestring) / sizeof(datestring[0])),
            (__extension__({static const char __c[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "agenda.ino" "." "572" "." "101" "\", \"aSM\", @progbits, 1 #"))) = ("%02u/%02u/%04u %02u:%02u:%02u"); &__c[0];})),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}


String RetornaData(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring,
            (sizeof(datestring) / sizeof(datestring[0])),
            (__extension__({static const char __c[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "agenda.ino" "." "589" "." "102" "\", \"aSM\", @progbits, 1 #"))) = ("%02u/%02u/%04u %02u:%02u:%02u"); &__c[0];})),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    return datestring;
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\api.ino"
void api() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

    String vPassApi = server.arg("pw");
    vPassApi.toLowerCase();

    if (AlowApi == true && vPassApi == ApiPass)
    {
        String action = server.arg("a");
        String apiPort = server.arg("p");
        String apiSource = server.arg("s");
        Serial.println(apiSource);
        // consulta
        if (action == "q")
        {
            //todas portas
            if (apiPort == "a")
            {
                String sDados1 = "";
                String sDados2 = "";
                if (apiSource == "o") // saidas
                {
                    sDados1 = String(chip1.read8(), 2);
                    sDados2 = String(chip2.read8(), 2);
                }
                else if (apiSource == "i") // entradas
                {
                    sDados1 = String(sensor1.read8(), 2);
                    sDados2 = String(sensor2.read8(), 2);
                }
                else
                {
                    sDados1 = "00000000";
                    sDados2 = "00000000";
                }

                Serial.println("tamanho");
                Serial.println(sDados1.length());
                while (sDados1.length() < 8)
                {
                    sDados1 = '0' + sDados1;
                }

                while (sDados2.length() < 8)
                {
                    sDados2 = '0' + sDados2;
                }



                for (int i = 0; i < 8; i++) {
                    if (sDados1[i] == '0') {
                        sDados1[i] = '1';
                    }
                    else {
                        sDados1[i] = '0';
                    }
                }

                for (int i = 0; i < 8; i++) {
                    if (sDados2[i] == '0') {
                        sDados2[i] = '1';
                    }
                    else {
                        sDados2[i] = '0';
                    }
                }



                server.send(200, "text/html", sDados2+sDados1);
            }
            else
            {
                if (apiSource == "o")
                {
                    if (LePorta(apiPort.toInt()-1) == 0x1)
                    {
                        server.send(200, "text/html", "1");
                    } else
                    {
                        server.send(200, "text/html", "0");
                    }
                }
                else
                {
                    if (LeSensor(apiPort.toInt()-1) == 0x1)
                    {
                        server.send(200, "text/html", "1");
                    } else
                    {
                        server.send(200, "text/html", "0");
                    }
                }
            }
        }
        else if (action == "a") // ação
        {
            String valueApi = server.arg("v");
            String typeApi = server.arg("t");
            if (apiPort == "a") // todos
            {
                if (apiSource == "o") // saidas
                {
                    if (valueApi == "0")
                    {
                        if (typeApi == "n")
                        {
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;
                            Rtc.set_chip1();
                            Rtc.set_chip2();
                            chip1.write8(255);
                            chip2.write8(255);
                            server.send(200, "text/html", "1");
                        }
                        else if (typeApi == "p")
                        {
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;
                            Rtc.set_chip1();
                            Rtc.set_chip2();
                            chip1.write8(255);
                            chip2.write8(255);
                            server.send(200, "text/html", "1");
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");
                        }
                    }
                    else
                    if (valueApi == "1")
                    {
                        if (typeApi == "n")
                        {
                            Rtc.chip1 = 0;
                            Rtc.chip2 = 0;
                            Rtc.set_chip1();
                            Rtc.set_chip2();
                            chip1.write8(0);
                            chip2.write8(0);
                            server.send(200, "text/html", "1");
                        }
                        else if (typeApi == "p")
                        {
                            Rtc.chip1 = 0;
                            Rtc.chip2 = 0;
                            chip1.write8(0);
                            chip2.write8(0);
                            delay(300);
                            chip1.write8(255);
                            chip2.write8(255);
                            Rtc.chip1 = 255;
                            Rtc.chip2 = 255;
                            Rtc.set_chip1();
                            Rtc.set_chip2();
                            server.send(200, "text/html", "1");
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");
                        }
                    }
                    else
                    {
                        server.send(200, "text/html", "-1");
                    }
                }
                else
                {
                    server.send(200, "text/html", "-1");
                }
            }
            else // porta
            {
                if (apiSource == "o") // saidas
                {
                    if (typeApi == "n") //normal
                    {
                        if (valueApi == "0") // desliga
                        {
                            LigaDesliga(apiPort.toInt()-1, 0x0, "", 0);
                            server.send(200, "text/html", "1");
                        }
                        else if (valueApi == "1") // Liga
                        {
                            LigaDesliga(apiPort.toInt()-1, 0x1, "", 0);
                            server.send(200, "text/html", "1");
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");
                        }
                    }
                    else if (typeApi == "p") // pulsado
                    {
                        if (valueApi == "0") // desliga
                        {
                            LigaDesliga(apiPort.toInt()-1, 0x0, "", 1);
                            server.send(200, "text/html", "1");
                        }
                        else if (valueApi == "1") // Liga
                        {
                            LigaDesliga(apiPort.toInt()-1, 0x1, "", 1);
                            server.send(200, "text/html", "1");
                        }
                        else
                        {
                            server.send(200, "text/html", "-1");
                        }
                    }
                    else
                    {
                        server.send(200, "text/html", "-1");
                    }
                }
                else
                {
                    server.send(200, "text/html", "-1");
                }

            }
        }
        else
        // Infravermelho
        if (action == "i")
        {
            Serial.println("api infravermelho");
            String vModel1 = server.arg("m1");
            String vModel2 = server.arg("m2");
            String vModel3 = server.arg("m3");
            String vModel4 = server.arg("m4");
            String Comando1 = server.arg("c1");
            String Comando2 = server.arg("c2");
            String Comando3 = server.arg("c3");
            String Comando4 = server.arg("c4");
            String Comando12 = server.arg("c12");
            String Comando22 = server.arg("c22");
            String Comando32 = server.arg("c32");
            String Comando42 = server.arg("c42");
            String qtde1 = server.arg("qt1");
            String qtde2 = server.arg("qt2");
            String qtde3 = server.arg("qt3");
            String qtde4 = server.arg("qt4");
            String vp1 = server.arg("p1");
            String vp2 = server.arg("p2");
            String vp3 = server.arg("p3");
            String vp4 = server.arg("p4");

            if (vModel1 != "")
            {
                sendirAPI(qtde1.toInt(), vModel1.toInt(), Comando1, Comando12, vp1.toInt());
                Serial.println("1");
            }

            if (vModel2 != "")
            {
                delay(300);
                sendirAPI(qtde2.toInt(), vModel2.toInt(), Comando2, Comando22, vp2.toInt());
                Serial.println("2");
            }

            if (vModel3 != "")
            {
                delay(300);
                sendirAPI(qtde3.toInt(), vModel3.toInt(), Comando3, Comando32, vp3.toInt());
                Serial.println("3");
            }

            if (vModel4 != "")
            {
                delay(300);
                sendirAPI(qtde4.toInt(), vModel4.toInt(), Comando4, Comando42, vp4.toInt());
            }
            server.send(200, "text/html", "1");
       }
        else
        // Cenas
        if (action == "c")
        {
            Serial.println("api cenas");
            String valueApi = server.arg("v");
            triggerCena(valueApi);
            server.send(200, "text/html", "1");
       }
       else if (action == "l") // linha de ação 
       {
            for (int i = 1; i <= apiPort.length(); i++ )
            {
                int posicaoPorta = i-1;
                if (posicaoPorta <= 15) {
                    if (apiPort[posicaoPorta] == '0') {
                        // desliga
                        LigaDesliga(posicaoPorta, 0x0, "", 0);
                    } else if (apiPort[posicaoPorta] == '1') {
                        // liga
                        LigaDesliga(posicaoPorta, 0x1, "", 0);
                    }

               }
           }

            server.send(200, "text/html", "1");
       }
    }
    else
    {
        server.send(200, "text/html", "-1");
    }
}

void apiativo() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

    if (AlowApi == true)
    {
        server.send(200, "text/html", "1");
    }
    else
    {
        server.send(200, "text/html", "0");
    }
}

void apiconfig() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

    String vSenha = server.arg("s");
    String vApi = server.arg("v");

    if (vSenha == Senha)
    {
        SPIFFS.begin();

        File f = SPIFFS.open("/alowapi.txt", "w");
        f.println(vApi+"|");
        f.close();

        SPIFFS.end();

        if (vApi == "1") {
            AlowApi = true;
        }
        else if (vApi == "0") {
            AlowApi = false;
        }

        server.send(200, "text/html", "ok");
    }
    else
    {
        server.send(200, "text/html", "-1");
    }
}

void alterasenhapi() {
//  const char* www_username = www_username2.c_str();
//  const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String req = server.arg("v");
  String CurrentPass = server.arg("a");

  if (vSenha == Senha)
  {
    if (req == "")
    {
      req = "12345678"; // se não houver registro, vai para o padrão
    }

    MD5Builder md5;
    md5.begin();
    md5.add(req);
    md5.calculate();
    req = md5.toString();



    md5.begin();
    md5.add(CurrentPass);
    md5.calculate();
    CurrentPass = md5.toString();

    if (ApiPass == CurrentPass) {

      SPIFFS.begin();
      File f = SPIFFS.open("/apipass.txt", "w");

      f.println(req+"|");
      f.close();

      SPIFFS.end();
      ApiPass = req;
      server.send(200, "text/html", "ok");
    }
    else {
      server.send(200, "text/html", "-1");
    }
  }
  else {
    server.send(200, "text/html", "-1");
  }

}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\auxfunction.ino"
void log(String msg){
    Serial.println(msg);
}

void ConfigurarWebServer(void){
    server.on("/", configuracao);
    server.on("/grava", grava);
    server.on("/ler", ler);
    server.on("/config", configuracao);
    server.on("/gravarwifi", gravawifi);
    server.on("/gravasenhawifi", gravasenhawifi);
    server.on("/gravasenhahttp", gravasenhahttp);
    server.on("/reset", wifireset);
    server.on("/reiniciar", reiniciar);
    server.on("/valida", valida);
    server.on("/controle", controle);
    server.on("/situacao", situacao);
    server.on("/chipid", retornachip);
    server.on("/chamaddns", chamaddns);
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
    server.on("/getir", getIR);
    server.on("/sendir", sendir);
    server.on("/habir", habir);
    //RF
    server.on("/habrf", habRF);
    server.on("/getrf", getRF);
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
    server.on("/dirarquivos", dirarquivos);
    server.on("/downloadfile", File_Download);
    server.on("/uploadfile", File_Upload);
    server.on(
        "/fupload", HTTP_POST, []() { server.send(200); }, handleFileUpload);
    server.on("/deletefile", File_Delete);
    //server.on("/cloud", cloud);
    //  server.on("/sendcloud", sendCloud);

    server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();

    Serial.println("HTTP server started");
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\cenas.ino"
void gravacena()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    String ia = server.arg("ia");
    String vTipo = server.arg("t");
    String Valor = server.arg("v");
    String IdCena = server.arg("id");
    Serial.println(Valor);
    SPIFFS.begin();

    if (ia == "1") // inicio de arquivo
    {
      File rFile = SPIFFS.open("/ce_" + IdCena + ".cfg", "w");
      rFile.println(Valor);
      rFile.close();
    }
    else
    {
      File rFile = SPIFFS.open("/ce_" + IdCena + ".cfg", "a");
      //String ValorAnterior = rFile.readString();
      //rFile.close();

      //rFile = SPIFFS.open("/ce_"+IdCena+".cfg","w");
      //rFile.print(ValorAnterior);
      rFile.println(Valor);
      rFile.close();
    }

    SPIFFS.end();
    //lerArquivo(IdCena);
    server.send(200, "text/html", "ok");
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void lerArquivo(String id)
{
  SPIFFS.begin();

  File rFile = SPIFFS.open("/ce_" + id + ".cfg", "r");
  String linhas;
  //linhas = rFile.readString();

  //Serial.println("linhas das cenas");
  int qtde = 0;
  while (rFile.available())
  {
    String linhas = rFile.readStringUntil('\n');
    Serial.print(linhas);
    qtde++;
  }

  rFile.close();
  SPIFFS.end();
  //Serial.print("qtde de linhas: ");
  Serial.println(qtde);
}

void triggerCena(String arq)
{
  cenaExecucao = true;
  cenaPAtual = 0;
  cenaPTotal = 0;
  ArqCena = arq;
}

void checkCena()
{
  if (cenaExecucao == true)
  {
    String Comando;
    static File rFile;
    SPIFFS.begin();
    if (cenaPAtual == 0) // abre spiff e mantem aberto
    {
      rFile = SPIFFS.open("/ce_" + ArqCena + ".cfg", "r");

      while (rFile.available())
      {

        String linhas = rFile.readStringUntil('\n');
        if (cenaPTotal == 0)
        {
          Comando = linhas;
        }
        cenaPTotal++;
      }
      Serial.println("Total cena: " + String(cenaPTotal));
      cenaPAtual++;
      Serial.println("Cena atual: " + String(cenaPAtual));

      rFile.close();
      SPIFFS.end();
    }
    if (cenaPAtual >= 1)
    {
      //Serial.print("processando cena: ");
      //Serial.println(cenaPAtual);

      if (cenaPAtual == 1)
      {
        //Serial.print(Comando);
        //Serial.println("primeiro");
        //Serial.println("Cena atual: " + String(cenaPAtual));
        Serial.println("Executar cena: " + Comando);
        executaCena(Comando);
        //Serial.println("Exceutou cena primeiro");
            }
      else
      {
        Serial.println("Cena atual: " + String(cenaPAtual));
        //rFile.seek(0, SeekSet);

        SPIFFS.begin();
        rFile = SPIFFS.open("/ce_" + ArqCena + ".cfg", "r");
        int conCena = 1;

        while (rFile.available())
        {
          //Serial.println("\nInicio While");
          //Serial.println("conCena while: " + String(conCena));
          //Serial.println("Cena atual while: " + String(cenaPAtual));

          String linhas = rFile.readStringUntil('\n');

          //Serial.println("Codigo lido arquivo: " + linhas + "Posição no arquivo: " + rFile.position());

          if (conCena == cenaPAtual)
          {

            Comando = linhas;
            //Serial.println("Executar cena while: " + Comando);
            //Serial.println("Cena atual antes execução: " + String(cenaPAtual));
            Serial.println("Executar cena: " + Comando);
            executaCena(Comando);
            //Serial.println("Cena atual apos execução" + String(cenaPAtual));
            rFile.close();
            SPIFFS.end();
            //conCena = 0;
          }
          conCena++;

          //Serial.println("ConCena fim while: " + String(conCena));
        }
        //Serial.println("Sai do while");
      }
    }

    if (cenaPAtual > cenaPTotal)
    {
      rFile.close();
      SPIFFS.end();
      Serial.println("\nfim da cena\n");
      cenaExecucao = false;
      cenaPAtual = 0;
      cenaPTotal = 0;
    }
  }
}

void executaCena(String comandoCena)
{
  //Serial.print(comandoCena);
  String cmdChipID;
  String cmdIP;
  String cmdTipo;
  String cmdPorta;
  String cmdAcao;
  String cmdAcao2;
  String cmdQtde;
  String cmdValorMin;
  String cmdModelo;
  int i;
  int posicaoi = 1;
  int posicaof = 4;

  for (i = 0; i < comandoCena.length(); i++)
  {
    //Serial.println("Posicao " + String(i));
    if (comandoCena[i] != '|' && posicaoi <= posicaof)
    {
      if (posicaoi == 1) // chip id
      {
        cmdChipID += comandoCena[i];
      }
      if (posicaoi == 2) // IP
      {
        cmdIP += comandoCena[i];
      }
      if (posicaoi == 3) // Tipo do comando (1 - Rele; 2 - IR; 3 - Intervalo; 4 - Dimmer)
      {
        cmdTipo += comandoCena[i];
      }
      if (posicaoi == 4)
      {
        if (cmdTipo == "1")
        {
          posicaof = 5;
          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          posicaof = 8;
          cmdQtde += comandoCena[i];
        }
        else if (cmdTipo == "3")
        {
          posicaof = 4;
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          posicaof = 6;
          cmdPorta += comandoCena[i];
        }
      }
      else if (posicaoi == 5)
      {
        if (cmdTipo == "1")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdValorMin += comandoCena[i];
        }
      }
      else if (posicaoi == 6)
      {
        if (cmdTipo == "2")
        {
          cmdAcao2 += comandoCena[i];
          //          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdAcao += comandoCena[i];
        }
      }
      else if (posicaoi == 7)
      {
        if (cmdTipo == "2")
        {
          cmdPorta += comandoCena[i];
          //cmdModelo += comandoCena[i];
        }
      }
      else if (posicaoi == 8)
      {
        if (cmdTipo == "2")
        {
          cmdModelo += comandoCena[i];
        }
      }
    }
    else // encontrou o caracter '|'
    {
      posicaoi++;
    }
  }

  IPAddress Destino;
  Destino.fromString(cmdIP);
  String Texto;
  // comandos
  //Serial.println("Tipo " + cmdTipo);
  if (cmdTipo == "1") // Rele
  {
    if (Destino == IpDispositivo)
    {
      if (cmdAcao == "0") // desliga
      {
        LigaDesliga(cmdPorta.toInt() - 1, 0x0, "", 0);
      }
      else if (cmdAcao == "1") // liga
      {
        LigaDesliga(cmdPorta.toInt() - 1, 0x1, "", 0);
      }
      else if (cmdAcao == "2") // pulso
      {
        LigaDesliga(cmdPorta.toInt() - 1, 0x0, "", 1);
      }
      cenaPAtual++;
    }
    else //upd
    {
      char replyPacekt[255] = "";
      if (cmdAcao == "0") // desliga
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "1") // liga
      {
        Texto = cmdIP + "|" + cmdPorta + "|true|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "2") // pulso
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|1|";
      }
      Serial.println(Texto);
      Texto.toCharArray(replyPacekt, 255);
      Serial.println("enviado comando UDP");
      Udp.beginPacket(Destino, localUdpPort);
      Udp.write(replyPacekt);
      Udp.endPacket();
      delay(200);
      cenaPAtual++;
    }
  }
  else if (cmdTipo == "2") // IR
  {
    if (Destino == IpDispositivo)
    {
      if (lastCnTime == 0 || (millisAtual - lastCnTime >= 300))
      {
        //Serial.println("CenaAntes " + String(cenaPAtual));

        sendIRCMD(cmdAcao, cmdAcao2, cmdQtde.toInt(), cmdPorta.toInt(), cmdModelo.toInt(), cmdQtde.toInt());
        lastCnTime = millisAtual;
        //lastCnTime = millis();
        //Serial.println("terminou IR");
        // Serial.println("Cena apos ir " + String(cenaPAtual));
        cenaPAtual++;
        // Serial.println("Cena apos ir + 1 " + String(cenaPAtual));
        //delay(300);
      }
      else if (millisAtual - lastCnTime < 0)
      {
        lastCnTime = millisAtual;
        //lastCnTime = millis();
      }
    }
    else //upd - não implentado UPD para IR
    {
    }
  }
  else if (cmdTipo == "3") // timer
  {
    //Serial.println("Entrou no timer " + String(lastCnTime));
    if (lastCnTime <= 0)
    {
      lastCnTime = millisAtual;
    }

    if (lastCnTime == 0 || (millisAtual - lastCnTime >= cmdAcao.toInt()))
    {
      cenaPAtual++;
      lastCnTime = -1;
    }
    else if (millisAtual - lastCnTime < 0)
    {
      lastCnTime = millisAtual;
    }
  }
  else if (cmdTipo == "4") // Dimmer
  {
    char replyPacekt[255] = "";
    String Texto = cmdIP + "|" + cmdPorta + "|" + cmdAcao + "|" + cmdChipID + "|X|";

    Texto += cmdValorMin + "|n|";
    Texto.toCharArray(replyPacekt, 255);

    Serial.println("enviado comando UDP");
    Udp.beginPacket(Destino, localUdpPort);
    Udp.write(replyPacekt);
    Udp.endPacket();

    cenaPAtual++;
  }
  else
  {
    cenaPAtual++;
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\cloud.ino"
void cloud()
{
  HTTPClient http;
  http.begin("http://cloud.keepin.com.br/api/control/3");
  int httpCode = http.GET();
  String payload;
  if (httpCode > 0)
  {
    payload = http.getString();
    Serial.println(payload);
  }

  http.end();

  //    StaticJsonBuffer<200> jsonBuffer;

  DynamicJsonBuffer jsonBuffer(payload.length());
  JsonObject &root = jsonBuffer.parseObject(payload);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }
  else
  {

    const String Descricao = root["descricao"];
    const bool ED1 = root["ed1"];
    const bool ED3 = root["ed3"];

    Serial.println(Descricao);
    Serial.println(ED1);
    Serial.println(ED3);
  }

  server.send(200, "text/html", "ok");
}

void sendCloud()
{
  String sSensor1 = String(sensor1.read8(), 2);
  String sSensor2 = String(sensor2.read8(), 2);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  String sChip1 = String(chip1.read8(), 2);
  String sChip2 = String(chip2.read8(), 2);

  while (sChip1.length() < 8)
  {
    sChip1 = '0' + sChip1;
  }

  while (sChip2.length() < 8)
  {
    sChip2 = '0' + sChip2;
  }

  int32_t rssi;
  rssi = WiFi.RSSI();

  String dataPost = "";
  dataPost = "{";
  dataPost = dataPost + "\"idchip\": \"" + vchipId + "\",";
  dataPost = dataPost + "\"SD1\": \"" + retornaValorCloud(sChip1[7]) + "\",";
  dataPost = dataPost + "\"SD2\": \"" + retornaValorCloud(sChip1[6]) + "\",";
  dataPost = dataPost + "\"SD3\": \"" + retornaValorCloud(sChip1[5]) + "\",";
  dataPost = dataPost + "\"SD4\": \"" + retornaValorCloud(sChip1[4]) + "\",";
  dataPost = dataPost + "\"SD5\": \"" + retornaValorCloud(sChip1[3]) + "\",";
  dataPost = dataPost + "\"SD6\": \"" + retornaValorCloud(sChip1[2]) + "\",";
  dataPost = dataPost + "\"SD7\": \"" + retornaValorCloud(sChip1[1]) + "\",";
  dataPost = dataPost + "\"SD8\": \"" + retornaValorCloud(sChip1[0]) + "\",";
  dataPost = dataPost + "\"SD9\": \"" + retornaValorCloud(sChip2[7]) + "\",";
  dataPost = dataPost + "\"SD10\": \"" + retornaValorCloud(sChip2[6]) + "\",";
  dataPost = dataPost + "\"SD11\": \"" + retornaValorCloud(sChip2[5]) + "\",";
  dataPost = dataPost + "\"SD12\": \"" + retornaValorCloud(sChip2[4]) + "\",";
  dataPost = dataPost + "\"SD13\": \"" + retornaValorCloud(sChip2[3]) + "\",";
  dataPost = dataPost + "\"SD14\": \"" + retornaValorCloud(sChip2[2]) + "\",";
  dataPost = dataPost + "\"SD15\": \"" + retornaValorCloud(sChip2[1]) + "\",";
  dataPost = dataPost + "\"SD16\": \"" + retornaValorCloud(sChip2[0]) + "\",";
  dataPost = dataPost + "\"ED1\": \"" + retornaValorCloud(sSensor1[7]) + "\",";
  dataPost = dataPost + "\"ED2\": \"" + retornaValorCloud(sSensor1[6]) + "\",";
  dataPost = dataPost + "\"ED3\": \"" + retornaValorCloud(sSensor1[5]) + "\",";
  dataPost = dataPost + "\"ED4\": \"" + retornaValorCloud(sSensor1[4]) + "\",";
  dataPost = dataPost + "\"ED5\": \"" + retornaValorCloud(sSensor1[3]) + "\",";
  dataPost = dataPost + "\"ED6\": \"" + retornaValorCloud(sSensor1[2]) + "\",";
  dataPost = dataPost + "\"ED7\": \"" + retornaValorCloud(sSensor1[1]) + "\",";
  dataPost = dataPost + "\"ED8\": \"" + retornaValorCloud(sSensor1[0]) + "\",";
  dataPost = dataPost + "\"ED9\": \"" + retornaValorCloud(sSensor2[7]) + "\",";
  dataPost = dataPost + "\"ED10\": \"" + retornaValorCloud(sSensor2[6]) + "\",";
  dataPost = dataPost + "\"ED11\": \"" + retornaValorCloud(sSensor2[5]) + "\",";
  dataPost = dataPost + "\"ED12\": \"" + retornaValorCloud(sSensor2[4]) + "\",";
  dataPost = dataPost + "\"ED13\": \"" + retornaValorCloud(sSensor2[3]) + "\",";
  dataPost = dataPost + "\"ED14\": \"" + retornaValorCloud(sSensor2[2]) + "\",";
  dataPost = dataPost + "\"ED15\": \"" + retornaValorCloud(sSensor2[1]) + "\",";
  dataPost = dataPost + "\"ED16\": \"" + retornaValorCloud(sSensor2[0]) + "\",";
  dataPost = dataPost + "\"ip\": \"" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "\",";
  dataPost = dataPost + "\"notificacao\": \"" + String(notificar) + "\",";
  dataPost = dataPost + "\"sinal\": \"" + String(rssi) + "\"";
  dataPost = dataPost + " }";

  WiFiClient cliente;
  HTTPClient http;
  String payload;
  http.setTimeout(600);
  http.setReuse(true);
  //http.useHTTP10(false);
  //  http.begin(cliente, "192.168.1.147", 443, "/api/keepin", true);
  http.begin(cliente, "http://cloud.keepin.com.br/api/keepin");
  //  http.begin(cliente, "http://192.168.1.147/api/keepin");
  http.addHeader("Content-Type", "application/json");
  http.setUserAgent("KEEPIN/" + String("2,36") + " Automacao");
  //http.setUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0");
  int httpCode = http.POST(dataPost);
  //http.writeToStream(&Serial);
  payload = http.getString();
  http.end();
  //Serial.println(payload);
  Serial.println("Cloud code: " + String(httpCode));
  if (httpCode == 200 && payload != "[]")
  {
    //Serial.println("Payload: " + payload);
    DynamicJsonBuffer jsonBuffer(payload.length());
    JsonArray &array1 = jsonBuffer.parseArray(payload);
    //    JsonObject& root = jsonBuffer.parseObject(payload);

    if (!array1.success())
    {
      Serial.println("antes");
      Serial.println("parseObject() failed");
    }
    else
    {
      //Serial.println("qtde registros");
      //Serial.println(array1.size());

      for (int indice = 0; indice < array1.size(); indice++)
      {
        //JsonObject& root = jsonBuffer.parseObject(array1[indice]);

        //if(!root.success()) {
        //Serial.println("parseObject() failed");
        //}
        //{"tipo":"1","acao":"1","modelo":null,"qtdeBit":null,"porta":3},
        const String tipoJson = array1[indice]["tipo"];
        const String acaoJson = array1[indice]["acao"];
        const String qtdeJson = array1[indice]["qtdeBit"];
        const String modeloJson = array1[indice]["modelo"];
        const String portaJson = array1[indice]["porta"];
        //Serial.println("porta: " + portaJson);

        if (tipoJson == "1") // saida
        {
          //Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            if (acaoJson == "1")
            {
              LigaDesliga(porta, 0x1, "", 0);
            }
            else if (acaoJson == "0")
            {
              LigaDesliga(porta, 0x0, "", 0);
            }
          }
        }
        else if (tipoJson == "2") // saida pulsada
        {
          //Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            LigaDesliga(porta, 0x1, "", 1);
          }
        }
        else if (tipoJson == "3") // IR
        {
          //Serial.println("ligar lampada");
          int porta = portaJson.toInt();
          porta = retornaPorta(porta);
          if (porta >= 0)
          {
            cenaPAtual++;
            sendIRCMD(acaoJson, "", qtdeJson.toInt(), portaJson.toInt(), modeloJson.toInt(), qtdeJson.toInt());
            lastCnTime = millisAtual;
            delay(300);
          }
        }
        else if (tipoJson == "4") // RF
        {
          unsigned long Valor = strtoul(acaoJson.c_str(), __null, 10);

          sSendRF.send(Valor, 32);
        }
        else if (tipoJson == "5")
        {
          triggerCena(acaoJson);
        }
        else if (tipoJson == "6") // notificacao
        {
          SPIFFS.begin();
          File f = SPIFFS.open("/notific.txt", "w");

          f.println(acaoJson);

          f.close();
          SPIFFS.end();

          if (acaoJson == "true")
          {
            notificar = true;
          }
          else
          {
            notificar = false;
          }
        }
      }
    }

    if (payload != "[]")
    {
      http.setTimeout(600);
      http.setReuse(true);
      http.begin(cliente, "http://cloud.keepin.com.br/api/keepinactions/delete");
      http.addHeader("Content-Type", "application/json");
      http.setUserAgent("KEEPIN/" + String("2,36") + " Automacao");
      //http.setUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0");
      httpCode = http.POST(payload);
      if (httpCode == 200)
      {
        payload = http.getString();
        //Serial.println(payload);
      }
    }
    http.end();
  }
  else if (httpCode != 200)
  {
    Serial.println("servidor fora! Code: " + String(httpCode));
    http.end();
  }
  cliente.stop();
}

String retornaValorCloud(char val)
{
  if (val == '0')
  {
    return "true";
  }
  else
  {
    return "false";
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\config.ino"
void reiniciar()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");
  delay(1000);
  ESP.restart();
}

String wifiPadrao()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "r");
  String texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  Serial.println("wifipadrao: " + texto);

  return texto;
}

String pegaSSID()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaPassword()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/pass.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaIP()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/ip.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaMask()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/mask.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

String pegaGateway()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/gateway.txt", "r");
  String texto = f.readStringUntil('|');
  f.close();
  SPIFFS.end();

  return texto;
}

void setWifiPadrao(String valor)
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  f.println(valor + "|");

  f.close();
  SPIFFS.end();
}

void configuracao2()
{
  /*

  String ssid = "";

  String pass = "";

  String ip = "";

  String mask = "";

  String gateway = "";*/
# 107 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\config.ino"
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "r");
  String ssid = f.readStringUntil('|');
  f.close();

  f = SPIFFS.open("/pass.txt", "r");
  String pass = f.readStringUntil('|');
  f.close();

  f = SPIFFS.open("/ip.txt", "r");
  String ip = f.readStringUntil('|');
  ip.replace(",", ".");
  f.close();

  f = SPIFFS.open("/mask.txt", "r");
  String mask = f.readStringUntil('|');
  mask.replace(",", ".");
  f.close();

  f = SPIFFS.open("/gateway.txt", "r");
  String gateway = f.readStringUntil('|');
  gateway.replace(",", ".");
  f.close();

  SPIFFS.end();
  configuracao();
}

void wifireset()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ESP resetado");
  wifireset2();
}

void wifireset2()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  String req = "1|";

  f.println(req);
  f.close();

  // volta senha padrao do wifi
  f = SPIFFS.open("/senhaap.txt", "w");

  f.println("12345678|");
  f.close();

  // senha API padrao
  f = SPIFFS.open("/apipass.txt", "w");
  f.println("25d55ad283aa400af464c76d713c07ad|");
  f.close();

  // HTTP usuario
  f = SPIFFS.open("/httpuser.txt", "w");
  f.println("keepin|");
  f.close();

  // HTTP senha
  f = SPIFFS.open("/httppass.txt", "w");
  f.println("keepin|");
  f.close();

  SPIFFS.end();

  ConfigEN(); // configura as entradas como normal

  ESP.restart();
}

void gravawifi()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //SSID
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/ssid.txt", "w");
  }

  String req = server.arg("txtnomerede");

  f.println(req + "|");
  f.close();

  //SENHA
  f = SPIFFS.open("/pass.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/pass.txt", "w");
  }

  req = server.arg("txtsenha");

  f.println(req + "|");
  f.close();

  // IP

  f = SPIFFS.open("/ip.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/ip.txt", "w");
  }

  req = server.arg("txtip");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  //Mask

  f = SPIFFS.open("/mask.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/mask.txt", "w");
  }

  req = server.arg("txtmascara");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  //gateway

  f = SPIFFS.open("/gateway.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/gateway.txt", "w");
  }

  req = server.arg("txtgateway");
  req.replace(".", ",");

  f.println(req + "|");
  f.close();

  ///fim

  f = SPIFFS.open("/wifipadrao.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/wifipadrao.txt", "w");
  }

  req = "0|";

  f.println(req + "|");
  f.close();

  SPIFFS.end();

  String conRetorno = server.arg("ret");

  if (conRetorno == "1")
  {
    server.send(200, "text/html", "ok");
    gravahtml();
  }
  else
  {
    gravahtml();
    configuracao();
  }
}

int retornaPorta(int vporta)
{
  if ((vporta >= 1) && (vporta <= 16))
  {
    return vporta - 1;
  }
  else
  {
    return -1;
  }
}

void IniciaRTC()
{
  // rtc

  Serial.println();

  RtcDateTime now;

  now = carregaHora();
}

RtcDateTime carregaHora()
{
  Rtc.get_time();

  RtcDateTime dt2(Rtc.year, Rtc.month, Rtc.day, Rtc.hour, Rtc.minute, Rtc.second);

  return dt2;
}

void valorwifi()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  int32_t rssi;
  rssi = WiFi.RSSI();

  server.send(200, "text/html", String(rssi));
}

void fmodelo()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "2");
}

String lerMemoria()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "r");
  String texto = f.readStringUntil('|');

  f.close();
  SPIFFS.end();
  return texto;
}

void Memoria()
{

  String retorno = "1"; //lerMemoria();
  if (retorno == "1")
  {

    Rtc.get_chip1();
    Rtc.get_chip2();

    chip1.write8(Rtc.chip1);
    chip2.write8(Rtc.chip2);

    //    Serial.println("Chip1: ");
    //    Serial.println(Rtc.chip1);
    //    Serial.println("Chip2: ");
    //    Serial.println(Rtc.chip2);
  }
  else
  {
    chip1.write8(255);
    chip2.write8(255);
  }
}

void fMemoria()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String valorm = server.arg("m");
  SPIFFS.begin();
  File f = SPIFFS.open("/memoria.txt", "w");
  f.println(valorm + "|");
  f.close();
  TipoMemoria = "1"; //lerMemoria();

  if (TipoMemoria == "1")
  {
    /*

    f = SPIFFS.open("/dm1.txt", "w");

    f.println(String(chip1.read8())+"|");

    f.close();



    f = SPIFFS.open("/dm2.txt", "w");

    f.println(String(chip2.read8())+"|");

    f.close();

    */
# 421 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\config.ino"
    Rtc.chip1 = String(chip1.read8()).toInt();
    Rtc.chip2 = String(chip2.read8()).toInt();
    ;
    Rtc.set_chip1();
    Rtc.set_chip2();
  }
  else
  {
    Rtc.chip1 = 255;
    Rtc.chip2 = 255;
    Rtc.set_chip1();
    Rtc.set_chip2();
  }

  SPIFFS.end();
  server.send(200, "text/html", "ok");
}

void lerConfiguracao()
{
  // garante que o led do IR estará apagado
  IRsend irsend(16, true);
  irsend.begin();
  digitalWrite(16, 0x0);

  // pega o tempo registrado
  SPIFFS.begin();

  //senha padrao wifi (Access Point)
  File f = SPIFFS.open("/senhaap.txt", "r");
  String texto2 = f.readStringUntil('|');
  f.close();
  vSenhaAP = texto2;

  if (vSenhaAP == "")
  {
    vSenhaAP = "12345678";

    f = SPIFFS.open("/senhaap.txt", "w");

    f.println(vSenhaAP + "|");
    f.close();
  }

  // Flag API
  f = SPIFFS.open("/alowapi.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "1")
  {
    AlowApi = true;
  }
  else if (texto2 == "0")
  {
    AlowApi = false;
  }
  else
  {
    AlowApi = false;
    f = SPIFFS.open("/alowapi.txt", "w");
    f.println("1|");
    f.close();
  }

  //SenhaAPI
  f = SPIFFS.open("/apipass.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "")
  {
    ApiPass = "25d55ad283aa400af464c76d713c07ad";
    f = SPIFFS.open("/apipass.txt", "w");
    f.println("25d55ad283aa400af464c76d713c07ad|");
    f.close();
  }
  else
  {
    ApiPass = texto2;
  }

  // Cloud
  f = SPIFFS.open("/cloud.txt", "r");
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "")
  {
    usaCloud = false;
    f = SPIFFS.open("/cloud.txt", "w");
    f.println("0|");
    f.close();
  }
  else if (texto2 == "1")
  {
    usaCloud = true;
  }
  else
  {
    usaCloud = false;
  }

  SPIFFS.end();
  //AlowApi = true;
}

void GravaCloud()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String func = server.arg("f");

  if (vSenha == Senha)
  {
    String req = server.arg("v");
    if (func == "w")
    {
      SPIFFS.begin();

      File f = SPIFFS.open("/cloud.txt", "w");
      f.println(req + "|");
      f.close();
      SPIFFS.end();

      if (req == "1")
      {
        usaCloud = true;
      }
      else
      {
        usaCloud = false;
      }
      server.send(200, "text/html", "1");
    }
    else if (func == "r")
    {
      if (usaCloud == true)
      {
        server.send(200, "text/html", "1");
      }
      else
      {
        server.send(200, "text/html", "0");
      }
    }
    else
    {
      server.send(200, "text/html", "1");
    }
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void dirarquivos()
{
  String arquivos = "";
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
  SPIFFS.begin();
  Serial.println("Consultar sistema de arquivos");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    arquivos += dir.fileName();
    //Serial.print(dir.fileName());
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      arquivos += f.size();
      //Serial.println(f.size());
      f.close();
    }
    arquivos += "<BR>";
  }
  SPIFFS.end();

  arquivos += "*";

  server.send(200, "text/html", arquivos);
}

/*

void ConfigAuth() 

{

  

  SPIFFS.begin();

  

  //Senha HTTP

  File f = SPIFFS.open("/httppass.txt", "r");

  String texto2 = "";

  texto2 = f.readStringUntil('|');

  f.close();

   

  //const char* www_password2;



  if (texto2 == "" || texto2 == " ") 

  {

    www_password2 = "keepin";

    f = SPIFFS.open("/httppass.txt", "w");

    f.println("keepin|");

    f.close();         

  }

  else{

    www_password2 = texto2;

  }

  



  // Usuario HTTP

  f = SPIFFS.open("/httpuser.txt", "r");

  texto2 = "";

  texto2 = f.readStringUntil('|');

  f.close();



  if (texto2 == "" || texto2 == " ") 

  {  

    www_username2 = "keepin";

    f = SPIFFS.open("/httpuser.txt", "w");

    f.println("keepin|");

    f.close();         

  }

  else{

    www_username2 = texto2;

  }



  Serial.println("username: " + String(www_username2));

  Serial.println("password: " + String(www_password2));

 

  SPIFFS.end();    



}

*/
# 659 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\config.ino"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Download()
{ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String path = server.arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  //String path = "/httpuser.txt";
  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    Serial.println("Arquivo existe");

    File download = SPIFFS.open(path, "r");
    //Serial.println(download);
    //size_t sent = server.streamFile(file, "text/html");
    //file.close();

    if (download)
    {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename=" + path);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    }
  }
  else
  {
    Serial.println("Arquivo não existe");
  }
  SPIFFS.end();
}

void File_Upload()
{
  Serial.println("File upload stage-1");
  //append_page_header();
  String webfile = "<h3>Select File to Upload</h3>";
  webfile += "<FORM action='/fupload' method='post' enctype='multipart/form-data'>";
  webfile += "<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>";
  webfile += "<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>";
  webfile += "<a href='/'>[Back]</a><br><br>";
  //append_page_footer();
  Serial.println("File upload stage-2");
  server.send(200, "text/html", webfile);
}

void handleFileUpload()
{ // upload a new file to the Filing system

  Serial.println("File upload stage-3");
  HTTPUpload &uploadfile = server.upload();

  if (uploadfile.status == UPLOAD_FILE_START)
  {
    Serial.println("File upload stage-4");
    String filename = uploadfile.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    Serial.print("Upload File Name: ");
    Serial.println(filename);

    SPIFFS.begin();

    SPIFFS.remove(filename); // Remove a previous version, otherwise data is appended the file again

    UploadFile = SPIFFS.open(filename, "a"); // Open the file for writing in SPIFFS (create it, if doesn't exist)
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    Serial.println("File upload stage-5");
    if (UploadFile)
    {
      //SPIFFS.begin();

      UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
    }
  }
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      Serial.print("Upload Size: ");
      Serial.println(uploadfile.totalSize);

      //append_page_header();
      String webfile = "<h3>File was successfully uploaded</h3>";
      webfile += "<h2>Uploaded File Name: ";
      webfile += uploadfile.filename + "</h2>";
      webfile += "<h2>File Size: OK";
      //webfile += uploadfile.totalSize + "</h2><br>";
      //append_page_footer();
      server.send(200, "text/html", webfile);
      //
      SPIFFS.end();
    }
  }
  else
  {
    Serial.println(uploadfile.totalSize);
    SPIFFS.end();
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void File_Delete()
{ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments

  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String path = server.arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  SPIFFS.begin();
  if (SPIFFS.exists(path))
  {
    Serial.println("Arquivo existe");
    if (SPIFFS.remove(path))
    {
      Serial.println("Removido");
      server.send(200, "text/html", "Removido");
    }
  }
  else
  {
    Serial.println("Arquivo não existe");
    server.send(200, "text/html", "Não existe");
  }
  SPIFFS.end();
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\controles.ino"
void executaPulso(int porta)
{
  if (porta < 8)
  {
    chip1.write(porta, 0x0);
  }
  else
  {
    chip2.write(porta - 8, 0x0);
  }
  g_tempoInicioPulso[porta] = millisAtual;
  g_pulsoHabilita[porta] = true;
}

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base)
{
  for (int i = 0; i < maxBytes; i++)
  {
    bytes[i] = strtoul(str, __null, base);
    str = strchr(str, sep);
    if (str == __null || *str == '\0')
    {
      break;
    }
    str++;
  }
}

void retornachip()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", vchipId);
}

void controle()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  //Serial.println("user: " + String(www_username) + " - pass: " + String(www_password));
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String p = server.arg("p");
  String k = server.arg("k");
  String f = server.arg("f");
  String Nome = server.arg("n");
  int porta = p.toInt();
  int Tipoa = server.arg("pu").toInt();

  //  if (porta == 1)
  //  {
  //porta = 14;
  //}
  porta = retornaPorta(porta);
  if (porta >= 0)
    //pinMode(porta, OUTPUT);

    if (k == "kdi9e")
    {

      if (f == "true")
      {
        //digitalWrite(porta, 1);
        LigaDesliga(porta, 0x1, Nome, Tipoa);
        Serial.println("led 1 ligado - Porta: " + String(porta));
      }
      else
      {
        //digitalWrite(porta, 0);
        LigaDesliga(porta, 0x0, Nome, Tipoa);
        Serial.println("led 1 desligado - Porta: " + String(porta));
      }
    }
  server.send(200, "text/html", "ok");
}

void situacao()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String p = server.arg("p");
  String k = server.arg("k");
  int porta = p.toInt();

  //  if (porta == 1)
  //  {
  //    porta = 14;
  //  }
  porta = retornaPorta(porta);
  if (porta >= 0)
    //pinMode(porta, OUTPUT);

    if (k == "kdi9e")
    {
      if (LePorta(porta) == 0x1)
      {
        server.send(200, "text/html", "true");
      }
      else
      {
        server.send(200, "text/html", "false");
      }
    }
}

void grava()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  SPIFFS.begin();
  File f = SPIFFS.open("/config.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/config.txt", "w");
  }

  f.println("IP: teste");

  f.close();
  SPIFFS.end();
}
void valida()
{

  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "16");
}

void ler()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  SPIFFS.begin();
  File f = SPIFFS.open("/config.txt", "r");
  String texto = f.readStringUntil('\n');
  server.send(200, "text/html", texto);
  f.close();
  SPIFFS.end();
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  // digitalWrite(led, 0);

  //ESP.restart();
}

void LigaDesliga(int vPorta, int vFuncao, String Nome, int Tipo)
{
  if (Tipo != 1) //normal
  {
    if (vFuncao == 0x1)
    {
      vFuncao = 0x0;
    }
    else
    {
      vFuncao = 0x1;
    }

    if (vPorta < 8)
    {
      chip1.write(vPorta, vFuncao);
    }
    else
    {
      chip2.write(vPorta - 8, vFuncao);
    }

    //if (TipoMemoria == "1")
    //{
    Rtc.chip1 = String(chip1.read8()).toInt();
    Rtc.chip2 = String(chip2.read8()).toInt();

    Rtc.set_chip1();
    Rtc.set_chip2();
    //}
  }
  else //pulsado
  {
    executaPulso(vPorta);
  }
}

int LePorta(int vPorta)
{
  if (vPorta < 8)
  {
    if (chip1.read(vPorta) == 0x1)
    {
      return 0x0;
    }
    else
    {
      return 0x1;
    }
  }
  else
  {
    if (chip2.read(vPorta - 8) == 0x1)
    {
      return 0x0;
    }
    else
    {
      return 0x1;
    }
  }
}

int LeSensor(int vPorta)
{
  if (vPorta < 8)
  {
    if (sensor1.read(vPorta) == 0x1)
    {
      return 0x0;
    }
    else
    {
      return 0x1;
    }
  }
  else
  {
    if (sensor2.read(vPorta - 8) == 0x1)
    {
      return 0x0;
    }
    else
    {
      return 0x1;
    }
  }
}

void ApagaPortas()
{
  for (int i = 1; i <= 16; i++)
  {
    if (i <= 8)
    {
      chip1.write(retornaPorta(i), 0x1);
      chip2.write(retornaPorta(i), 0x1);
    }
    else
    {
      chip2.write(retornaPorta(i) - 8, 0x1);
    }
  }

  for (int i = 0; i < 8; i++)
  {
    chip3.write(i, 0x0);
  }

  chip3.write(4, 0x1);
  chip3.write(5, 0x1);
  chip3.write(6, 0x1);
  chip3.write(7, 0x1);
}

void Inverte(int vPorta)
{
  if (vPorta < 8)
  {
    chip1.write(vPorta, !chip1.read(vPorta));
  }
  else
  {
    chip2.write(vPorta - 8, !chip2.read(vPorta - 8));
  }

  //if (TipoMemoria == "1")
  //{
  /*

    SPIFFS.begin();

    File f = SPIFFS.open("/dm1.txt", "w");

    f.println(String(chip1.read8())+"|");

    f.close();



    f = SPIFFS.open("/dm2.txt", "w");

    f.println(String(chip2.read8())+"|");

    f.close();



    SPIFFS.end();       

    */
# 320 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\controles.ino"
  Rtc.chip1 = String(chip1.read8()).toInt();
  Rtc.chip2 = String(chip2.read8()).toInt();

  Rtc.set_chip1();
  Rtc.set_chip2();
  //}
}
/*

// Return RSSI or 0 if target SSID not found

int32_t getRSSI(const char* target_ssid) {

  byte available_networks = WiFi.scanNetworks();



  for (int network = 0; network < available_networks; network++) {

    if (strcmp(WiFi.SSID(network), target_ssid) == 0) {

      return WiFi.RSSI(network);

    }

  }

  return 0;

}

*/
# 341 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\controles.ino"
void CarregaEntradas()
{
  String sSensor1 = String(sensor1.read8(), 2);
  String sSensor2 = String(sensor2.read8(), 2);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  int posicaoSensor;

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    Sensores[posicaoSensor] = sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor);
    if (sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor) == "0")
    {
      estadoAtual[posicaoSensor] = 0x1;
      ultimoEstado[posicaoSensor] = 0x1;
    }
    else
    {
      estadoAtual[posicaoSensor] = 0x0;
      ultimoEstado[posicaoSensor] = 0x0;
    }
  }

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    Sensores[posicaoSensor + 8] = sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor);

    if (sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor) == "0")
    {
      estadoAtual[posicaoSensor + 8] = 0x1;
      ultimoEstado[posicaoSensor + 8] = 0x1;
    }
    else
    {
      estadoAtual[posicaoSensor + 8] = 0x0;
      ultimoEstado[posicaoSensor + 8] = 0x0;
    }
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ddns.ino"
void chamaddns() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String sIP = server.arg("ip");
  String Senha = server.arg("s");
  int Porta = server.arg("p").toInt();
  String funcao = server.arg("f");
  int IdChip = server.arg("c").toInt();
  String Tipo = server.arg("t");
  String Nome = server.arg("n");
  int Tipoa = server.arg("pu").toInt();

  IPAddress Destino;

  Destino.fromString(sIP);

  if (Destino != IpDispositivo) {

    if (Senha == "kdi9e")
    {
      char replyPacekt[255] = "";
      String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip)+"|"+Tipo+"|";

      if (Tipo == "A" || Tipo == "G")
      {
        Texto += server.arg("j") + "|n|";
      }
      else if (Tipo == "B")
      {
        Texto += server.arg("j") + "|"+server.arg("b")+"|";
      }
      else if (Tipo == "E" || Tipo == "X")
      {
        Texto += String(Tipoa) + "|n|";
      }
      else
      {
        Texto += "n|n|";
      }


      Texto.toCharArray(replyPacekt, 255);

      Serial.println("enviado comando UDP");
      Serial.println(Texto);
      Udp.beginPacket(Destino, localUdpPort);
      Udp.write(replyPacekt);
      Udp.endPacket();

      if (Tipo == "E")
      {
        //server.send(200, "text/html", "ok");      
      }
      else
      {
         //server.send(200, "text/html", "ok");      
      }
    }
  }
  else
  {
    Porta = retornaPorta(Porta);
    if (Porta >= 0)
    {
      if (Senha == "kdi9e")
      {
        if (Tipo == "E")
        {
          if (funcao=="true")
          {
            //digitalWrite(Porta, 1);
            LigaDesliga(Porta, 0x1, Nome, Tipoa);
            Serial.println("led 1 ligado");
            server.send(200, "text/html", "ok");
          }
          else
          {
            //digitalWrite(Porta, 0);
            LigaDesliga(Porta, 0x0, Nome, Tipoa);
            Serial.println("led 1 desligado");
            server.send(200, "text/html", "ok");
          }
        }
        else
        if (Tipo == "C")
        {
          if (LePorta(Porta) == 0x1)
          {
            server.send(200, "text/html", "true");
          }
          else
          {
            server.send(200, "text/html", "false");
          }
        }
        else
        if (Tipo == "S")
        {
          if (ultimoDisparo == "")
          {
            String sSensor1 = String(sensor1.read8(), 2);
            String sSensor2 = String(sensor2.read8(), 2);

            while (sSensor1.length() < 8)
            {
              sSensor1 = '0' + sSensor1;
            }

            while (sSensor2.length() < 8)
            {
              sSensor2 = '0' + sSensor2;
            }

            server.send(200, "text/html", sSensor1 + sSensor2);
          }
          else
          {
            server.send(200, "text/html", ultimoDisparo);
          }
        }
        else if (Tipo == "N")
        {
            server.send(200, "text/html", String(notificar));
        }
        else if (Tipo == "A")
        {
          String Texto = server.arg("j");
          gravasensor2(Texto);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "G")
        {
          String Texto = server.arg("j");
// liberar depois que colocar o arquivo de RF          gravarf2(Texto);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "B")
        {
          String Texto = server.arg("j");
          String Telef = server.arg("b");
          gravasms2(Texto, Telef);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "D")
        {
          server.send(200, "text/html", consultasms2());
        }
        else if (Tipo == "F")
        {
          String Valor = server.arg("j");
          gravanot2(Valor);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "I")
        {
          Inverte(Porta);
        }
        else if (Tipo == "H")
        {
          String sSensor1 = String(sensor1.read8(), 2);
          String sSensor2 = String(sensor2.read8(), 2);

          while (sSensor1.length() < 8)
          {
            sSensor1 = '0' + sSensor1;
          }

          while (sSensor2.length() < 8)
          {
            sSensor2 = '0' + sSensor2;
          }

          if (DeviceAlterado)
          {
            //trataDevice();
          }

          String sChip1 = String(chip1.read8(), 2);
          String sChip2 = String(chip2.read8(), 2);

          while (sChip1.length() < 8)
          {
            sChip1 = '0' + sChip1;
          }

          while (sChip2.length() < 8)
          {
            sChip2 = '0' + sChip2;
          }

          // separa os valores ligos no array
          String valorSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
          int posicaoSensor;



          for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
          {
            valorSensores[posicaoSensor] = sSensor1.substring(7-posicaoSensor, 8-posicaoSensor);
          }

          for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
          {
            valorSensores[posicaoSensor+8] = sSensor2.substring(7-posicaoSensor, 8-posicaoSensor);
          }

          int32_t rssi;
          rssi = WiFi.RSSI();
          char replyPacekt[255] = "";
          String valSensores = sSensor1 + sSensor2;
          valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) +"." + String(IpDispositivo[3]) + "|" + valSensores+"|" + sChip1+sChip2+"|"+rssi+"*";
          valSensores.toCharArray(replyPacekt, 255);
          server.send(200, "text/html", replyPacekt);

        }
      }
    }
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\fab.ino"
bool ConfigEN() {
    String texto = "1|6|1|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "2|6|2|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "3|6|3|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "4|6|4|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "5|6|5|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "6|6|6|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "7|6|7|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "8|6|8|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";

    texto += "9|6|9|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "10|6|10|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "11|6|11|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "12|6|12|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "13|6|13|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "14|6|14|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "15|6|15|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "16|6|16|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|*";

    gravasensor2(texto);

    //Serial.println(texto);
    return true;
}

bool ConfigEP() {
    String texto = "1|5|1|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "2|5|2|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "3|5|3|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "4|5|4|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "5|5|5|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "6|5|6|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "7|5|7|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "8|5|8|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";

    texto += "9|5|9|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "10|5|10|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "11|5|11|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "12|5|12|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "13|5|13|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "14|5|14|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "15|5|15|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|";
    texto += "16|5|16|1|"+String(IpDispositivo[0])+"."+String(IpDispositivo[1])+"."+String(IpDispositivo[2])+"."+String(IpDispositivo[3])+"|"+vchipId+"|0|0|0|0|0|*";

    gravasensor2(texto);

    //Serial.println(texto);
    return true;
}

bool resetIntNormal ()
{
    int val1 = sensor1.read(0);
    int val2 = sensor1.read(1);
    int val3 = sensor1.read(2);

    if (val1 == 0 && val2 == 0 && val3 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool resetIntPulsado ()
{
    int val1 = sensor1.read(0);
    int val2 = sensor1.read(1);
    int val3 = sensor1.read(2);
    int val4 = sensor1.read(3);

    if (val1 == 0 && val2 == 0 && val3 == 0 && val4 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
void configIR()
{

  save.rawbuf = new uint16_t[irrecv.getBufSize()];
  if (save.rawbuf == __null)
  {
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
    Serial.print("UNKNOWN");
    Modelo = 0;
    break;
  case NEC:
    Serial.print("NEC");
    Modelo = 1;
    break;
  case NEC_LIKE:
    Serial.print("NEC (non-strict)");
    Modelo = 2;
    break;
  case SONY:
    Serial.print("SONY");
    Modelo = 3;
    break;
  case RC5:
    Serial.print("RC5");
    Modelo = 4;
    break;
  case RC5X:
    Serial.print("RC5X");
    Modelo = 5;
    break;
  case RC6:
    Serial.print("RC6");
    Modelo = 6;
    break;
  case RCMM:
    Serial.print("RCMM");
    Modelo = 7;
    break;
  case DISH:
    Serial.print("DISH");
    Modelo = 8;
    break;
  case SHARP:
    Serial.print("SHARP");
    Modelo = 9;
    break;
  case JVC:
    Serial.print("JVC");
    Modelo = 10;
    break;
  case SANYO:
    Serial.print("SANYO");
    Modelo = 11;
    break;
  case SANYO_LC7461:
    Serial.print("SANYO_LC7461");
    Modelo = 12;
    break;
  case MITSUBISHI:
    Serial.print("MITSUBISHI");
    Modelo = 13;
    break;
  case SAMSUNG:
    Serial.print("SAMSUNG");
    Modelo = 14;
    break;
  case LG:
    Serial.print("LG");
    Modelo = 15;
    break;
  case WHYNTER:
    Serial.print("WHYNTER");
    Modelo = 16;
    break;
  case AIWA_RC_T501:
    Serial.print("AIWA_RC_T501");
    Modelo = 17;
    break;
  case PANASONIC:
    Serial.print("PANASONIC");
    Modelo = 18;
    break;
  case DENON:
    Serial.print("DENON");
    Modelo = 19;
    break;
  case COOLIX:
    Serial.print("COOLIX");
    Modelo = 20;
    break;
  case GREE:
    Serial.print("GREE");
    Modelo = 21;
    break;
  }
  if (results->repeat)
    Serial.print(" (Repeat)");
}

// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
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
    tamanho = String(results->bits, 10).toInt();
  }
  //  Serial.print(results->bits, DEC);
  //  Serial.println(" bits)");
}

uint16_t getCookedLength(decode_results *results)
{
  uint16_t length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++)
  {
    uint32_t usecs = results->rawbuf[i] * kRawTick /* Deprecated. For legacy user code support only.*/;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    length += (usecs / 65535) * 2;
  }
  return length;
}

// Dump out the decode_results structure.
//
void dumpRaw(decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen - 1, 10);
  Serial.println("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    if (i % 100 == 0)
      yield(); // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0)
    { // even
      Serial.print("-");
    }
    else
    { // odd
      Serial.print("   +");
    }
    Serial.printf("%6d", results->rawbuf[i] * kRawTick /* Deprecated. For legacy user code support only.*/);
    if (i < results->rawlen - 1)
      Serial.print(", "); // ',' not needed for last one
    if (!(i % 8))
      Serial.println("");
  }
  Serial.println(""); // Newline
}

// Dump out the decode_results structure.
//

void dumpCode(decode_results *results)
{

  // Start declaration
  String codigoIR2 = "";
  Serial.print("uint16_t "); // variable type
  Serial.print("rawData["); // array name
  tamanho = String(getCookedLength(results), 10).toInt();
  Serial.print(getCookedLength(results), 10); // array size
  codigoIR2 = "{";
  Serial.print("] = {"); // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++)
  {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * kRawTick /* Deprecated. For legacy user code support only.*/;
         usecs > 65535;
         usecs -= 65535)
    {
      Serial.printf("%d, 0", 65535);
      codigoIR2 += printf("%d, 0", 65535);
    }
    Serial.print(usecs, 10);
    codigoIR2 += String(usecs, 10);
    if (i < results->rawlen - 1)
    {
      Serial.print(", "); // ',' not needed on last one
      codigoIR2 += ", ";
    }
    if (i % 2 == 0)
    {
      Serial.print(" "); // Extra if it was even.
      codigoIR2 += " ";
    }
  }

  // End declaration
  Serial.print("};"); //
  codigoIR2 += "}";
  //  Serial.println("tamanho");
  // Serial.println(String(tamanho));
  //Serial.println("codigoIR2");
  //Serial.println(codigoIR2);

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  serialPrintUint64(results->value, 16);
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
# 262 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
    codigoIR = "" + uint64ToString(results->value, 16) + "";
  }
  // Serial.println("CodigoIR: " + codigoIR) ;

  Serial.println("Modelo: " + String(Modelo));
  if (Modelo == 0)
  {
    SPIFFS.begin();
    File rFile = SPIFFS.open("/ir_" + codigoIR + ".cfg", "w");
    rFile.println(codigoIR2);
    rFile.close();
    SPIFFS.end();

    Serial.print("arquivo: ");
    Serial.println(codigoIR2);
  }

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN)
  {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0)
    {
      Serial.print("uint32_t address = 0x");
      Serial.print(results->address, 16);
      Serial.println(";");
      Serial.print("uint32_t command = 0x");
      Serial.print(results->command, 16);
      Serial.println(";");
    }

    // All protocols have data
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

      /*      Serial.println("");

      Serial.print("Codigo 64: ");

      Serial.println(codigoIR);



      Serial.print("Tamanho: ");

      Serial.println(tamanho);*/
# 317 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
    }
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

  Serial.println(Codigo + Codigo2);

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
    else if (vModelo == 2) // NEC (non-strict)
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_NEC
      irsend.sendNEC(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 3) //Sony
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
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
      ///Serial.println(Codigo);
      unsigned long vCodigo = strtoul(Codigo.c_str(), 0, 16) + strtoul(Codigo2.c_str(), 0, 16);
      //uint64_t vCodigo2 = vCodigo;
      //#if SEND_SHARP
      irsend.sendSharp(vCodigo, QtdeBit);
      //#endif
    }
    else if (vModelo == 10) // JVC
    {
      Codigo = "0x" + Codigo;
      ///Serial.println(Codigo);
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
      ///Serial.println(Codigo);
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
      ////Serial.println(Codigo);
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
      //Serial.println("DENON OK");
      //Serial.println(vCodigo);
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
      Serial.println("");
      Serial.println("GREE");
      Serial.println("Qtde Bit");
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
# 629 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
      //      Serial.println("Qtde: " + String(QtdeBit));

      //      for (int i = 0; i <= QtdeBit-1; i++)
      //      {
      //        Serial.println(String(rawData[i]));
      //      }

      //#if SEND_RAW
      irsend.sendRaw(rawData, QtdeBit, 38);
      Serial.println("enviado...\n");
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
# 653 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
    }

    Codigo = "";
    Codigo2 = "";
    digitalWrite(16, 0x0);
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
# 741 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\ir.ino"
//return rawData;
/*}*/

void habir()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  irrecv.resume();
  irrecv.resume();
  irrecv.resume();

  enReadIR = true;
  server.send(200, "text/html", "ok");
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
    chip3.write(0, 0x0);
    chip3.write(1, 0x0);
    chip3.write(2, 0x0);
  }
  else if (PortIR2 == 2)
  {
    chip3.write(0, 0x1);
    chip3.write(1, 0x0);
    chip3.write(2, 0x0);
  }
  else if (PortIR2 == 3)
  {
    chip3.write(0, 0x0);
    chip3.write(1, 0x1);
    chip3.write(2, 0x0);
  }
  else if (PortIR2 == 4)
  {
    chip3.write(0, 0x1);
    chip3.write(1, 0x1);
    chip3.write(2, 0x0);
  }
  else if (PortIR2 == 5)
  {
    chip3.write(0, 0x0);
    chip3.write(1, 0x0);
    chip3.write(2, 0x1);
  }
  else if (PortIR2 == 6)
  {
    chip3.write(0, 0x1);
    chip3.write(1, 0x0);
    chip3.write(2, 0x1);
  }
  else if (PortIR2 == 7)
  {
    chip3.write(0, 0x0);
    chip3.write(1, 0x1);
    chip3.write(2, 0x1);
  }
  else if (PortIR2 == 8)
  {
    chip3.write(0, 0x1);
    chip3.write(1, 0x1);
    chip3.write(2, 0x1);
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
  else if (vModelo == 2) // NEC (non-strict)
  {
    vCod = "0x" + vCod;
    ///Serial.println(Codigo);
    unsigned long vCodigo = strtoul(vCod.c_str(), 0, 16) + strtoul(vCod2.c_str(), 0, 16);
    //uint64_t vCodigo2 = vCodigo;
    //#if SEND_NEC
    irsend.sendNEC(vCodigo, QtdeBit);
    //#endif
  }
  else if (vModelo == 3) //Sony
  {
    vCod = "0x" + vCod;
    //Serial.println(vCod);
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
                                        //      Serial.println("DENON OK");
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
    Serial.println("enviado");
    //#endif
  }

  digitalWrite(16, 0x0);
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
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\leituras.ino"
void leituraUDP()
{

//UDP
int packetSize = Udp.parsePacket();
  int len = Udp.read(incomingPacket, 255);
  if (len > 0)
  {
    incomingPacket[len] = 0;
  }

  String cabecalho = "";
  for (int i = 0; i < 255; i++)
  {
    cabecalho += incomingPacket[i];
  }

  if (packetSize > 16)
  {
    //Serial.println(cabecalho.substring(0,7));
    if (cabecalho.substring(0,7) == "2934d04")
    {
      //Serial.println("cabeçalho");
      //Serial.println(cabecalho);
      //Serial.println("2934d03" + cabecalho.substring(7));
      server.send(200, "text/html", "2934d03" + cabecalho.substring(7));
    }
    else
    {

      // receive incoming UDP packets
  //    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  //    Serial.printf("UDP packet contents: %s\n", incomingPacket);

      String sIP = "";
      String Porta = "";
      int i2 = 0;
      String funcao = "";
      String ChipId = "";
      String Tipo = "";
      String Texto = "";
      String Telefone = "";

      for (int i = 0; i < 255; i++)
      {
        if (incomingPacket[i] != '|' && i2 <= 6)
        {
          if (i2 == 0)
          {
            sIP += incomingPacket[i];
          }

          if (i2 == 1)
          {
            Porta += incomingPacket[i];
          }
          if (i2 == 2)
          {
            funcao += incomingPacket[i];
          }
          if (i2 == 3)
          {
            ChipId += incomingPacket[i];
          }
          if (i2 == 4)
          {
            Tipo += incomingPacket[i];
          }
          if (i2 == 5)
          {
            Texto += incomingPacket[i];
          }
          if (i2 == 6)
          {
            Telefone += incomingPacket[i];
          }

        }
        else
        {
          i2++;
        }
      }
  //    Serial.println("");
      //Serial.println("IP: " + sIP);
      //Serial.println("Porta: " + Porta);
      //Serial.println("Funcao: " + funcao);
      //Serial.println("ChipId: " + ChipId);
      //Serial.println("Tipo: " + Tipo);
      IPAddress Destino;
      Destino.fromString(sIP);

    // if (Porta != "")
        //pinMode(Porta.toInt(), OUTPUT);          

      //if (Porta.toInt() == 1)
      //{
  //      Porta = "14";
    //  }

      if (Destino == IpDispositivo) {
        Porta = String(retornaPorta(Porta.toInt()));
        if (Tipo == "E")
        {
          if (funcao=="true")
          {
            //digitalWrite(Porta.toInt(), 1);
            LigaDesliga(Porta.toInt(), 0x1, "", Texto.toInt());
            Serial.println("led ligado UDP");
          }
          else
          {
            //digitalWrite(Porta.toInt(), 0);
            LigaDesliga(Porta.toInt(), 0x0, "", Texto.toInt());
            Serial.println("led 1 desligado UDP");
          }
        }
        else if (Tipo == "C")
        {
          if (LePorta(Porta.toInt()) == 0x1)
          {
            server.send(200, "text/html", "true");

            Tipo = "R";
            funcao = "true";
            char replyPacekt[255] = "";
            IPAddress ipRetorno = Udp.remoteIP();
            String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId+"|"+Tipo+"|";
            Texto.toCharArray(replyPacekt, 255);

            Serial.println("enviado comando UDP");
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write(replyPacekt);
            Udp.endPacket();

          } else
          {
            server.send(200, "text/html", "false");

            Tipo = "R";
            funcao = "false";
            char replyPacekt[255] = "";
            IPAddress ipRetorno = Udp.remoteIP();
            String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId+"|"+Tipo+"|";
            Texto.toCharArray(replyPacekt, 255);

            //Serial.println("enviado comando UDP");
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write(replyPacekt);
            Udp.endPacket();

          }
        }
        else if (Tipo == "R")
        {
          if (funcao == "true")
          {
            server.send(200, "text/html", "true");
          }
          else
          {
            server.send(200, "text/html", "false");
          }
        }
        else if (Tipo == "S")
        {
            if (ultimoDisparo == "")
            {
              String sSensor1 = String(sensor1.read8(), 2);
              String sSensor2 = String(sensor2.read8(), 2);

              while (sSensor1.length() < 8)
              {
                sSensor1 = '0' + sSensor1;
              }

              while (sSensor2.length() < 8)
              {
                sSensor2 = '0' + sSensor2;
              }

              server.send(200, "text/html", sSensor1 + sSensor2);
            }
            else
            {
              server.send(200, "text/html", ultimoDisparo);
            }
        }
        else if (Tipo == "N")
        {
          server.send(200, "text/html", String(notificar));
        }
        else if (Tipo == "A")
        {
          gravasensor2(Texto);
          server.send(200, "text/html", "ok");
        }
        else if (Tipo == "B")
        {
            String Texto = server.arg("j");
            String Telef = server.arg("b");
            gravasms2(Texto, Telef);
            server.send(200, "text/html", "ok");
        }
        else if (Tipo == "D")
        {
            server.send(200, "text/html", consultasms2());
        }
        else if (Tipo == "F")
        {
            String Valor = server.arg("j");
            gravanot2(Valor);
            server.send(200, "text/html", "ok");
        }
        else if (Tipo == "X") // retorno udp valor do dimmer
        {
            server.send(200, "text/html", funcao);
        }
        else if (Tipo == "I")
        {
          Inverte(Porta.toInt());
        }
        else if (Tipo == "H")
        {
            String sSensor1 = String(sensor1.read8(), 2);
            String sSensor2 = String(sensor2.read8(), 2);

            while (sSensor1.length() < 8)
            {
              sSensor1 = '0' + sSensor1;
            }

            while (sSensor2.length() < 8)
            {
              sSensor2 = '0' + sSensor2;
            }

            if (DeviceAlterado)
            {
              //trataDevice();
            }


            String sChip1 = String(chip1.read8(), 2);
            String sChip2 = String(chip2.read8(), 2);

            while (sChip1.length() < 8)
            {
              sChip1 = '0' + sChip1;
            }

            while (sChip2.length() < 8)
            {
              sChip2 = '0' + sChip2;
            }

            // separa os valores ligos no array
            String valorSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
            int posicaoSensor;



            for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
            {
              valorSensores[posicaoSensor] = sSensor1.substring(7-posicaoSensor, 8-posicaoSensor);
            }

            for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
            {
              valorSensores[posicaoSensor+8] = sSensor2.substring(7-posicaoSensor, 8-posicaoSensor);
            }

            int32_t rssi;
            rssi = WiFi.RSSI();
            char replyPacekt[255] = "";
            String valSensores = sSensor1 + sSensor2;
            valSensores = "2934d04" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) +"." + String(IpDispositivo[3]) + "|" + valSensores+"|" + sChip1+sChip2+"|"+rssi+"*";
            valSensores.toCharArray(replyPacekt, 255);
            //retorna udp


            //Serial.println(Contador);
        //    Contador = 0;  

            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write(replyPacekt);
            Udp.endPacket();

            //server.send(200, "text/html", replyPacekt);          

        }

        //retorna udp
        char replyPacekt[255] = "";
        String Texto = "ok";
        Texto.toCharArray(replyPacekt, 255);

        //Serial.println("enviado comando UDP");
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(replyPacekt);
        Udp.endPacket();
      }
    }
  }
  else if (packetSize == 2)
  {
    server.send(200, "text/html", "ok");
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
/*

void mesh() {

  //const char* www_username = www_username2.c_str();

  //const char* www_password = www_password2.c_str();

//if(!server.authenticate(www_username, www_password))

    //return server.requestAuthentication();

  

  // get the value of request argument "state" and convert it to an int

  int state = server.arg("state").toInt();



    String sIP = server.arg("ip");

    String Senha = server.arg("s");

    int Porta = server.arg("p").toInt();

    String funcao = server.arg("f");

    int IdChip = server.arg("c").toInt();

    String Tipo = server.arg("t");

    IPAddress Destino;







  server.send(200, "text/plain", String("LED is now "));

  WiFi.disconnect(); 

  conectarmesh();

      

  Serial.println("");

  if ((state != 1) and (state != 0)){

    /* Scan for other nodes and send them a message */
# 28 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
    /*

    char request[60];

    char MeshNode[60];



    sprintf(MeshNode, "Mesh_Node%d", state);

    

    Serial.println("Entrou...");  

    Serial.print("Tentdo encontrar ");

    Serial.println(MeshNode);

    server.send(200, "text/plain", String("LED is now ") + ((state)));





    

    String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip)+"|"+Tipo+"|"+Senha+"|";

    

    sprintf(request, Texto.c_str(), request_i++, ESP.getChipId());

    

    //mesh_node.attemptScan(request);

    mesh_node.attemptScanNodeSend(MeshNode,request);

    delay(1000);

    

    Serial.println("Saiu..");

        

    Serial.println(WiFi.status());

    //if (WiFi.status()== 3) {

      WiFi.disconnect();

      Serial.println(WiFi.status());

    //};

    //if (WiFi.status()!= WL_CONNECTED){

    // Connect to WiFi network

    server.send(200, "text/plain", String("LED is now ") + ((state)));

    conectar();

    //ESP.restart();

  }

  else if (state == 1){

    /* Scan for other nodes and send them a message */
# 64 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
    /*

    char request[60];

    Serial.println("Entrou...");

    Serial.println("Tentdo encontrar *");

    server.send(200, "text/plain", String("LED is now ") + ((state)));



    String Texto = sIP + "|" + String(Porta) + "|" + funcao + "|" + String(IdChip)+"|"+Tipo+"|";

    

    sprintf(request, Texto.c_str(), request_i++, ESP.getChipId());

    mesh_node.attemptScan(request);

    //mesh_node.attemptScanNodeSend("Mesh_Node1026892",request);

    delay(1000);

    

    Serial.println("Saiu..");

    



    

    Serial.println(WiFi.status());

    //if (WiFi.status()== 3) {

      WiFi.disconnect();

      Serial.println(WiFi.status());

    //};

    //if (WiFi.status()!= WL_CONNECTED){

      // Connect to WiFi network

    server.send(200, "text/plain", String("LED is now ") + ((state)));

    conectar();

    //ESP.restart();

   };



  server.send(200, "text/plain", String("LED is now ") + ((state)));



}





void meshconsulta() {

  //const char* www_username = www_username2.c_str();

  //const char* www_password = www_password2.c_str();

  //if(!server.authenticate(www_username, www_password))

    //return server.requestAuthentication();

  

  // get the value of request argument "state" and convert it to an int



  String sIP = server.arg("ip");

  String Senha = server.arg("s");

  int Porta = server.arg("p").toInt();

  String funcao = server.arg("f");

  int IdChip = server.arg("c").toInt();

  String Tipo = server.arg("t");



  if ((IdChip == mesh_ChipId.toInt()) && (mesh_Tipo=="R"))

  {

    server.send(200, "text/plain", mesh_funcao);

  }

  else

  {

    server.send(200, "text/plain", "-1");

  }

}



*/
# 124 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
void conectar() {
  WiFi.disconnect();
  WiFi.softAPdisconnect();

  String retorno = wifiPadrao();
  vConfigWIFI = retorno;

  Serial.println("Valor de retorno: " + retorno);
  if (retorno == "0")
  {
    WiFi.mode(WIFI_STA);
    listawifi();
    tipoWifiAtual = 1;
    String ssid_STR = pegaSSID();
    const char *ssid = ssid_STR.c_str();


    String password_STR = pegaPassword();
    const char* password = password_STR.c_str();

    String ip_STR = pegaIP();
    const char* ipStr = ip_STR.c_str();
    byte ip2[4];
    parseBytes(ipStr, ',', ip2, 4, 10);
    IPAddress ip(ip2);

    String mask_STR = pegaMask();
    const char* maskStr = mask_STR.c_str();
    byte mask2[4];
    parseBytes(maskStr, ',', mask2, 4, 10);
    IPAddress subnet(mask2); // set gateway to match your network

    String gateway_STR = pegaGateway();
    const char* gatewayStr = gateway_STR.c_str();
    byte gateway2[4];
    parseBytes(gatewayStr, ',', gateway2, 4, 10);
    IPAddress gateway(gateway2);

    IPAddress dns(8,8,8,8);


    Serial.println(ip);
    Serial.println(gateway);
    Serial.println(subnet);
    Serial.println(ssid);
    Serial.println(password);

    IpDispositivo = ip;

    local_IP = ip;

    WiFi.config(ip, gateway, subnet, DNS1, DNS2);
    WiFi.begin(ssid, password);
    Serial.println("");
    counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
      if(counter > 20){
          WiFi.mode(WIFI_AP);
          tipoWifiAtual = 2;
          listawifi();
          chip3.write(LedWiFI, 0x1);
          Serial.println("Rede não localizada");
          IPAddress local_IP(192,168,4,1);
          IPAddress gateway(192,168,4,1);
          IPAddress subnet(255,255,255,0);

          Serial.print("Setting soft-AP configuration ... ");
          Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

          Serial.print("Setting soft-AP ... ");
          Serial.println("idencitifcador");
          //int chipId = ESP.getChipId();
          String NomeRede = "KEEPIN_" + vchipId;
          Serial.println(NomeRede);
          const char *nRede = NomeRede.c_str();
          Serial.println(nRede);

          //Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

          if (! WiFi.softAP(nRede, vSenhaAP.c_str())) {
            wifireset2();
          }

          Serial.print("Soft-AP IP address = ");
          Serial.println(WiFi.softAPIP());
          IpDispositivo = local_IP;

          break;

      }
      else
      {
        chip3.write(LedWiFI, 0x0);
        ///mesh_node.begin();        
      }

      delay(500);
      Serial.print(".");
      counter++;

      int ValorbuttonState = digitalRead(buttonState);

      if (ValorbuttonState == 0x1) {
        Serial.print("resetando");
        WiFi.disconnect();
        wifireset2();
        ESP.restart();
      }

    }
    //Verifica o nivel do sinal wifi
    //int32_t rssi = getRSSI(SSID);

  }
  else
  {
    WiFi.mode(WIFI_AP);
    tipoWifiAtual = 2;
    chip3.write(LedWiFI, 0x1);

    Serial.println();

    listawifi();
    Serial.println(vListaWifi);


    IPAddress local_IP(192,168,4,1);
    IPAddress gateway(192,168,4,1);
    IPAddress subnet(255,255,255,0);

    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println("idencitifcador");
    //int chipId = ESP.getChipId();
    String NomeRede = "KEEPIN_" + vchipId;
    Serial.println(NomeRede);
    const char *nRede = NomeRede.c_str();
    Serial.println(nRede);

    if (! WiFi.softAP(nRede, vSenhaAP.c_str())) {
      wifireset2();
    }

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    IpDispositivo = local_IP;
  }

}

/*

void conectarmesh()

{

  WiFi.disconnect();    

  

  String retorno = wifiPadrao();



          Serial.println("Rede não localizada");    

          IPAddress local_IP(192,168,4,1);

          IPAddress gateway(192,168,4,1);

          IPAddress subnet(255,255,0,0);



          IPAddress mesh_ip(0,0,0,0);

          IPAddress mesh_gateway(0,0,0,0);

          IPAddress mesh_subnet(0,0,0,0);

          

          

          WiFi.config(mesh_ip, mesh_gateway, mesh_subnet);





          Serial.print("Setting soft-AP configuration ... ");

          Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

      

          Serial.print("Setting soft-AP ... ");

          Serial.println("idencitifcador");

          int chipId = ESP.getChipId();

          String NomeRede = "ESP_" + String(chipId);

          Serial.println(NomeRede);

          const char *nRede = NomeRede.c_str();

          Serial.println(nRede);

          

          Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

      

          Serial.print("Soft-AP IP address = ");

          Serial.println(WiFi.softAPIP());  

          IpDispositivo = local_IP;

  

}

*/
# 317 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
/*

String manageRequest(String request)

{

  mesh_sIP = "";

  mesh_Porta = "";

  mesh_funcao = "";

  mesh_ChipId = "";

  mesh_Tipo = "";

  mesh_Senha = "";  

  /* Print out received message */
# 327 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
  /*
/*
  Serial.print("received: ");

  Serial.println(request);



    int i2 = 0;

   

    for (int i = 0; i < 255; i++) 

    {

      if (request[i] != '|' && i2 <= 5)

      { 

        if (i2 == 0)

        {       

          mesh_sIP += request[i];

        }



        if (i2 == 1)

        {       

          mesh_Porta += request[i];

        }

        if (i2 == 2)

        {       

          mesh_funcao += request[i];

        }

        if (i2 == 3)

        {       

          mesh_ChipId += request[i];

        }    

        if (i2 == 4)

        {       

          mesh_Tipo += request[i];

        }        

        if (i2 == 5)

        {       

          mesh_Senha += request[i];

        }        

            

      }      

      else

      {

        i2++;

      }

    }

    Serial.println("");

    Serial.println("IP: " + mesh_sIP);

    Serial.println("Porta: " + mesh_Porta);

    Serial.println("Funcao: " + mesh_funcao);

    Serial.println("ChipId: " + mesh_ChipId);

    Serial.println("Tipo: " + mesh_Tipo);



    String Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";



  /* return a string to send back */
# 379 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
  /*

  char response[60];



  if (mesh_Senha == "kdi9e")

  {



    mesh_Porta = String(retornaPorta(mesh_Porta.toInt()));

    if (mesh_Porta != "")

      //pinMode(mesh_Porta.toInt(), OUTPUT);          

    

//    if (mesh_Porta.toInt() == 1)

//    {

//      mesh_Porta = "14";

//    }



      if ((mesh_Tipo == "E") && (mesh_ChipId.toInt() == ESP.getChipId()))

      {

        if (mesh_funcao=="true")

        {

          //digitalWrite(mesh_Porta.toInt(), 1);

          LigaDesliga(mesh_Porta.toInt(), HIGH, "", 0);

          mesh_funcao = "true";

          mesh_Tipo = "R";

          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";

          Serial.println("led ligado UDP");        

          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());

          return response;

          

        }

        else 

        {

          //digitalWrite(mesh_Porta.toInt(), 0);

          LigaDesliga(mesh_Porta.toInt(), LOW, "", 0);

          Serial.println("led 1 desligado UDP");      

          mesh_Tipo = "R";

          mesh_funcao = "false";

          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";

          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());

          return response;          

        }

      }

      else

      if ((mesh_Tipo == "C") && (mesh_ChipId.toInt() == ESP.getChipId())){

        if (LePorta(mesh_Porta.toInt()) == HIGH) {

          mesh_funcao = "true";

          mesh_Tipo = "R";

          Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";

          sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());

          return response;          

        }

//        else

        //{

//          mesh_Tipo = "R";

  //        mesh_funcao = "false";

    //      Texto = mesh_sIP + "|" + mesh_Porta + "|" + mesh_funcao + "|" + mesh_ChipId+"|"+mesh_Tipo+"|"+mesh_Senha+"|";

      //    sprintf(response, Texto.c_str(), response_i++, ESP.getChipId());

        //  return response;          

        //}

      }

   }

}

*/
# 441 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
void listawifi()
{

  Serial.println("consultou");
  int numberOfNetworks = WiFi.scanNetworks();
  vListaWifi = "";

  for(int i =0; i<numberOfNetworks; i++)
  {
      vListaWifi += WiFi.SSID(i);
      vListaWifi += "|";
  }
  vListaWifi += "*";
  server.send(200, "text/html", vListaWifi);
}

void listawifi2()
{

  server.send(200, "text/html", vListaWifi);
}

void gravasenhawifi()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String vSenha = String(server.arg("s"));
  String req = server.arg("v");
  String CurrentPass = server.arg("a");

  if (vSenha == Senha)
  {
    if (req == "")
    {
      req = "12345678"; // se não houver registro, vai para o padrão
    }

    if (vSenhaAP == CurrentPass) {

      SPIFFS.begin();
      File f = SPIFFS.open("/senhaap.txt", "w");

      f.println(req+"|");
      f.close();

      SPIFFS.end();
      vSenhaAP = req;
      server.send(200, "text/html", "ok");
      Serial.println("Alterado: " + vSenhaAP);
    }
    else {
      server.send(200, "text/html", "-1");
      Serial.println("Senha invalida: Atual = " + vSenhaAP + " Informada: " + CurrentPass);
    }
  }
  else {
    Serial.println("erro de senha de comunicacao: Senha Registrada: " + Senha + " senha enviada: " + vSenha);
    server.send(200, "text/html", "-1");
  }
}

void gravasenhahttp()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

    String vSenha = server.arg("s");
    String vreq = server.arg("v");
    String CurrentPass = server.arg("a");
    String vUsuarioAntigo = server.arg("ua");
    String vUsuariohttp = server.arg("u");

    if (vSenha == Senha)
    {
      if (vreq == "" || vreq == " ")
      {
        vreq = "keepin"; // se não houver registro, vai para o padrão
      }

      if (vUsuariohttp == "" || vUsuariohttp == " ")
      {
        vUsuariohttp = "keepin"; // se não houver usuario vai para o padrao
      }

      if (senha1 == CurrentPass && vUsuarioAntigo == usuario1) {

        seg.gravar(vUsuariohttp, vreq);
        /*

        SPIFFS.begin();

        //senha

        File f = SPIFFS.open("/httppass.txt", "w");

        

        f.println(vreq+"|");

//        f.println("keepin|");

        f.close();



        //Usuario

        f = SPIFFS.open("/httpuser.txt", "w");

        

//        f.println("keepin|");

        f.println(vUsuariohttp+"|");

        f.close();  



        Serial.println("req: " + vreq);

        Serial.println("usuario: " + vUsuariohttp);

        SPIFFS.end();    

        */
# 553 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\mesh.ino"
        Serial.println("Reiniciando sistema depois de alterar http senha");
        server.send(200, "text/html", "ok");
        delay(500);
        ESP.restart();
      }
      else {
        server.send(200, "text/html", "-1");
      }
    }
    else {
      server.send(200, "text/html", "-1");
    }

}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\normalize.ino"
String vNormalize() {
  String html = "html {";
  html += "    font-family: sans-serif;";
  html += "    -ms-text-size-adjust: 100%;";
  html += "    -webkit-text-size-adjust: 100%;";
  html += "}";
  html += "";
  html += "body {";
  html += "    margin: 0;";
  html += "}";
  html += "";
  html += "article,";
  html += "aside,";
  html += "details,";
  html += "figcaption,";
  html += "figure,";
  html += "footer,";
  html += "header,";
  html += "hgroup,";
  html += "main,";
  html += "menu,";
  html += "nav,";
  html += "section,";
  html += "summary {";
  html += "    display: block;";
  html += "}";
  html += "";
  html += "audio,";
  html += "canvas,";
  html += "progress,";
  html += "video {";
  html += "    display: inline-block;";
  html += "    vertical-align: baseline;";
  html += "}";
  html += "";
  html += "audio:not([controls]) {";
  html += "    display: none;";
  html += "    height: 0;";
  html += "}";
  html += "";
  html += "[hidden],";
  html += "template {";
  html += "    display: none;";
  html += "}";
  html += "";
  html += "a {";
  html += "    background-color: transparent;";
  html += "}";
  html += "";
  html += "a:active,";
  html += "a:hover {";
  html += "    outline: 0;";
  html += "}";
  html += "";
  html += "abbr[title] {";
  html += "    border-bottom: 1px dotted;";
  html += "}";
  html += "";
  html += "b,";
  html += "strong {";
  html += "    font-weight: bold;";
  html += "}";
  html += "";
  html += "dfn {";
  html += "    font-style: italic;";
  html += "}";
  html += "";
  html += "h1 {";
  html += "    font-size: 2em;";
  html += "    margin: 0.67em 0;";
  html += "}";
  html += "";
  html += "mark {";
  html += "    background: #ff0;";
  html += "    color: #000;";
  html += "}";
  html += "";
  html += "small {";
  html += "    font-size: 80%;";
  html += "}";
  html += "";
  html += "sub,";
  html += "sup {";
  html += "    font-size: 75%;";
  html += "    line-height: 0;";
  html += "    position: relative;";
  html += "    vertical-align: baseline;";
  html += "}";
  html += "";
  html += "sup {";
  html += "    top: -0.5em;";
  html += "}";
  html += "";
  html += "sub {";
  html += "    bottom: -0.25em;";
  html += "}";
  html += "";
  html += "img {";
  html += "    border: 0;";
  html += "}";
  html += "";
  html += "svg:not(:root) {";
  html += "    overflow: hidden;";
  html += "}";
  html += "";
  html += "figure {";
  html += "    margin: 1em 40px;";
  html += "}";
  html += "";
  html += "hr {";
  html += "    -moz-box-sizing: content-box;";
  html += "    box-sizing: content-box;";
  html += "    height: 0;";
  html += "}";
  html += "";
  html += "pre {";
  html += "    overflow: auto;";
  html += "}";
  html += "";
  html += "code,";
  html += "kbd,";
  html += "pre,";
  html += "samp {";
  html += "    font-family: monospace, monospace;";
  html += "    font-size: 1em;";
  html += "}";
  html += "";
  html += "button,";
  html += "input,";
  html += "optgroup,";
  html += "select,";
  html += "textarea {";
  html += "    color: inherit;";
  html += "    font: inherit;";
  html += "    margin: 0;";
  html += "}";
  html += "";
  html += "button {";
  html += "    overflow: visible;";
  html += "}";
  html += "";
  html += "button,";
  html += "select {";
  html += "    text-transform: none;";
  html += "}";
  html += "";
  html += "button,";
  html += "html input[type=\"button\"],";
  html += "input[type=\"reset\"],";
  html += "input[type=\"submit\"] {";
  html += "    -webkit-appearance: button;";
  html += "    cursor: pointer;";
  html += "}";
  html += "";
  html += "button[disabled],";
  html += "html input[disabled] {";
  html += "    cursor: default;";
  html += "}";
  html += "";
  html += "button::-moz-focus-inner,";
  html += "input::-moz-focus-inner {";
  html += "    border: 0;";
  html += "    padding: 0;";
  html += "}";
  html += "";
  html += "input {";
  html += "    line-height: normal;";
  html += "}";
  html += "";
  html += "input[type=\"checkbox\"],";
  html += "input[type=\"radio\"] {";
  html += "    box-sizing: border-box;";
  html += "    padding: 0;";
  html += "}";
  html += "";
  html += "input[type=\"number\"]::-webkit-inner-spin-button,";
  html += "input[type=\"number\"]::-webkit-outer-spin-button {";
  html += "    height: auto;";
  html += "}";
  html += "";
  html += "input[type=\"search\"] {";
  html += "    -webkit-appearance: textfield;";
  html += "    -moz-box-sizing: content-box;";
  html += "    -webkit-box-sizing: content-box;";
  html += "    box-sizing: content-box;";
  html += "}";
  html += "";
  html += "input[type=\"search\"]::-webkit-search-cancel-button,";
  html += "input[type=\"search\"]::-webkit-search-decoration {";
  html += "    -webkit-appearance: none;";
  html += "}";
  html += "";
  html += "fieldset {";
  html += "    border: 1px solid #c0c0c0;";
  html += "    margin: 0 2px;";
  html += "    padding: 0.35em 0.625em 0.75em;";
  html += "}";
  html += "";
  html += "legend {";
  html += "    border: 0;";
  html += "    padding: 0;";
  html += "}";
  html += "";
  html += "textarea {";
  html += "    overflow: auto;";
  html += "}";
  html += "";
  html += "optgroup {";
  html += "    font-weight: bold;";
  html += "}";
  html += "";
  html += "table {";
  html += "    border-collapse: collapse;";
  html += "    border-spacing: 0;";
  html += "}";
  html += "";
  html += "td,";
  html += "th {";
  html += "    padding: 0;";
  html += "}";

  return html;
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\rf.ino"
void configRF()
{
  mySwitch.enableReceive(rxRF);
  sSendRF.enableTransmit(txRF);
  sSendRF.setRepeatTransmit(5);
}

void getRF()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", String(tamanhoRF) + "|" + codigoRF + "|" + String(gProtocoloRF) + "*");
  tamanhoRF = -1;
  gProtocoloRF = -1;
  codigoRF = "-1";

}

void habRF()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //enReadRF = true;
  server.send(200, "text/html", "ok");
}

void gravarf()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  SensorRFAlterado = true;
  //String idAgenda = server.arg("ag");
  String Valor = server.arg("s");
  String Senha = server.arg("k");

  Serial.println(Valor);
  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/rf.txt", "w");

    if (!f)
    {
      SPIFFS.format();
      File f = SPIFFS.open("/rf.txt", "w");
    }

    f.println(Valor);
    f.close();
    SPIFFS.end();
  }
  consultaSensorRF();
  server.send(200, "text/html", "ok");
}

void trataRF()
{
  static unsigned long limparUltimoDisparoRF = 0;
  unsigned long currenTime = millisAtual;
  //Contador += 1;
  if (currenTime - starTime >= interval)
  {
    limparUltimoDisparoRF++;

    if (limparUltimoDisparoRF > 1200)
    {
      limparUltimoDisparoRF = 0;
      ultimoDisparoRF = "";
    }
  }

  if (verificaSensoresRF())
  {
    if (msgDisparadaRF[numSensorMSG] == false)
    {
      //Serial.print("Notificar? ");
      //Serial.println(notificar);
      if (notificar)
      {
        sendDataToFirebase("Sensor " + String(numSensorMSG + 1) + " disparado", numSensorMSG, "2");
      }
      else if (enviarsms)
      {
        sendSMS(numSensorMSG);
      }
      ultimoDisparo = numSensorMSG;
      limparUltimoDisparoRF = 0;
    }
  }
}

boolean verificaSensoresRF()
{
  if (SensorRFAlterado)
  {
    Serial.println(" sensor alterado ");
    SensorRFAlterado = false;
    consultaSensorRF();
  }
  sensorRFDisparado = false;

  String Funcao;
  String PortaAparelho;
  String Msg;
  String vChip;
  String vIPDest;
  String vCodigo;
  String vTamanho;
  int i3;
  String texto = "";
  boolean valorRetorno = false;

  for (int nsensor = 0; nsensor < 30; nsensor++)
  {
    texto = SensoresRF[nsensor];
    texto.trim();
    //separa dados nas variaveis
    Funcao = "";
    PortaAparelho = "";
    Msg = "";
    vChip = "";
    vIPDest = "";
    vCodigo = "";
    vTamanho = "";
    i3 = 1;
    for (int i2 = 0; i2 <= 60; i2++)
    {
      if (texto[i2] != '|' && i3 <= 7 && texto != "")
      {
        if (i3 == 1)
        {
          Funcao += texto[i2];
        }

        if (i3 == 2)
        {
          PortaAparelho += texto[i2];
        }

        if (i3 == 3)
        {
          Msg += texto[i2];
        }

        if (i3 == 4) //ip
        {
          vIPDest += texto[i2];
        }

        if (i3 == 5) // ChipID
        {
          vChip += texto[i2];
        }

        if (i3 == 6) // Codigo
        {
          vCodigo += texto[i2];
        }

        if (i3 == 7) // Tamanho
        {
          vTamanho += texto[i2];
        }
      }
      else
      {
        i3++;
      }

    } // fim separar dados
    vCodigo.trim();
    if (vCodigo == codigoRF)
    {
      sensorRFDisparado = true;
      numSensorDisparado = nsensor;
    }
  }

  if (sensorRFDisparado)
  {
    // pega somente o disparo
    numSensorMSG = numSensorDisparado;

    texto = SensoresRF[numSensorDisparado];
    texto.trim();
    Funcao = "";
    PortaAparelho = "";
    Msg = "";
    vChip = "";
    vIPDest = "";
    vCodigo = "";
    vTamanho = "";
    i3 = 1;

    //separa dados nas variaveis
    for (int i2 = 0; i2 <= 60; i2++)
    {
      if (texto[i2] != '|' && i3 <= 7 && texto != "")
      {
        if (i3 == 1)
        {
          Funcao += texto[i2];
        }

        if (i3 == 2)
        {
          PortaAparelho += texto[i2];
        }

        if (i3 == 3)
        {
          Msg += texto[i2];
        }

        if (i3 == 4) //ip
        {
          vIPDest += texto[i2];
        }

        if (i3 == 5) // ChipID
        {
          vChip += texto[i2];
        }

        if (i3 == 6) // Codigo
        {
          vCodigo += texto[i2];
        }

        if (i3 == 7) // Tamanho
        {
          vTamanho += texto[i2];
        }
      }
      else
      {
        i3++;
      }

    } // fim separar dados
  }

  if (sensorRFDisparado) // se houve correspondencia
  {
    sensorRFDisparado = false;
    enviarsms = false;
    // iniciar tratamento.
    String ValorAtuacaoRF;
    String vsAtualRF = "1";
    if (Funcao == "0" || Funcao == "2" || Funcao == "3" || Funcao == "7")
    {
      ValorAtuacaoRF = "1";
    }
    else if (Funcao == "1")
    {
      ValorAtuacaoRF = "0";
    }
    else if (Funcao == "4")
    {
      enviarsms = true;
      ValorAtuacaoRF = "1";
      msgDisparadaRF[numSensorDisparado] = false;
    }
    else if (Funcao == "5")
    {
      ValorAtuacaoRF = "1";
    }
    else if (Funcao == "6")
    {
      ValorAtuacaoRF = vsAtualRF;
    }
    else if (Funcao == "8")
    {
      ValorAtuacaoRF = "1";
    }
    else
    {
      ValorAtuacaoRF = "1";
    }

    //varia com o status atual da prota
    if (vsAtualRF == "0")
    {
      estadoAtualRF[numSensorDisparado] = 0x0;
    }
    else
    {
      estadoAtualRF[numSensorDisparado] = 0x1;
    }

    ////
    //    Serial.print("Valor Atuacao: ");
    //  Serial.println(ValorAtuacaoRF);
    //    Serial.print("Valor vsAtualRF: ");
    //Serial.println(vsAtualRF);
    //Serial.print("Valor estadoAtualRF: ");
    //Serial.println(estadoAtualRF[nsensor]);
    //Serial.print("Valor ultimoestadorf: ");
    //Serial.println(ultimoEstadoRF[nsensor]);

    ///  Reseta os marcadores de acionamento

    if (estadoAtualRF[numSensorDisparado] == ultimoEstadoRF[numSensorDisparado] && ValorAtuacaoRF != vsAtualRF)
    {
      if (Funcao == "7")
      {
        ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
      }
    }

    if (ValorAtuacaoRF == vsAtualRF && estadoAtualRF[numSensorDisparado] != ultimoEstadoRF[numSensorDisparado])
    {
      if (Funcao == "0" || Funcao == "1" || Funcao == "4")
      {
        //Serial.println("disparou");
        ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
        msgDisparadaRF[numSensorDisparado] = false;
        valorRetorno = true;
      }
      else if (Funcao == "2") // liga porta
      {
        IPAddress Destino;
        Destino.fromString(vIPDest);

        if (Destino == IpDispositivo)
        {
          LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 0);
        }
        else // envia udp
        {
          char replyPacekt[255] = "";
          String Texto = vIPDest + "|" + PortaAparelho + "|true|" + String(vChip) + "|E|";
          Serial.println(Texto);
          Texto.toCharArray(replyPacekt, 255);
          Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparadaRF[numSensorDisparado] = true;
        }
        if (Msg == "1")
        {
          valorRetorno = true;
          msgDisparadaRF[numSensorDisparado] = false;
        }
        else
        {
          valorRetorno = false;
        }
      }
      else if (Funcao == "3") // desliga porta
      {
        IPAddress Destino;
        Destino.fromString(vIPDest);

        if (Destino == IpDispositivo)
        {
          LigaDesliga(PortaAparelho.toInt() - 1, 0x0, "", 0);
        }
        else
        {
          char replyPacekt[255] = "";
          String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|";
          Texto.toCharArray(replyPacekt, 255);
          Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparadaRF[numSensorDisparado] = true;
        }
        if (Msg == "1")
        {
          valorRetorno = true;
          msgDisparadaRF[numSensorDisparado] = false;
        }
        else
        {
          valorRetorno = false;
        }
      }
      else if (Funcao == "5")
      {

        if (estadoAtualRF[numSensorDisparado] == 0x1 && ultimoEstadoRF[numSensorDisparado] == 0x0)
        {
          //            Inverte(PortaAparelho.toInt()-1);
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
              //  delay(300);
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              Serial.println(Texto);
              Texto.toCharArray(replyPacekt, 255);
              Serial.println("enviado comando UDP");
              Udp.beginPacket(Destino, localUdpPort);
              Udp.write(replyPacekt);
              Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        valorRetorno = false;
      }
      else if (Funcao == "6")
      {
        if (estadoAtualRF[numSensorDisparado] == 0x1 && ultimoEstadoRF[numSensorDisparado] == 0x0)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
              //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              Texto.toCharArray(replyPacekt, 255);
              Serial.println("enviado comando UDP");
              Udp.beginPacket(Destino, localUdpPort);
              Udp.write(replyPacekt);
              Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        else if (estadoAtualRF[numSensorDisparado] == 0x0 && ultimoEstadoRF[numSensorDisparado] == 0x1)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            IPAddress Destino;
            Destino.fromString(vIPDest);

            if (Destino == IpDispositivo)
            {
              lastDebounceTime = millisAtual;
              Inverte(PortaAparelho.toInt() - 1);
              //            LigaDesliga(PortaAparelho.toInt()-1, LOW, "", 0);
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
            else
            {
              lastDebounceTime = millisAtual;
              char replyPacekt[255] = "";
              String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
              Texto.toCharArray(replyPacekt, 255);
              Serial.println("enviado comando UDP");
              Udp.beginPacket(Destino, localUdpPort);
              Udp.write(replyPacekt);
              Udp.endPacket();
              msgDisparadaRF[numSensorDisparado] = true;
              ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
            }
          }
        }
        valorRetorno = false;
      }
      else if (Funcao == "7")
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay * 3)
        {
          lastDebounceTime = millisAtual;
          IPAddress Destino;
          Destino.fromString(vIPDest);

          if (Destino == IpDispositivo && ultimoEstadoRF[numSensorDisparado] != estadoAtualRF[numSensorDisparado])
          {
            LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 1);
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
          else if (Destino != IpDispositivo && ultimoEstadoRF[numSensorDisparado] != estadoAtualRF[numSensorDisparado])
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
            Texto.toCharArray(replyPacekt, 255);
            Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparadaRF[numSensorDisparado] = true;
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
        }
      }
      else if (Funcao == "8")
      {
        if (estadoAtualRF[numSensorDisparado] == 0x1 && ultimoEstadoRF[numSensorDisparado] == 0x0)
        {
          if ((millisAtual - lastDebounceTime) > rfDelay)
          {
            // Cena
            triggerCena(PortaAparelho);
            lastDebounceTime = millisAtual;
            msgDisparadaRF[numSensorDisparado] = true;
            ultimoEstadoRF[numSensorDisparado] = estadoAtual[numSensorDisparado];
          }
        }
        valorRetorno = false;
      }
      else
      {
        if (msgDisparadaRF[numSensorDisparado] == true)
        {
          msgDisparadaRF[numSensorDisparado] = false;
        }
        valorRetorno = false;
      }
    }
    numSensorDisparado = -1;
  }
  ultimoEstadoRF[numSensorDisparado] = estadoAtualRF[numSensorDisparado];
  return valorRetorno;
}

void consultaSensorRF()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/rf.txt", "r");
  String texto = f.readStringUntil('*');
  texto += '*';
  Serial.println(texto);
  f.close();
  SPIFFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoSensor = "";

  if (texto.length() > 4)
  {
    SensoresRF[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoSensor += texto[posicao];
      }
      else if (texto[posicao] == '|' && contador < 7)
      {
        contador++;
        textoSensor += texto[posicao];
      }

      if (contador == 7)
      {
        if (i <= 29)
        {
          SensoresRF[i] = textoSensor;
          textoSensor = "";
          contador = 0;
        }
        i++;
      }
      posicao++;
    }
  }
}

void ultimodisprf()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    server.send(200, "text/html", ultimoDisparoRF);
  }
}

void sendRFp()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  unsigned long Valor = strtoul(server.arg("c").c_str(), __null, 10);
  unsigned long _tamanhorf = strtoul(server.arg("t").c_str(), __null, 10);
  unsigned long _protocol = strtoul(server.arg("p").c_str(), __null, 10);
  String Senha = server.arg("k");

  if (Senha == "kdi9e") {
    Serial.println("Valor: " + String(Valor));
    Serial.println("Tamanho: " + String(_tamanhorf));
    Serial.println("Protocolo: " + String(_protocol));
    sSendRF.setProtocol(_protocol);
    sSendRF.send(Valor, _tamanhorf);
    server.send(200, "text/html", "ok");
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\sensores.ino"
void trataDevice(String (&Devices)[20])
{
  SPIFFS.begin();
  File f = SPIFFS.open("/device.txt", "r");

  String valorDevice = f.readStringUntil('*');
  //Serial.println(valorDevice);
  f.close();
  SPIFFS.end();

  int posicaoDevice = 0;

  for (int i = 0; i < 20; i++)
  {
    Devices[i] = "";
  }

  for (int i = 0; i <= valorDevice.length(); i++)
  {
    if (valorDevice[i] != ';')
    {
      if (valorDevice[i] != '*' && int(valorDevice[i]) != 13 && int(valorDevice[i]) != 0)
      {
        Devices[posicaoDevice] += valorDevice[i];
      }
    }
    else
    {
      if (posicaoDevice >= 20)
      {
        posicaoDevice = 20;
      }
      else
      {
        posicaoDevice += 1;
      }
    }
  }
  Serial.println(valorDevice);
  DeviceAlterado = false;
}

void trataSensores()
{
  static unsigned long limparUltimoDisparo = 0;

  String sSensor1 = String(sensor1.read8(), 2);
  String sSensor2 = String(sensor2.read8(), 2);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  if (DeviceAlterado)
  {
    //trataDevice();
  }

  String sChip1 = String(chip1.read8(), 2);
  String sChip2 = String(chip2.read8(), 2);

  while (sChip1.length() < 8)
  {
    sChip1 = '0' + sChip1;
  }

  while (sChip2.length() < 8)
  {
    sChip2 = '0' + sChip2;
  }

  // separa os valores ligos no array
  String valorSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
  int posicaoSensor;

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    valorSensores[posicaoSensor] = sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor);
  }

  for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
  {
    valorSensores[posicaoSensor + 8] = sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor);
  }
  // fim separação dos valroes lidos

  //sensor1.write(7, LOW);
  //Serial.println(sSensor1);
  //Serial.println(String(sensor1.read(7)));

  unsigned long currenTime = millisAtual;
  //Contador += 1;
  if (currenTime - starTime >= interval)
  {
    limparUltimoDisparo++;

    if (limparUltimoDisparo > 1200)
    {
      limparUltimoDisparo = 0;
      ultimoDisparo = "";
    }
    int32_t rssi;
    rssi = WiFi.RSSI();
    char replyPacekt[255] = "";
    String valSensores = sSensor1 + sSensor2;
    valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + valSensores + "|" + sChip1 + sChip2 + "|" + rssi + "*";
    valSensores.toCharArray(replyPacekt, 255);
    //Serial.println(valSensores);
    IPAddress broadcastIp = IpDispositivo;
    broadcastIp[0] = 255;
    broadcastIp[1] = 255;
    broadcastIp[2] = 255;
    broadcastIp[3] = 255;

    if (IpDispositivo[0] == 192 && IpDispositivo[1] == 168 && IpDispositivo[2] == 4)
    {
      broadcastIp[0] = 192;
      broadcastIp[1] = 168;
      broadcastIp[2] = 4;
      broadcastIp[3] = 255;
    }

    if (subnet[0] == 255 && subnet[1] == 255 && subnet[2] == 255) {
      broadcastIp[0] = IpDispositivo[0];
      broadcastIp[1] = IpDispositivo[1];
      broadcastIp[2] = IpDispositivo[2];
      broadcastIp[3] = 255;
    }

    if (subnet[0] == 255 && subnet[1] == 255 && subnet[2] == 0) {
      broadcastIp[0] = IpDispositivo[0];
      broadcastIp[1] = IpDispositivo[1];
      broadcastIp[2] = 255;
      broadcastIp[3] = 255;
    }

    //Serial.println(Contador);
    //    Contador = 0;
    Udp.beginPacketMulticast(broadcastIp, localUdpPort, IpDispositivo);
    //Udp.beginPacket(broadcastIp, localUdpPort);
    Udp.write(replyPacekt);
    Udp.endPacket();
    starTime = millisAtual;
  }
  else if ((currenTime - starTime) < 0)
  {
    starTime = millisAtual;
  }

  for (int numSensor = 0; numSensor <= 15; numSensor++)
  {

    //  if (SensorAlterado && (sSensor1 != "11111111" || sSensor2 != "11111111"))
    if (verificaSensores(numSensor, valorSensores[numSensor]))
    {
      if (msgDisparada[numSensor] == false)
      {
        if (notificar)
        {
          if (nomeSensores[numSensor + 1] != "")
          {
            sendDataToFirebase("Sensor " + nomeSensores[numSensor + 1] + " disparado", numSensor, "1");
          }
          else
          {
            sendDataToFirebase("Sensor " + String(numSensor + 1) + " disparado", numSensor, "1");
          }
        }
        else if (enviarsms)
        {
          sendSMS(numSensor);
        }
        ultimoDisparo = sSensor1 + sSensor2;
        limparUltimoDisparo = 0;
        Serial.println("Sensor disparado");
      }
    }
  }
}

void sendDataToFirebase(String MSG, int numSen, String vTag)
{
  WiFiClient cliente;
  String Devices[20];
  trataDevice(Devices);
  String serve = "AAAAKmhc-Lk:APA91bFrNKIu9Ufqj3AmtXBPcd-87smD9kO4Z6CNdBOejY9nGFjvMCBCP2-7eTVqXlqmJpL2o4cTXD6eD4gbkvnpihPFxougju-536CCPZZn62DNTGhgtUFHoBPw0yyq4xmbjb7Bm9IF";

  String reg = "";

  for (int i = 0; i <= 9; i++)
  {
    //    reg = "APA91bH1RwO17t1VXFfQ4HkQTsvIncLp-SjhGH3WTDtO3_fTDJiOdk43wjNR2-IaOPcEOJvfm-Gp5iRMdk8c-Sy-GNyspsUxK4JS8ZuPo6Nhe9tR9smyrxvSRWBU216mVgTN6UjzdjEj";
    //    reg = "fshYIY1u_GY:APA91bFXybZ0qcmldA8uIqltpGz8nHImMYV6B2I_PqsCmQiE6WKQFSXJhq0zo3vlIi-h-JZLt5i6HyZbJE9qFGJvA_Qg2ioDdQqfNnIY8Emun_LzEUCO48xXTKs9GcyU9UuaaASkEDOX";
    reg = String(Devices[i]);
    //reg.trim();

    if (reg.length() > 10)
    {
      //Serial.println(reg.substring(0,1));
      //Serial.println(reg.substring(1));
      String data = "";
      if (reg.substring(0, 1) == "2")
      {
        Serial.println("Iphone");
        data = "{";
        data = data + "\"to\": \"" + reg.substring(1) + "\",";
        data = data + "\"notification\": {";
        //data = data + "\"custom_notification\": {";
        data = data + "\"body\": \"" + MSG + "\",";
        data = data + "\"title\" : \"" + MSG + "\",";
        data = data + "\"tag\":\"" + vTag + "\",";
        data = data + "\"badge\":\"0\",";
        data = data + "\"content-available\": \"1\",";
        data = data + "\"sound\":\"default\"";
        data = data + "},";
        data = data + "\"data\": {";
        data = data + "\"app\": \"keepin\",";
        data = data + "\"msg\": \"" + MSG + "\"";
        data = data + "}";
        data = data + " }";
      }
      else
      {
        data = "{";
        data = data + "\"to\": \"" + reg.substring(1) + "\",";
        data = data + "\"notification\": {";
        //data = data + "\"custom_notification\": {";
        data = data + "\"body\": \"" + MSG + "\",";
        data = data + "\"title\" : \"" + MSG + "\",";
        data = data + "\"tag\":\"" + vTag + "\",";
        data = data + "\"badge\":\"0\",";
        data = data + "\"content-available\": \"1\",";
        data = data + "\"sound\":\"default\"";
        data = data + "},";
        data = data + "\"data\": {";
        data = data + "\"app\": \"keepin\",";
        data = data + "\"msg\": \"" + MSG + "\"";
        data = data + "}";
        data = data + " }";
      }
      Serial.println("Send data...");
      //      if (cliente.connect("fcm.googleapis.com", 80)) {
      if (cliente.connect("cloud.fcleal.com.br", 80))
      {
        Serial.println("Connected to the server..");
        //        cliente.println("POST /fcm/send HTTP/1.1");
        cliente.println("POST /api/keepin/fcm HTTP/1.1");
        cliente.println("Authorization: key=" + serve + "");
        cliente.println("Content-Type: application/json");
        //        cliente.println("Host: fcm.googleapis.com");
        cliente.println("Host: cloud.fcleal.com.br");
        cliente.print("Content-Length: ");
        cliente.println(data.length());
        cliente.print("\n");
        cliente.print(data);
      }
      Serial.println("Data sent...Reading response..");
      while (cliente.available())
      {
        char c = cliente.read();
        Serial.print(c);
      }
      Serial.println("Finished!");
      cliente.flush();
      Serial.println(data);
      msgDisparada[numSen] = true;
      msgDisparadaRF[numSen] = true;
    }
  }
  cliente.stop();
}

void lersensores()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String sSensor1 = String(sensor1.read8(), 2);
  String sSensor2 = String(sensor2.read8(), 2);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  server.send(200, "text/html", sSensor1);
}

void gravasensor()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  //String idAgenda = server.arg("ag");
  String Valor = server.arg("s");
  String Senha = server.arg("k");
  String nomeS = server.arg("nome");
  String gv = server.arg("gn");
  int Indice = server.arg("p").toInt();
  String nomesG = "";

  if (gv == "1")
  {
    nomeSensores[Indice] = nomeS;
  }

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/sensores.txt", "w");

    if (!f)
    {
      SPIFFS.format();
      File f = SPIFFS.open("/sensores.txt", "w");
    }

    File nf = SPIFFS.open("/nsensores.txt", "w");

    for (int id = 0; id < 16; id++)
    {
      nomesG += nomeSensores[id] + "|";
    }
    nomesG += "*";

    f.println(Valor);
    nf.println(nomesG);
    f.close();
    nf.close();
    SPIFFS.end();
    //Serial.println("valor salvo na ag"+idAgenda+".txt");
    Serial.println(Valor);
  }
  server.send(200, "text/html", "ok");
  SensorAlterado = true;
  consultaSensor();
}

void gravasensor2(String Valor)
{
  SensorAlterado = true;

  SPIFFS.begin();
  File f = SPIFFS.open("/sensores.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/sensores.txt", "w");
  }

  f.println(Valor);
  f.close();
  SPIFFS.end();
}

boolean verificaSensores(int nsensor, String vsAtual)
{
  if (SensorAlterado)
  {
    SensorAlterado = false;
    consultaSensor();
  }
  //String texto = consultaAgenda(i);
  String texto = Sensores[nsensor];
  texto.trim();

  String Porta = "";
  String Funcao = "";
  String PortaAparelho = "";
  String Msg = "";
  String vChip = "";
  String vIPDest = "";
  String PortaAparelho2 = "";
  String vChip2 = "";
  String vIPDest2 = "";
  String vTipo1 = "";
  String vTipo2 = "";
  int i3 = 1;
  boolean valorRetorno = false;
  enviarsms = false;
  static unsigned long inicioAcionado[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static boolean vDisparadoSensor[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

  //separa dados nas variaveis
  for (int i2 = 0; i2 <= 100; i2++)
  {
    if (texto[i2] != '|' && i3 <= 11 && texto != "")
    {
      if (i3 == 1)
      {
        Porta += texto[i2];
      }

      if (i3 == 2)
      {
        Funcao += texto[i2];
      }

      if (i3 == 3)
      {
        PortaAparelho += texto[i2];
      }

      if (i3 == 4)
      {
        Msg += texto[i2];
      }

      if (i3 == 5) //ip
      {
        vIPDest += texto[i2];
      }

      if (i3 == 6) // ChipID
      {
        vChip += texto[i2];
      }

      if (i3 == 7) // Porta Aparelho 2
      {
        PortaAparelho2 += texto[i2];
      }

      if (i3 == 8) // IP Destino 2
      {
        vIPDest2 += texto[i2];
      }

      if (i3 == 9) // ChipID 2
      {
        vChip2 += texto[i2];
      }

      if (i3 == 10) // vTipo1 - Se é retenção ou pulso
      {
        vTipo1 += texto[i2];
      }

      if (i3 == 11) // vTipo2 - Se é retenção ou pulso
      {
        vTipo2 += texto[i2];
      }
    }
    else
    {
      i3++;
    }

  } // fim separar dados

  // iniciar tratamento.
  String ValorAtuacao;
  if (Funcao == "0" || Funcao == "2" || Funcao == "3" || Funcao == "8")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "1")
  {
    ValorAtuacao = "1";
  }
  else if (Funcao == "4")
  {
    enviarsms = true;
    ValorAtuacao = "0";
  }
  else if (Funcao == "5")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "6" || Funcao == "9" || Funcao == "10")
  {
    ValorAtuacao = vsAtual;
  }
  else if (Funcao == "7")
  {
    ValorAtuacao = "0";
  }

  //varial com o status atual da prota

  if (vsAtual == "0")
  {
    estadoAtual[nsensor] = 0x1;
  }
  else
  {
    estadoAtual[nsensor] = 0x0;
  }

  if (nsensor == 3)
  {
    //Serial.println(Funcao);
    //Serial.print("estadoAtual: ");
    //Serial.println(estadoAtual[nsensor]);
    //       ultimoEstado[nsensor] = estadoAtual[nsensor];
    //Serial.print("ultimoEstado: ");
    //      Serial.println(ultimoEstado[nsensor]);
  }

  ////
  ///  Reseta os marcadores de acionamento
  if (estadoAtual[nsensor] == ultimoEstado[nsensor] && ValorAtuacao != vsAtual)
  {
    if (Funcao == "7")
    {
      ultimoEstado[nsensor] = estadoAtual[nsensor];
    }
  }

  if (Funcao == "8" && ValorAtuacao != vsAtual) // trata dupla funcao
  {
    if (inicioAcionado[nsensor] > 0)
    {
      if (millisAtual < inicioAcionado[nsensor])
      {
        inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
      }

      if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
      {
        Serial.println("maior que 2 segundos"); /// acionamento 2
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest2);

        IPAddress Comparacao(192, 168, 4, 1);
        if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
        {
          lastDebounceTime = millisAtual;
          if (vTipo2 == "2") // cenas
          {
            triggerCena(PortaAparelho2);
          }
          else if (vTipo2 == "1")
          {
            LigaDesliga(PortaAparelho2.toInt() - 1, 0x1, "", 1);
          }
          else
          {
            Inverte(PortaAparelho2.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          //udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo2 == "1")
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
          }
          else
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparada[nsensor] = true;
        }
      }
      else if (vDisparadoSensor[nsensor] == false)
      {
        Serial.println("Menor que 2 segundos");
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest);
        IPAddress Comparacao(192, 168, 4, 1);

        if (Destino == IpDispositivo || IpDispositivo == Comparacao)
        {
          lastDebounceTime = millisAtual;
          if (vTipo1 == "1")
          {
            LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 1);
          }
          else
          {
            Inverte(PortaAparelho.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          //udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo1 == "1")
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
          }
          else
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          Serial.println("enviado comando UDP");
          Udp.beginPacket(Destino, localUdpPort);
          Udp.write(replyPacekt);
          Udp.endPacket();
          msgDisparada[nsensor] = true;
        }
      }
    }
  }
  else if (Funcao == "8" && ValorAtuacao == vsAtual) // caso tenha ficado mais de 2 segundos ligado já executa.
  {
    if (millisAtual < inicioAcionado[nsensor])
    {
      inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
    }

    if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
    {
      Serial.println("maior que 2 segundos");
      inicioAcionado[nsensor] = 0;
      vDisparadoSensor[nsensor] = true;

      IPAddress Destino;
      Destino.fromString(vIPDest2);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
      {
        lastDebounceTime = millisAtual;
        if (vTipo2 == "2") // cenas
        {
          triggerCena(PortaAparelho2);
        }
        else if (vTipo2 == "1")
        {
          LigaDesliga(PortaAparelho2.toInt() - 1, 0x1, "", 1);
        }
        else
        {
          Inverte(PortaAparelho2.toInt() - 1);
        }
        ultimoEstado[nsensor] = estadoAtual[nsensor];
        //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
      }
      else
      {
        //udp
        char replyPacekt[255] = "";
        String Texto = "";
        if (vTipo2 == "1")
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
        }
        else
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
        }
        Texto.toCharArray(replyPacekt, 255);
        Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
    }
  }

  if (Msg == "1")
  {
    valorRetorno = true;
  }

  if (ValorAtuacao == vsAtual && estadoAtual[nsensor] != ultimoEstado[nsensor])
  {
    if (Funcao == "0" || Funcao == "1" || Funcao == "4")
    {
      valorRetorno = true;
    }
    else if (Funcao == "2") // liga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 0);
      }
      else // envia udp
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|true|" + String(vChip) + "|E|0|";
        Serial.println(Texto);
        Texto.toCharArray(replyPacekt, 255);
        Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "3") // desliga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, 0x0, "", 0);
      }
      else
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|0|";
        Texto.toCharArray(replyPacekt, 255);
        Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "5")
    {
      if (estadoAtual[nsensor] == 0x1 && ultimoEstado[nsensor] == 0x0)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            //  delay(300);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      //valorRetorno = false;
    }
    else if (Funcao == "6") // interruptor normal
    {
      if (estadoAtual[nsensor] == 0x1 && ultimoEstado[nsensor] == 0x0)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || Destino == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            //LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      else if (estadoAtual[nsensor] == 0x0 && ultimoEstado[nsensor] == 0x1)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          Inverte(PortaAparelho.toInt() - 1);
          //LigaDesliga(PortaAparelho.toInt()-1, LOW, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "7")
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if ((Destino == IpDispositivo && ultimoEstado[nsensor] != estadoAtual[nsensor]) || (ultimoEstado[nsensor] != estadoAtual[nsensor] && IpDispositivo == Comparacao))
      {
        LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 1);
        ultimoEstado[nsensor] = estadoAtual[nsensor];
      }
      //      else if (Destino == IpDispositivo && ultimoEstado[Porta.toInt()] == estadoAtual[nsensor])
      //      {
      //        if (millisAtual - lastPulso > 10000 || millisAtual - lastPulso < 0)
      //        {
      //          ultimoEstado[Porta.toInt()] = !estadoAtual[nsensor];
      //          lastPulso = millisAtual;
      //        }
      //      }
      else if (Destino != IpDispositivo && ultimoEstado[Porta.toInt()] != estadoAtual[nsensor] && IpDispositivo != Comparacao)
      {
        //udp
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
        Texto.toCharArray(replyPacekt, 255);
        Serial.println("enviado comando UDP");
        Udp.beginPacket(Destino, localUdpPort);
        Udp.write(replyPacekt);
        Udp.endPacket();
        msgDisparada[nsensor] = true;
        ultimoEstado[nsensor] = estadoAtual[nsensor];
      }
    }
    else if (Funcao == "8")
    {
      if (estadoAtual[nsensor] == 0x1 && ultimoEstado[nsensor] == 0x0)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {

          if (inicioAcionado[nsensor] == 0)
          {
            inicioAcionado[nsensor] = millisAtual;
            vDisparadoSensor[nsensor] = false;
          }
        }
      }

      //     valorRetorno = false;
    }
    else if (Funcao == "9") // interruptor invertido
    {
      if (estadoAtual[nsensor] == 0x1 && ultimoEstado[nsensor] == 0x0)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            //Inverte(PortaAparelho.toInt()-1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            LigaDesliga(PortaAparelho.toInt() - 1, 0x0, "", 0);
          }
          else
          {
            //udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            Serial.println("enviado comando UDP");
            Udp.beginPacket(Destino, localUdpPort);
            Udp.write(replyPacekt);
            Udp.endPacket();
            msgDisparada[nsensor] = true;
          }
        }
      }
      else if (estadoAtual[nsensor] == 0x0 && ultimoEstado[nsensor] == 0x1)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          //Inverte(PortaAparelho.toInt()-1);
          LigaDesliga(PortaAparelho.toInt() - 1, 0x1, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "10") // Interruptor Cena
    {
      if (estadoAtual[nsensor] == 0x1 && ultimoEstado[nsensor] == 0x0)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena1
          triggerCena(PortaAparelho);
        }
      }
      else if (estadoAtual[nsensor] == 0x0 && ultimoEstado[nsensor] == 0x1)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena2
          triggerCena(PortaAparelho2);
        }
      }
      valorRetorno = false;
    }
  }
  else
  {
    if (msgDisparada[nsensor] == true)
    {
      msgDisparada[nsensor] = false;
    }
    valorRetorno = false;
  }
  ultimoEstado[nsensor] = estadoAtual[nsensor];
  return valorRetorno;
}

void consultaSensor()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/sensores.txt", "r");
  File nf = SPIFFS.open("/nsensores.txt", "r");
  String texto = f.readStringUntil('*');
  String nomeS = nf.readStringUntil('*');
  texto += '*';
  nomeS += '*';

  f.close();
  nf.close();
  SPIFFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoSensor = "";

  if (texto.length() > 4)
  {
    Sensores[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoSensor += texto[posicao];
      }
      else if (texto[posicao] == '|' && contador < 11)
      {
        contador++;
        textoSensor += texto[posicao];
      }

      if (contador == 11)
      {
        Sensores[i] = textoSensor;
        textoSensor = "";
        contador = 0;
        i++;
      }
      posicao++;
    }
  }

  // nomes dos sensores
  posicao = 0;
  i = 0;
  String textoNome = "";

  if (nomeS.length() > 4)
  {
    nomeSensores[i] = "";
    while (nomeS[posicao] != '*')
    {
      if (nomeS[posicao] != '|')
      {
        textoNome += nomeS[posicao];
      }
      else if (nomeS[posicao] == '|')
      {
        nomeSensores[i] = textoNome;
        textoNome = "";
        i++;
      }
      posicao++;
    }
  }
}

String lerSensor()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/sensores.txt", "r");
  String texto = f.readStringUntil('*');
  f.close();
  SPIFFS.end();

  return texto;
}

void consensor()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    String texto = lerSensor();
    server.send(200, "text/html", texto);
  }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

void gravadevice()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Valor = server.arg("d");
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/device.txt", "w");

    if (!f)
    {
      SPIFFS.format();
      File f = SPIFFS.open("/device.txt", "w");
    }

    f.println(Valor);

    f.close();
    SPIFFS.end();
    DeviceAlterado = true;
    Serial.println("gravado device");
  }

  server.send(200, "text/html", "ok");
}

void buscadevice()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/device.txt", "r");

    String valorDevice = f.readStringUntil('*');

    f.close();
    SPIFFS.end();
    server.send(200, "text/html", valorDevice);
  }
}

/*

void limpadevice() 

{

    SPIFFS.begin();   

    //File f = SPIFFS.open("/device.txt", "w");

    SPIFFS.remove("/device.txt");  

    //if (!f) {

    //  SPIFFS.format();

    //  File f = SPIFFS.open("/device.txt", "w");

    //}



    //f.println("");    



    //f.close();

    SPIFFS.end();



    server.send(200, "text/html", "OK");    

}

*/
# 1138 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\sensores.ino"
void ultimodisp()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    server.send(200, "text/html", ultimoDisparo);
  }
}

void retornaNotificar()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/notific.txt", "r");

  String valor = f.readStringUntil('*');

  if (valor == "true")
    notificar = true;
  else
    notificar = false;
  f.close();
  SPIFFS.end();
}

void buscaNotificar()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    server.send(200, "text/html", String(notificar));
  }
  else
  {
    server.send(200, "text/html", "0");
  }
}

void gravanot()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Valor = server.arg("v");
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/notific.txt", "w");

    if (!f)
    {
      SPIFFS.format();
      File f = SPIFFS.open("/notific.txt", "w");
    }

    f.println(Valor);

    f.close();
    SPIFFS.end();
    Serial.println("gravado notificacao");

    if (Valor == "true")
    {
      notificar = true;
    }
    else
    {
      notificar = false;
    }
  }

  server.send(200, "text/html", "ok");
}

void gravanot2(String Valor)
{

  SPIFFS.begin();
  File f = SPIFFS.open("/notific.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/notific.txt", "w");
  }

  f.println(Valor);

  f.close();
  SPIFFS.end();
  Serial.println("gravado notificacao");

  if (Valor == "true")
  {
    notificar = true;
  }
  else
  {
    notificar = false;
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\skeleton.ino"
String vskeleton()
{
  String html = ".container {";
  html += "    position: relative;";
  html += "    width: 100%;";
  html += "    max-width: 960px;";
  html += "    margin: 0 auto;";
  html += "    padding: 0 20px;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += ".column,";
  html += ".columns {";
  html += "    width: 100%;";
  html += "    float: left;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += "@media (min-width: 400px) {";
  html += "    .container {";
  html += "        width: 85%;";
  html += "        padding: 0;";
  html += "    }";
  html += "}";
  html += "@media (min-width: 550px) {";
  html += "    .container {";
  html += "        width: 80%;";
  html += "    }";
  html += "    .column,";
  html += "    .columns {";
  html += "        margin-left: 4%;";
  html += "    }";
  html += "    .column:first-child,";
  html += "    .columns:first-child {";
  html += "        margin-left: 0;";
  html += "    }";
  html += "    .one.column,";
  html += "    .one.columns {";
  html += "        width: 4.66666666667%;";
  html += "    }";
  html += "    .two.columns {";
  html += "        width: 13.3333333333%;";
  html += "    }";
  html += "    .three.columns {";
  html += "        width: 22%;";
  html += "    }";
  html += "    .four.columns {";
  html += "        width: 30.6666666667%;";
  html += "    }";
  html += "    .five.columns {";
  html += "        width: 39.3333333333%;";
  html += "    }";
  html += "    .six.columns {";
  html += "        width: 48%;";
  html += "    }";
  html += "    .seven.columns {";
  html += "        width: 56.6666666667%;";
  html += "    }";
  html += "    .eight.columns {";
  html += "        width: 65.3333333333%;";
  html += "    }";
  html += "    .nine.columns {";
  html += "        width: 74.0%;";
  html += "    }";
  html += "    .ten.columns {";
  html += "        width: 82.6666666667%;";
  html += "    }";
  html += "    .eleven.columns {";
  html += "        width: 91.3333333333%;";
  html += "    }";
  html += "    .twelve.columns {";
  html += "        width: 100%;";
  html += "        margin-left: 0;";
  html += "    }";
  html += "    .one-third.column {";
  html += "        width: 30.6666666667%;";
  html += "    }";
  html += "    .two-thirds.column {";
  html += "        width: 65.3333333333%;";
  html += "    }";
  html += "    .one-half.column {";
  html += "        width: 48%;";
  html += "    }";
  html += "    .offset-by-one.column,";
  html += "    .offset-by-one.columns {";
  html += "        margin-left: 8.66666666667%;";
  html += "    }";
  html += "    .offset-by-two.column,";
  html += "    .offset-by-two.columns {";
  html += "        margin-left: 17.3333333333%;";
  html += "    }";
  html += "    .offset-by-three.column,";
  html += "    .offset-by-three.columns {";
  html += "        margin-left: 26%;";
  html += "    }";
  html += "    .offset-by-four.column,";
  html += "    .offset-by-four.columns {";
  html += "        margin-left: 34.6666666667%;";
  html += "    }";
  html += "    .offset-by-five.column,";
  html += "    .offset-by-five.columns {";
  return html;
}

String vskeleton2()
{
  String html = "        margin-left: 43.3333333333%;";
  html += "    }";
  html += "    .offset-by-six.column,";
  html += "    .offset-by-six.columns {";
  html += "        margin-left: 52%;";
  html += "    }";
  html += "    .offset-by-seven.column,";
  html += "    .offset-by-seven.columns {";
  html += "        margin-left: 60.6666666667%;";
  html += "    }";
  html += "    .offset-by-eight.column,";
  html += "    .offset-by-eight.columns {";
  html += "        margin-left: 69.3333333333%;";
  html += "    }";
  html += "    .offset-by-nine.column,";
  html += "    .offset-by-nine.columns {";
  html += "        margin-left: 78.0%;";
  html += "    }";
  html += "    .offset-by-ten.column,";
  html += "    .offset-by-ten.columns {";
  html += "        margin-left: 86.6666666667%;";
  html += "    }";
  html += "    .offset-by-eleven.column,";
  html += "    .offset-by-eleven.columns {";
  html += "        margin-left: 95.3333333333%;";
  html += "    }";
  html += "    .offset-by-one-third.column,";
  html += "    .offset-by-one-third.columns {";
  html += "        margin-left: 34.6666666667%;";
  html += "    }";
  html += "    .offset-by-two-thirds.column,";
  html += "    .offset-by-two-thirds.columns {";
  html += "        margin-left: 69.3333333333%;";
  html += "    }";
  html += "    .offset-by-one-half.column,";
  html += "    .offset-by-one-half.columns {";
  html += "        margin-left: 52%;";
  html += "    }";
  html += "}";
  html += "html {";
  html += "    font-size: 62.5%;";
  html += "}";
  html += "body {";
  html += "    font-size: 1.5em;";
  html += "    line-height: 1.6;";
  html += "    font-weight: 400;";
  html += "    font-family: \"Raleway\", \"HelveticaNeue\", \"Helvetica Neue\", Helvetica, Arial, sans-serif;";
  html += "    color: #222;";
  html += "}";
  html += "h1,";
  html += "h2,";
  html += "h3,";
  html += "h4,";
  html += "h5,";
  html += "h6 {";
  html += "    margin-top: 0;";
  html += "    margin-bottom: 2rem;";
  html += "    font-weight: 300;";
  html += "}";
  html += "h1 {";
  html += "    font-size: 4.0rem;";
  html += "    line-height: 1.2;";
  html += "    letter-spacing: -.1rem;";
  html += "}";
  html += "h2 {";
  html += "    font-size: 3.6rem;";
  html += "    line-height: 1.25;";
  html += "    letter-spacing: -.1rem;";
  html += "}";
  html += "h3 {";
  html += "    font-size: 3.0rem;";
  html += "    line-height: 1.3;";
  html += "    letter-spacing: -.1rem;";
  html += "}";
  html += "h4 {";
  html += "    font-size: 2.4rem;";
  html += "    line-height: 1.35;";
  html += "    letter-spacing: -.08rem;";
  html += "}";
  html += "h5 {";
  html += "    font-size: 1.8rem;";
  html += "    line-height: 1.5;";
  html += "    letter-spacing: -.05rem;";
  html += "}";
  html += "h6 {";
  html += "    font-size: 1.5rem;";
  html += "    line-height: 1.6;";
  html += "    letter-spacing: 0;";
  html += "}";
  html += "@media (min-width: 550px) {";
  html += "    h1 {";
  html += "        font-size: 5.0rem;";
  html += "    }";
  html += "    h2 {";
  html += "        font-size: 4.2rem;";
  html += "    }";
  return html;
}
String vskeleton3()
{
  String html = "    h3 {";
  html += "        font-size: 3.6rem;";
  html += "    }";
  html += "    h4 {";
  html += "        font-size: 3.0rem;";
  html += "    }";
  html += "    h5 {";
  html += "        font-size: 2.4rem;";
  html += "    }";
  html += "    h6 {";
  html += "        font-size: 1.5rem;";
  html += "    }";
  html += "}";
  html += "p {";
  html += "    margin-top: 0;";
  html += "}";
  html += "a {";
  html += "    color: #1EAEDB;";
  html += "}";
  html += "a:hover {";
  html += "    color: #0FA0CE;";
  html += "}";
  html += ".button,";
  html += "button,";
  html += "input[type=\"submit\"],";
  html += "input[type=\"reset\"],";
  html += "input[type=\"button\"] {";
  html += "    display: inline-block;";
  html += "    height: 38px;";
  html += "    padding: 0 30px;";
  html += "    color: #555;";
  html += "    text-align: center;";
  html += "    font-size: 11px;";
  html += "    font-weight: 600;";
  html += "    line-height: 38px;";
  html += "    letter-spacing: .1rem;";
  html += "    text-transform: uppercase;";
  html += "    text-decoration: none;";
  html += "    white-space: nowrap;";
  html += "    background-color: transparent;";
  html += "    border-radius: 4px;";
  html += "    border: 1px solid #bbb;";
  html += "    cursor: pointer;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += ".button:hover,";
  html += "button:hover,";
  html += "input[type=\"submit\"]:hover,";
  html += "input[type=\"reset\"]:hover,";
  html += "input[type=\"button\"]:hover,";
  html += ".button:focus,";
  html += "button:focus,";
  html += "input[type=\"submit\"]:focus,";
  html += "input[type=\"reset\"]:focus,";
  html += "input[type=\"button\"]:focus {";
  html += "    color: #333;";
  html += "    border-color: #888;";
  html += "    outline: 0;";
  html += "}";
  html += ".button.button-primary,";
  html += "button.button-primary,";
  html += "input[type=\"submit\"].button-primary,";
  html += "input[type=\"reset\"].button-primary,";
  html += "input[type=\"button\"].button-primary {";
  html += "    color: #FFF;";
  html += "    background-color: #33C3F0;";
  html += "    border-color: #33C3F0;";
  html += "}";
  html += ".button.button-primary:hover,";
  html += "button.button-primary:hover,";
  html += "input[type=\"submit\"].button-primary:hover,";
  html += "input[type=\"reset\"].button-primary:hover,";
  html += "input[type=\"button\"].button-primary:hover,";
  html += ".button.button-primary:focus,";
  html += "button.button-primary:focus,";
  html += "input[type=\"submit\"].button-primary:focus,";
  html += "input[type=\"reset\"].button-primary:focus,";
  html += "input[type=\"button\"].button-primary:focus {";
  html += "    color: #FFF;";
  html += "    background-color: #1EAEDB;";
  html += "    border-color: #1EAEDB;";
  html += "}";
  html += "input[type=\"email\"],";
  html += "input[type=\"number\"],";
  html += "input[type=\"search\"],";
  html += "input[type=\"text\"],";
  html += "input[type=\"tel\"],";
  html += "input[type=\"url\"],";
  html += "input[type=\"password\"],";
  html += "textarea,";
  html += "select {";
  html += "    height: 38px;";
  html += "    padding: 6px 10px;";
  html += "    background-color: #fff;";
  html += "    border: 1px solid #D1D1D1;";
  html += "    border-radius: 4px;";
  return html;
}

String vskeleton4()
{
  String html = "    box-shadow: none;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += "input[type=\"email\"],";
  html += "input[type=\"number\"],";
  html += "input[type=\"search\"],";
  html += "input[type=\"text\"],";
  html += "input[type=\"tel\"],";
  html += "input[type=\"url\"],";
  html += "input[type=\"password\"],";
  html += "textarea {";
  html += "    -webkit-appearance: none;";
  html += "    -moz-appearance: none;";
  html += "    appearance: none;";
  html += "}";
  html += "textarea {";
  html += "    min-height: 65px;";
  html += "    padding-top: 6px;";
  html += "    padding-bottom: 6px;";
  html += "}";
  html += "input[type=\"email\"]:focus,";
  html += "input[type=\"number\"]:focus,";
  html += "input[type=\"search\"]:focus,";
  html += "input[type=\"text\"]:focus,";
  html += "input[type=\"tel\"]:focus,";
  html += "input[type=\"url\"]:focus,";
  html += "input[type=\"password\"]:focus,";
  html += "textarea:focus,";
  html += "select:focus {";
  html += "    border: 1px solid #33C3F0;";
  html += "    outline: 0;";
  html += "}";
  html += "label,";
  html += "legend {";
  html += "    display: block;";
  html += "    margin-bottom: .5rem;";
  html += "    font-weight: 600;";
  html += "}";
  html += "fieldset {";
  html += "    padding: 0;";
  html += "    border-width: 0;";
  html += "}";
  html += "input[type=\"checkbox\"],";
  html += "input[type=\"radio\"] {";
  html += "    display: inline;";
  html += "}";
  html += "label>.label-body {";
  html += "    display: inline-block;";
  html += "    margin-left: .5rem;";
  html += "    font-weight: normal;";
  html += "}";
  html += "ul {";
  html += "    list-style: circle inside;";
  html += "}";
  html += "ol {";
  html += "    list-style: decimal inside;";
  html += "}";
  html += "ol,";
  html += "ul {";
  html += "    padding-left: 0;";
  html += "    margin-top: 0;";
  html += "}";
  html += "ul ul,";
  html += "ul ol,";
  html += "ol ol,";
  html += "ol ul {";
  html += "    margin: 1.5rem 0 1.5rem 3rem;";
  html += "    font-size: 90%;";
  html += "}";
  html += "li {";
  html += "    margin-bottom: 1rem;";
  html += "}";
  html += "code {";
  html += "    padding: .2rem .5rem;";
  html += "    margin: 0 .2rem;";
  html += "    font-size: 90%;";
  html += "    white-space: nowrap;";
  html += "    background: #F1F1F1;";
  html += "    border: 1px solid #E1E1E1;";
  html += "    border-radius: 4px;";
  html += "}";
  html += "pre>code {";
  html += "    display: block;";
  html += "    padding: 1rem 1.5rem;";
  html += "    white-space: pre;";
  html += "}";
  html += "th,";
  html += "td {";
  html += "    padding: 12px 15px;";
  html += "    text-align: left;";
  html += "    border-bottom: 1px solid #E1E1E1;";
  html += "}";
  html += "th:first-child,";
  html += "td:first-child {";
  return html;
}

String vskeleton5()
{
  String html = "    padding-left: 0;";
  html += "}";
  html += "th:last-child,";
  html += "td:last-child {";
  html += "    padding-right: 0;";
  html += "}";
  html += "button,";
  html += ".button {";
  html += "    margin-bottom: 1rem;";
  html += "}";
  html += "input,";
  html += "textarea,";
  html += "select,";
  html += "fieldset {";
  html += "    margin-bottom: 1.5rem;";
  html += "}";
  html += "pre,";
  html += "blockquote,";
  html += "dl,";
  html += "figure,";
  html += "table,";
  html += "p,";
  html += "ul,";
  html += "ol,";
  html += "form {";
  html += "    margin-bottom: 2.5rem;";
  html += "}";
  html += ".u-full-width {";
  html += "    width: 100%;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += ".u-max-full-width {";
  html += "    max-width: 100%;";
  html += "    box-sizing: border-box;";
  html += "}";
  html += ".u-pull-right {";
  html += "    float: right;";
  html += "}";
  html += ".u-pull-left {";
  html += "    float: left;";
  html += "}";
  html += "hr {";
  html += "    margin-top: 3rem;";
  html += "    margin-bottom: 3.5rem;";
  html += "    border-width: 0;";
  html += "    border-top: 1px solid #E1E1E1;";
  html += "}";
  html += ".container:after,";
  html += ".row:after,";
  html += ".u-cf {";
  html += "    content: \"\";";
  html += "    display: table;";
  html += "    clear: both;";
  html += "}";
  html += "@media (min-width: 400px) {}";
  html += "@media (min-width: 550px) {}";
  html += "@media (min-width: 750px) {}";
  html += "@media (min-width: 1000px) {}";
  html += "@media (min-width: 1200px) {}";
  html += ".btnred {";
  html += "    color: white;";
  html += "    background-color: red;";
  html += "    border-color: red;";
  html += "}";
  html += ".btnorange {";
  html += "    color: white;";
  html += "    background-color: orange;";
  html += "    border-color: orange;";
  html += "}";

  return html;
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\sms.ino"
void sendSMS(int numSensor) {
    WiFiClient cliente;
    SPIFFS.begin();
    File fTexto = SPIFFS.open("/sms_text.txt", "r");
    File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

    String Texto = fTexto.readStringUntil('*');
    String Numeros = fNumeros.readStringUntil('*');

    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    if (Numeros.length() > 8 && Texto.length() > 4 )
    {
      String data = "user=fcleal&password=201277&destinatario="+Numeros+"&msg="+Texto;

      Serial.println("envia sms");
      Serial.println(data);

      Serial.println("Send data...");
      if (cliente.connect("www.facilitamovel.com.br", 80)) {
        Serial.println("Connected to the server..");
        cliente.println("POST /api/multipleSend.ft HTTP/1.1");
//        cliente.println("Authorization: key=" + serve + "");
        cliente.println("Content-Type: application/x-www-form-urlencoded");
        cliente.println("Host: www.facilitamovel.com.br");
        cliente.print("Content-Length: ");
        cliente.println(data.length());
        cliente.println();
        cliente.println(data);
      }
      Serial.println("Enviado sms...Aguardando confirmacao..");
      while (cliente.available()) {
        char c = cliente.read();
//        Serial.print(c);
      }
      Serial.println("SMS Enviado!");
      cliente.flush();
      //Serial.println(data);

      msgDisparada[numSensor] = true;
    }
  cliente.stop();
}


void gravasms()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");
  String Texto = server.arg("t");
  String Numeros = server.arg("n");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File fTexto = SPIFFS.open("/sms_text.txt", "w");
    File fNumeros = SPIFFS.open("/sms_numb.txt", "w");

    if (!fTexto) {
      SPIFFS.format();
      File fTexto = SPIFFS.open("/sms_text.txt", "w");
    }

    if (!fNumeros) {
      SPIFFS.format();
      File fNumeros = SPIFFS.open("/sms_numb.txt", "w");
    }

    fTexto.println(Texto);
    fNumeros.println(Numeros);
    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    Serial.println(Numeros);
  }
  server.send(200, "text/html", "ok");
}

void gravasms2(String Texto, String Numeros)
{
  SPIFFS.begin();
  File fTexto = SPIFFS.open("/sms_text.txt", "w");
  File fNumeros = SPIFFS.open("/sms_numb.txt", "w");

  if (!fTexto) {
    SPIFFS.format();
    File fTexto = SPIFFS.open("/sms_text.txt", "w");
  }

  if (!fNumeros) {
    SPIFFS.format();
    File fNumeros = SPIFFS.open("/sms_numb.txt", "w");
  }

  fTexto.println(Texto);
  fNumeros.println(Numeros);
  fTexto.close();
  fNumeros.close();
  SPIFFS.end();
}

void consultasms()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if(!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File fTexto = SPIFFS.open("/sms_text.txt", "r");
    File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

    String Texto = fTexto.readStringUntil('*');
    String Numeros = fNumeros.readStringUntil('*');

    if (Texto == "" && Numeros == "")
    {
      gravasms2("*", "*");
      Serial.println("SMS: " + Texto);
    }

    fTexto.close();
    fNumeros.close();
    SPIFFS.end();

    server.send(200, "text/html", Texto+"|"+Numeros);
  }
}

String consultasms2()
{
  SPIFFS.begin();
  File fTexto = SPIFFS.open("/sms_text.txt", "r");
  File fNumeros = SPIFFS.open("/sms_numb.txt", "r");

  String Texto = fTexto.readStringUntil('*');
  String Numeros = fNumeros.readStringUntil('*');

  fTexto.close();
  fNumeros.close();
  SPIFFS.end();

  return Texto+"|"+Numeros;
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\update.ino"
void executeupdate()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");
//http://keepin.com.br/firmware/16/autoresidencial.ino.bin
  t_httpUpdate_return ret = ESPhttpUpdate.update("http://keepin.com.br/firmware/16/firmware16.bin");
  //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    //            server.send(200, "text/html", "HTTP_UPDATE_FAILD Error: " + String(ESPhttpUpdate.getLastErrorString().c_str()));
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    //    server.send(200, "text/html","HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("ok");
    //    server.send(200, "text/html", "HTTP_UPDATE_OK");
    break;
  }
}

void executeupdateBeta()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");
//http://keepin.com.br/firmware/16/autoresidencial.ino.bin
  t_httpUpdate_return ret = ESPhttpUpdate.update("http://keepin.com.br/firmware/16/beta/firmware16.bin");
  //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    //            server.send(200, "text/html", "HTTP_UPDATE_FAILD Error: " + String(ESPhttpUpdate.getLastErrorString().c_str()));
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    //    server.send(200, "text/html","HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("ok");
    //    server.send(200, "text/html", "HTTP_UPDATE_OK");
    break;
  }
}

void versao()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "2,36");
}

void linkversao()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "http://keepin.com.br/firmware/16/versao.txt");
}

void linkversaoBeta()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "http://keepin.com.br/firmware/16/beta/versao.txt");
}


void logData(String dados)
{
  RtcDateTime HorarioAtual;
  HorarioAtual = carregaHora();

  char time[50];
  sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());

  dados = "Data: " + String(time) + " - " + dados;

  SPIFFS.begin();
  File f = SPIFFS.open("/log.txt", "a");
  f.println(dados);
  f.close();
  SPIFFS.end();
}

void readlog()
{
  SPIFFS.begin();
  String comando = server.arg("c");

  if (comando == "delete") {
    if (SPIFFS.remove("/log.txt")) {
      SPIFFS.end();
      server.send(200, "text/html", "remove");
    }
    else
    {
      SPIFFS.end();
      server.send(200, "text/html", "falha");
    }

  }
  else
  if (comando == "read")
  {
    File f = SPIFFS.open("/log.txt", "r");
    String dados = "<html><body><p><h3><strong>Log</strong></p></h3><br><ul>";
    while (f.available()){
        dados = dados + "<li>" + f.readStringUntil('\n') + "</li>";
    }
    dados = dados + "</ul></body></html>";
    f.close();
    SPIFFS.end();
    server.send(200, "text/html", dados);
  }
  else
  {
    server.send(200, "text/html", "ok");
  }
}
# 1 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\webconfig.ino"
void configuracao() {
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();

//  WiFiClient client = server.client();

/*

*/
# 11 "d:\\Automação\\0-Projetos\\111101 - Keepin - Residencial\\3-Programas\\firmware16\\webconfig.ino"
  SPIFFS.begin();
  //SPIFFS.remove("/index.html");
  File f = SPIFFS.open("/index.html", "r");
  //Serial.println(f.readStringUntil('#fimarq#'));

  if (f.size() < 100)
  {
    SPIFFS.end();
    gravahtml();
  }

  server.streamFile(f, "text/html");
//  server.send(200, "text/html", String(f.size()));

  f.close();
  SPIFFS.end();
}

String htmlconfig1 ()
{
  String buf = "<!DOCTYPE html>";
  buf += "<html lang=\"pt-br\">";
  buf += "<head>";
  buf += "    <meta charset=\"utf-8\">";
  buf += "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
  buf += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  buf += "    <title>KeepIN</title>";
  buf += "    <style type=\"text/css\">";
  return buf;
}

String htmlconfig2()
{
  String buf = "	</style>";
  buf += "    <style type=\"text/css\">";
  return buf;
}

String htmlconfig3()
{

  String buf = "	</style>";
  buf += "</head>";
  buf += "<body>";
  buf += "    <center>";
  buf += "        <h3><strong>Configuração de Rede </strong></h3>";
  buf += "    </center>";
  buf += "    <div class=\"container\">";
  buf += "        <div class=\"row\">";
  buf += "            <div class=\"one column\"></div>";
  buf += "            <div class=\"ten columns\">";
//  buf += "                <h5><strong>Informe as configurações de rede</strong></h5>";
  buf += "                <form action=\"gravarwifi\" method=\"GET\">";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtnomerede\">Nome da rede wifi:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtnomerede\" name=\"txtnomerede\" value=\""+vssid+"\" placeholder=\"Nome da rede (SSID)\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtsenha\">Senha da rede wifi:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtsenha\" name=\"txtsenha\" value=\""+vpass+"\" placeholder=\"Senha da rede wifi\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtip\">Ip:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtip\" name=\"txtip\" value=\""+vip+"\" placeholder=\"Endereço de IP\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtmascara\">Mascara de sub-rede:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtmascara\" name=\"txtmascara\" value=\""+vmask+"\" placeholder=\"Mascara da sub-rede\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <label for=\"txtgateway\">Gateway:</label>";
  buf += "                <input type=\"text\" class=\"u-full-width\" id=\"txtgateway\" name=\"txtgateway\" value=\""+vgateway+"\" placeholder=\"Endereço do Gateway\" />";
  buf += "            </div>";
  buf += "            <div class=\"ten columns\">";
  buf += "                <button type=\"submit\" class=\"button-primary u-full-width\">Gravar</button>";
  buf += "                <a href=\"reiniciar\" class=\"button btnorange u-full-width\">Reiniciar</a>";
  buf += "                <a href=\"reset\" class=\"button btnred u-full-width\">Cfg. Fábrica</a>";
  buf += "                <a href=\"executeupdate?s=kdi9e\" class=\"button btnred u-full-width\">Atualizar Firmware</a>";
  buf += "            </div>";
  buf += "        </div>";
  buf += "        </form>";
  buf += "        <div class=\"one column\"></div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "    </div>";
  buf += "</body>";
  buf += "</html>";

  return buf;
}

void gravahtml()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  //if(!server.authenticate(www_username, www_password))
    //return server.requestAuthentication();

  carregaDadosHTML();

  SPIFFS.begin();
  File f = SPIFFS.open("/index.html", "w");

  if (!f) {
    SPIFFS .format();
    File f = SPIFFS.open("/index.html", "w");
  }


  f.println(htmlconfig1());
  f.println(vNormalize());
  f.println(htmlconfig2());
  f.println(vskeleton());
  f.println(vskeleton2());
  f.println(vskeleton3());
  f.println(vskeleton4());
  f.println(vskeleton5());
  f.println(htmlconfig3());
  //f.println("#fimarq#");
  f.close();
  SPIFFS.end();
}

void testes2()
{
  carregaDadosHTML();
  server.send(200, "text/html", htmlconfig3());
}

void carregaDadosHTML()
{
  SPIFFS.begin();
  File f = SPIFFS.open("/ssid.txt", "r");
  if (!f) {
    vssid = "";
  }
  else
  {
    vssid = f.readStringUntil('|');
  }
  f.close();

  f = SPIFFS.open("/pass.txt", "r");
  if (!f) {
    vpass = "";
  }
  else
  {
    vpass = f.readStringUntil('|');
  }
  f.close();

  f = SPIFFS.open("/ip.txt", "r");
  if (!f) {
    vip = "";
  }
  else
  {
    vip = f.readStringUntil('|');
  }
  vip.replace(",", ".");
  f.close();

  f = SPIFFS.open("/mask.txt", "r");
  if (!f) {
    vmask = "";
  }
  else
  {
    vmask = f.readStringUntil('|');
  }
  vmask.replace(",", ".");
  f.close();

  f = SPIFFS.open("/gateway.txt", "r");
  if (!f) {
    vgateway = "";
  }
  else
  {
    vgateway = f.readStringUntil('|');
  }
  vgateway.replace(",", ".");
  f.close();

  SPIFFS.end();

}
