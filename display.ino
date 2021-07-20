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

void Disp_Setup()
{
  Posicao_Bit = 0;
  inicializa_display();
}

//***********************************************************************************
void inicializa_display()
{
  display.init();
  //display.setFont(Cousine_Regular_12);
  //display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.flipScreenVertically();
  //display.drawString(1, 54, "Conec.<" + Conectado_WiFi + ">");
  display.drawString(1, 54, "Con <" + Conectado_WiFi + ">");
  //display.drawString(70, 54, "Sinal");
  display.drawString(60, 54, "Sinal");
  //display.drawProgressBar(95, 57, 30, 6, 0);
  display.drawProgressBar(85, 57, 42, 6, 0);
  display.display();
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
  display.drawString(3, 1, sOut);
  display.drawString(3, 14, sIn);

  display.setFont(ArialMT_Plain_10);
  display.drawLine(0, 28, 127, 28);
  display.drawString(1, 30, "WiFi: OK ");
  display.drawString(50, 30, "Sinal");
  display.drawProgressBar(80, 31, 47, 8, rssi);
  display.drawString(1, 42, "IP: " + IpDispositivo.toString());
  display.drawString(1, 54, "Int:OK MQTT:OK Cloud:OK");
}

// void atualiza_Display_PCF()
// { //************************* Montar os cabeçalhos so monta uma vez em cada atualização
//   // Posicao_Bit = 0;
//   // for (Contador_Posicao = 1; Contador_Posicao <= 8; Contador_Posicao++) //**** Função para posicionar os bytes dos PCFs no display repete 8 vezes 1 para cada PCF
//   // {
//   //   Valor_Bit = bitRead(E_S_Dummy, Posicao_Bit);
//   //   if (Valor_Bit == 1) // só vai mostrar no display as entradas/saidas acionadas 1 a 8 + 1 a 8 = 16
//   //   {
//   //     display.drawString(Coluna, Linha_Inicial, String(Contador_Posicao));
//   //   }
//   //   Coluna = Coluna + 7;
//   //   Posicao_Bit++;
//   // }
//   // Contador_monta_cabecalho = Contador_monta_cabecalho + 1;
//   // if (Contador_monta_cabecalho == 7)
//   // { // Atualizou todos PCFs...ai monta cabeçalho
//   //   //display.drawString(56, 1, ":");
//   //   //display.drawString(56, 13, ":");
//   //   //display.drawString(56, 25, ":");
//   //   //display.drawString(56, 37, ":");
//   //   //display.drawString(116, 1, "E0");
//   //   //display.drawString(116, 13, "S1");
//   //   // display.drawString(0, 25, sin1);
//   //   // display.drawString(0, 37, sout1);
//   //   Contador_monta_cabecalho = 0;
//   // }
//   //display.display();
// }
// void atualiza_Display_Sinal()
// { //*********************** Função Informação de conexão
//   // Inicio_X = 85;
//   // Inicio_Y = 57;
//   // Comprimento = 42;
//   // Altura = 6;
//   //Progresso = 90;
//   //display.drawString(70, 34, String(Contador)); display.display();
//   //Serial.println("Progreso + contador: " + String(Progresso) + ' - ' + String(Contador));
//   // if (Progresso >= 0 && Progresso <= 100 && Contador == 1)
//   // {
//   //   //display.clear(); //Sem este comando nao atualiza barra
//   //   //display.drawString(70, 14, String(Nivel_Sinal_dBm_dbl));
//   //   //display.drawString(70, 34, String(Progresso));
//   //   display.drawString(60, 54, "Sinal");
//   //   display.drawProgressBar(Inicio_X, Inicio_Y, Comprimento, Altura, Progresso);
//   //   display.drawString(1, 54, "Conec.<" + Conectado_WiFi + ">");
//   //   //display.display();
//   //   //     Serial.println("Niveld= " + String(Progresso));
//   //   //delay(250); Contador substitui o Delay()
//   //   Contador = 0;
//   // }
//   // Contador = Contador + 1; // se Progresso estiver fora da faixa, nao faz Contador = 0
//   // if (Contador > 1)
//   // {
//   //   Contador = 0;
//   // }
//   //display.display();
// }

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

void LoopDisplay()
{
  // Nivel_Sinal_dBm_dbl = 70 - analogRead(0);
  // Nivel_Sinal_dBm_dbl = Nivel_Sinal_dBm_dbl / 15;

  // Nivel_Sinal_dBm_dbl = WiFi.RSSI();

  // //Serial.println("Time = " + String(Nivel_Sinal_dBm_dbl) + " Seg." + String(Contador));
  // Conv_dBm_mV();
  if (millisAtual > (lastDisplay + 2000))
  {
    lastDisplay = millisAtual;
    display.clear();
    atualizaDisplay(~sensor1.read8(), ~sensor2.read8(), ~chip1.read8(), ~chip2.read8(), 60);
    display.display();
  }
  // Usar display.clear aqui e display.display embaixo...evita piscar o display...display.display executa os comandos
  //atualiza_Display_Sinal(); //Primeiro essa, pois tem que dar Clear. para atualizar a Barra
  //****************************************************** PCFs da Placa 1
  // Coluna = 0;
  // Linha_Inicial = 1;
  // //E_S_Dummy = PCF_3_P1_E1_8; // PCFs Entradas da Placa 1
  // //E_S_Dummy = ~sensor1.read8(); // PCFs Entradas da Placa 1
  // //atualiza_Display_PCF();

  // Coluna = 59;
  // Linha_Inicial = 1;
  // //E_S_Dummy = PCF_4_P1_E9_16; // PCFs Entradas da Placa 1
  // //E_S_Dummy = ~sensor2.read8();
  // //atualiza_Display_PCF();

  // Coluna = 0;
  // Linha_Inicial = 13;
  // //E_S_Dummy = PCF_1_P1_S1_8; // PCFs Saidas da Placa 1
  // //E_S_Dummy = ~chip1.read8();
  // //atualiza_Display_PCF();

  // Coluna = 59;
  // Linha_Inicial = 13;
  // //E_S_Dummy = PCF_2_P1_S9_16; // PCFs Saidas da Placa 1
  // //E_S_Dummy = ~chip2.read8();
  // //atualiza_Display_PCF();

  // //****************************************************** PCFs da Placa 2
  // Coluna = 0;
  // Linha_Inicial = 25;
  // //E_S_Dummy = 0; //PCF_3_P2_E1_8; // PCFs Entradas da Placa 2
  // //atualiza_Display_PCF();

  // Coluna = 59;
  // Linha_Inicial = 25;
  // //E_S_Dummy = 0; //PCF_4_P2_E9_16; // PCFs Entradas da Placa 2
  // //atualiza_Display_PCF();

  // Coluna = 0;
  // Linha_Inicial = 37;
  // //E_S_Dummy = 0; //PCF_1_P2_S1_8; // PCFs Saidas da Placa 2
  // //atualiza_Display_PCF();

  // Coluna = 59;
  // Linha_Inicial = 37;
  //E_S_Dummy = 0; //PCF_2_P2_S9_16; // PCFs Saidas da Placa 2
  //atualiza_Display_PCF();

  //display.drawLine(0, 28, 127, 28);
  // display.drawLine(0, 17, 100, 17);
  // display.drawLine(0, 18, 100, 18);
  // display.drawLine(0, 19, 100, 19);
  // display.drawLine(0, 20, 100, 20);
  // display.display(); //Aqui limpou o Display e escreveu tambem, por isso nao pisca.
  //delay(100);
  //display.clear();
  //display.display();
  //delay(5000);
}
