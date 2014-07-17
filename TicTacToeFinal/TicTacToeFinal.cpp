// Tictactoe.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

TGAME* Juego = new TGAME();


// main function
int _tmain(int argc, _TCHAR* argv[])
{  

	PlayerSetup(Juego);

	if(GameInit(Juego) <0)
		return -1;

	while(Juego->EstadoActual != Terminado)
	{
		
		if(GameUpdate(Juego) < 0)
			Juego->EstadoActual = EnError;

		if(GameRender(Juego) < 0)
			Juego->EstadoActual = EnError;
		Sleep(16);
	}

	
	GameDeInit(Juego);


	return 0;
}

