// CONNECTIONS:
// DS3231 SDA --> SDA
// DS3231 SCL --> SCL
// DS3231 VCC --> 3.3v or 5v
// DS3231 GND --> GND

void agenda()
{

  Serial.println("Consultado agenda...");

  verificaAgenda();
}

void verificaArquivos()
{

  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/agenda.txt", "w");
  }
  f.close();
  SPIFFS.end();
}

void verificaAgenda()
{
  for (int i = 1; i <= 6; i++)
  {
    if (AgendaAlterada)
    {
      AgendaAlterada = false;
      //consultaAgenda(i);
      consultaAgenda2();
    }
    //String texto = consultaAgenda(i);
    String texto = Agendas[i - 1];
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
    for (int i2 = 0; i2 <= qtdeTexto - 1; i2++)
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

    if (i == 3)
    {
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
      if (now.Day() < 10)
      {
        DataAgora += "0" + String(now.Day()) + "/";
      }
      else
      {
        DataAgora += String(now.Day()) + "/";
      }
      if (now.Month() < 10)
      {
        DataAgora += "0" + String(now.Month()) + "/";
      }
      else
      {
        DataAgora += String(now.Month()) + "/";
      }
      DataAgora += String(now.Year());
      String DiaSemanaAgora = String(now.DayOfWeek());
      int EstadoPorta = LOW;
      if (Porta != "")
      {
        EstadoPorta = LePorta(retornaPorta(Porta.toInt()));
      }

      String ValorAgendaDesligada = Data + "|" + HoraI + "|" + HoraF + "|" + Seg + "|" + Ter + "|" + Qua + "|" + Qui + "|" + Sex + "|" + Sab + "|" + Dom + "|" + Tipo + "|" + Porta + "|0|" + ECena + "|" + CenaLiga + "|" + CenaDesliga + "|";
      //Serial.println(HoraAgora);
      if (Tipo == "1") // apenas uma vez
      {
        //Serial.println("Agenda tipo 1 encontrada");
        if ((HoraAgora == HoraI) && (ECena == "1"))
        {
          triggerCena(CenaLiga);
        }
        else
        {
          if ((HoraAgora == HoraI) && (EstadoPorta == LOW))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), HIGH, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1"))
        {
          if (CenaDesliga != "")
          {
            triggerCena(CenaDesliga);
          }
          Agendas[i - 1] = ValorAgendaDesligada;
          Serial.println(Agendas[i - 1]);
          gravaragenda2();
        }
        else
        {
          if ((HoraAgora == HoraF) && (EstadoPorta == HIGH))
          {
            // deslita e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), LOW, "", 0);

            Agendas[i - 1] = ValorAgendaDesligada;
            Serial.println(Agendas[i - 1]);
            gravaragenda2();
          }
        }
      }
      if (Tipo == "2") // todos os dias
      {
        //Serial.println("Agenda tipo 2 encontrada");
        if ((HoraAgora == HoraI) && (ECena == "1"))
        {
          triggerCena(CenaLiga);
        }
        else
        {
          if ((HoraAgora == HoraI) && (EstadoPorta == LOW))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), HIGH, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1"))
        {
          if (CenaDesliga != "")
          {
            triggerCena(CenaDesliga);
          }
        }
        else
        {
          if ((HoraAgora == HoraF) && (EstadoPorta == HIGH))
          {
            // desgita
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), LOW, "", 0);
          }
        }
      }
      if (Tipo == "3") // data específica
      {
        //Serial.println("Agenda tipo 3 encontrada");
        //Serial.println("Data Agora: " + DataAgora);
        //Serial.println("Data: " + Data);
        if ((HoraAgora == HoraI) && (ECena == "1") && (DataAgora == Data))
        {
          triggerCena(CenaLiga);
        }
        else
        {

          if ((HoraAgora == HoraI) && (EstadoPorta == LOW) && (DataAgora == Data))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), HIGH, "", 0);
          }
        }

        if ((HoraAgora == HoraF) && (ECena == "1") && (DataAgora == Data))
        {
          if (CenaDesliga != "")
          {
            triggerCena(CenaDesliga);
          }
          Agendas[i - 1] = ValorAgendaDesligada;
          gravaragenda2();
        }
        else
        {
          if ((HoraAgora == HoraF) && (EstadoPorta == HIGH) && (DataAgora == Data))
          {
            // desliga e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), LOW, "", 0);

            Agendas[i - 1] = ValorAgendaDesligada;
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
        else if ((Ter == "1") && (DiaSemanaAgora == "2"))
        {
          EDiaSemana = true;
        }
        else if ((Qua == "1") && (DiaSemanaAgora == "3"))
        {
          EDiaSemana = true;
        }
        else if ((Qui == "1") && (DiaSemanaAgora == "4"))
        {
          EDiaSemana = true;
        }
        else if ((Sex == "1") && (DiaSemanaAgora == "5"))
        {
          EDiaSemana = true;
        }
        else if ((Sab == "1") && (DiaSemanaAgora == "6"))
        {
          EDiaSemana = true;
        }
        else if ((Dom == "1") && (DiaSemanaAgora == "0"))
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

        if ((HoraAgora == HoraI) && (ECena == "1") && (EDiaSemana == 1))
        {
          triggerCena(CenaLiga);
        }
        else
        {
          if ((HoraAgora == HoraI) && (EstadoPorta == LOW) && (EDiaSemana == 1))
          {
            //digitalWrite(retornaPorta(Porta.toInt()), 1);
            LigaDesliga(retornaPorta(Porta.toInt()), HIGH, "", 0);
          }
        }
        if ((HoraAgora == HoraF) && (ECena == "1") && (EDiaSemana == 1))
        {
          if (CenaDesliga != "")
          {
            triggerCena(CenaDesliga);
          }
        }
        else
        {
          if ((HoraAgora == HoraF) && (EstadoPorta == HIGH) && (EDiaSemana == 1))
          {
            // desliga e desativa agenda
            //digitalWrite(retornaPorta(Porta.toInt()), 0);
            LigaDesliga(retornaPorta(Porta.toInt()), LOW, "", 0);
          }
        }
      }
    }
  }
}

void atualizahora()
{
  //const char* www_username = www_username2.c_str();
  //const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  int vHora = server.arg("h").toInt();
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    server.send(200, "text/html", "ok");
    RtcDateTime HoraNova = vHora;

    Rtc.year = HoraNova.Year();
    Rtc.month = HoraNova.Month();
    Rtc.day = HoraNova.Day();
    Rtc.hour = HoraNova.Hour();
    Rtc.minute = HoraNova.Minute();
    Rtc.second = HoraNova.Second();
    Rtc.set_time();

    //Serial.println("Hora atualizada com sucesso!");
    printDateTime(HoraNova);
    }
  else
  {
    server.send(200, "text/html", "-1");
  }
}

String consultaAgenda(int id)
{
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
  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "r");
  String texto = f.readStringUntil('*');
  f.close();
  SPIFFS.end();

  //Serial.println("consultando agenda " + String(id));
  //Serial.println(texto);

  return texto;
}

void consultaAgenda2()
{
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
      else if (texto[posicao] == '|' && contador < 16)
      {
        contador++;
        textoAgenda += texto[posicao];
      }

      if (contador == 16)
      {
        Agendas[i] = textoAgenda;
        textoAgenda = "";
        contador = 0;
        i++;
      }
      posicao++;
    }
  }
}

void conagenda()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
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

void gravaragenda()
{
  //  const char* www_username = www_username2.c_str();
  //  const char* www_password = www_password2.c_str();
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();

  server.send(200, "text/html", "ok");

  AgendaAlterada = true;
  //String idAgenda = server.arg("ag");
  String Valor = server.arg("v");
  String Senha = server.arg("k");

  if (Senha == "kdi9e")
  {
    SPIFFS.begin();
    File f = SPIFFS.open("/agenda.txt", "w");

    if (!f)
    {
      SPIFFS.format();
      File f = SPIFFS.open("/agenda.txt", "w");
    }

    f.println(Valor);
    f.close();
    SPIFFS.end();
    //Serial.println("valor salvo na ag"+idAgenda+".txt");
    //Serial.println(Valor);
  }
}

void gravaragenda2()
{
  String texto = "";

  texto += Agendas[0] + Agendas[1] + Agendas[2] + Agendas[3] + Agendas[4] + Agendas[5] + '*';
  SPIFFS.begin();
  File f = SPIFFS.open("/agenda.txt", "w");

  if (!f)
  {
    SPIFFS.format();
    File f = SPIFFS.open("/agenda.txt", "w");
  }

  f.println(texto);
  f.close();
  SPIFFS.end();

  //Serial.println("valor salvo agenda");
  //Serial.println(texto);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.print(datestring);
}

String RetornaData(const RtcDateTime &dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  return datestring;
}

void LoopAgenda()
{
  if (HorarioAtual.Minute() != Minuto)
  {
    Serial.println("\nCiclos por segundos: " + String(nCiclos / 60));
    Serial.println("Ciclos por minuto: " + String(nCiclos));
    memorialivre = system_get_free_heap_size();
    Serial.println("memoria livre: " + String(memorialivre));

    nCiclos = 0;
    Minuto = HorarioAtual.Minute();

    char time[50];
    sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());
    Serial.println(time);

    agenda();
  }
}