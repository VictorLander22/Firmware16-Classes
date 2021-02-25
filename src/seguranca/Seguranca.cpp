// Biblioteca para gestão de senha do http basic authentication
// Esta biblioteca é free

#include "Seguranca.h"
#include <FS.h>

#if ARDUINO >= 100
	#include "Arduino.h"
#else
	extern "C" {
		#include "WConstants.h"
	}
#endif


String UsuarioHTTP = "";
String SenhaHTTP = "";	


// construtor
Seguranca::Seguranca(){
  SPIFFS.begin();
  
  //Senha HTTP
  File f = SPIFFS.open("/httppass.txt", "r");
  
  if (!f)
  {
		SPIFFS.end();
		gravar("keepin", "keepin");
		SPIFFS.begin();
		File f = SPIFFS.open("/httppass.txt", "r");	
  }

  String texto2 = f.readStringUntil('|');
  
  f.close();

  if (texto2 == "" || texto2 == " ") 
  {
    SenhaHTTP = "keepin";
    f = SPIFFS.open("/httppass.txt", "w");
    f.println("keepin|");
    f.close();         
  }
  else{
    SenhaHTTP = texto2;
  }
  

  // Usuario HTTP
  f = SPIFFS.open("/httpuser.txt", "r");
  texto2 = "";
  texto2 = f.readStringUntil('|');
  f.close();

  if (texto2 == "" || texto2 == " ") 
  {  
    UsuarioHTTP = "keepin";
    f = SPIFFS.open("/httpuser.txt", "w");
    f.println("keepin|");
    f.close();         
  }
  else{
    UsuarioHTTP = texto2;
  }

  SPIFFS.end();    	
}

// Inicaliza
void Seguranca::begin() {
}

String Seguranca::retornaUsuario() {
	return UsuarioHTTP;
}

String Seguranca::retornaSenha() {
	return SenhaHTTP;
}

bool Seguranca::gravar(String usuario, String senha)
{
  SPIFFS.begin();
  
  //Senha HTTP
  File f = SPIFFS.open("/httppass.txt", "w");
  if (!f)
  {
	SPIFFS .format();
	File f = SPIFFS.open("/httppass.txt", "w");
  }
  
  f.println(senha + "|");
  f.close();         
  
  SenhaHTTP = senha;
  
  // Usuario HTTP
  f = SPIFFS.open("/httpuser.txt", "w");
  f.println(usuario + "|");
  f.close();         
  
  UsuarioHTTP = usuario;
 
  SPIFFS.end();    
	
}