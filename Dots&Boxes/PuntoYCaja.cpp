// PuntoYCaja.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

TGAME* Juego = new TGAME();

int _tmain(int argc, _TCHAR* argv[])
{
	
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

