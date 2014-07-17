#include "Game.h"

TJUEGO *Juego = new TJUEGO();

int main()
{
	srand(time(NULL));

	InicializarJuego(Juego);
	
	
	while(!key[KEY_ESC] && Juego->estado != Exit){
		
		if(ActualizarJuego(Juego) < 0){
			Juego->estado = EnError;
		}
		if(RenderizarJuego(Juego) < 0){
			Juego->estado = EnError;
		}


		rest(16);
	}

	
	FinalizarJuego(Juego);

	return 0;
}
END_OF_MAIN();
