// Desenvolvido por: Fernando Leal
// fcleal@fcleal.com.br

#ifndef Seguranca_h
#define Seguranca_h

#define SegurancaVersion "1.0.0" 

// Essa biblioteca foi desenvolvida para implementar rotina
// de modificacao de senha padra para o basic authentication
// do HTTP


// includes acesso a arquivos SPIFF
#include <FS.h>


class Seguranca {
public:
	Seguranca();


	// initialise bus
	void begin();
	
	String retornaUsuario();
	String retornaSenha();
	bool gravar(String usuario, String senha);


};
#endif
