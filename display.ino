// Inicializa o display Oled

//display.init();

//Pinos do NodeMCU
// SDA => D1
// SCL => D2

//display.flipScreenVertically();

// byte PCF_3_P1_E1_8 = 1; //posicao byte X entrada 12345678
// byte PCF_4_P1_E9_16 = 128;
// byte PCF_1_P1_S1_8 = 1;    //170
// byte PCF_2_P1_S9_16 = 255; //85

// byte PCF_3_P2_E1_8 = 1;
// byte PCF_4_P2_E9_16 = 128;
// byte PCF_1_P2_S1_8 = 1;
// byte PCF_2_P2_S9_16 = 255;

byte E_S_Dummy = 0;
byte Posicao_Bit = 0;
bool Valor_Bit = 0;
//byte Bit_LSB = 0;
//byte Bit_MSB = 0;
byte Linha_Inicial = 0;
byte Contador_Posicao = 0;
byte Nivel_Sinal = 0;
//bool Conectado = 0;
byte Contador_monta_cabecalho = 0;
// ****************************************************  Variaveis do display para medicao de sinal
char Nivel_Sinal_dbm;
int Nivel_Sinal_dbl = -70; // = zero de sinal
double Nivel_Sinal_dBm_dbl = -50;
double Progresso;
String _RSSI = "";
byte Contador = 0;
String Conectado_WiFi = "N";
byte Inicio_X, Inicio_Y, Altura, Comprimento; //Display ProgressBar
// ****************************************************  Variaveis do display para medicao de sinal
//byte counter = 0;
int Coluna = 0;
int Linha = 0;

void DisplaySetup()
{
  if (hasDisplay)
  {
    display.init();
    display.flipScreenVertically();
  }
}

void atualizaDisplay(uint8_t in1, uint8_t in2, uint8_t out1, uint8_t out2, int16_t rssi)
{
  String sIn = "";
  String sIn2 = "";
  String sOut = "";
  String sOut2 = "";
  for (int bit = 0; bit <= 7; bit++)
  {
    (bitRead(in1, bit)) ? sIn += String(bit + 1) : sIn += " ";
    (bitRead(in2, bit)) ? sIn2 += String(bit + 1) : sIn2 += " ";
    (bitRead(out1, bit)) ? sOut += String(bit + 1) : sOut += " ";
    (bitRead(out2, bit)) ? sOut2 += String(bit + 1) : sOut2 += " ";
  }
  sIn += ":" + sIn2;
  sOut += ":" + sOut2;

  display.setFont(Cousine_Regular_12);
  display.drawString(3, dispY[0], sOut);
  display.drawString(3, dispY[1], sIn);

  display.setFont(ArialMT_Plain_10);
  display.drawLine(0, 28, 127, 28);
  display.drawString(1, dispY[2], "WiFi: OK ");
  display.drawString(50, dispY[2], "Sinal");
  display.drawProgressBar(80, 31, 47, 8, rssi);
  display.drawString(1, dispY[3], "IP: " + IpDispositivo.toString());
  display.drawString(1, dispY[4], "Int:OK MQTT:OK Cloud:OK");
}

void Conv_dBm_mV()
{ // Converte dBm em milivolts para apresentar linear na barra do display
  //Nivel_Sinal_dBm_dbl = (WiFi.RSSI()); //Habilitar essa linha
  //Nivel_Sinal_dBm_dbl = -40;
  //Serial.println("Nivelb= " + String(Nivel_Sinal_dBm_dbl));
  double expoente = (Nivel_Sinal_dBm_dbl / 10);
  double mV = sqrt(pow(10, expoente) * 600 * 1000); //coloquei 100 no lugar de 1000...1000 tem resposta mV
  double K_corrige_fe_bargraph = 12.9;              //display vai 0 a 100%...100% = -40dB = 7.75mV
  Progresso = mV * K_corrige_fe_bargraph;
  //Serial.println("dBm<" + String(Nivel_Sinal_dBm_dbl) + ">K<" + expoente + ">P<" + String(Progresso) + ">mV<" +  mV + ">");
}

//***********************************************************************************
bool UpdateDisplay(String text)
{
  if (hasDisplay)
  {
    display.clear();
    delay(50);
    for (int i = 2; i < 5; i++)
    {
      dispText[i - 1] = dispText[i];
      display.drawString(1, dispY[i - 1], dispText[i - 1]);
    }
    display.drawString(1, dispY[0], dispText[0]);
    dispText[4] = text;
    display.drawString(1, dispY[4], dispText[4]);
    //}
    display.display();
  }
  return true;
}

void LoopDisplay()
{
  if ((millisAtual > (lastDisplay + 2000)) && hasDisplay)
  {
    lastDisplay = millisAtual;
    display.clear();
    atualizaDisplay(~sensor1.read8(), ~sensor2.read8(), ~chip1.read8(), ~chip2.read8(), 60);
    display.display();
  }
}
