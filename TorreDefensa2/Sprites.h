#ifndef _SPRITES // Estas instrucciones son para agrupar (estilo namespace).
#define _SPRITES

#include "main.h"

/*
Crear un objeto que almacena varios bitmaps para crear las animaciones. Esto se agrego en un struct por 
asunto de memoria.
*/
struct BitmapVectors
{
	vector<BITMAP*> bv;
};


typedef struct arma
{
	int id;
	int efectoSonido;
	int rangoAtaque;
	int factorAtaque;
	int velocidadAtaqueEscalar;
}tArma;

typedef struct caracter
{
	int id_lista_imagenes;
	bool alive;
	float hp;
	int x;
	int posXInicial;
	int y;
	int width;
	int curframe;
	int height;
	int posYInicial;
	float anguloAtaque;
	float velocidadX;
	float velocidadY;
	float velocidadXInicial;
	float velocidadYInicial;
	float tiempoDisparo;
	float tiempoVivo;
	float tiempoActual;
	float tiempoMov;
	short occupied_space;
	char nombre[50];
	int factorAtaque;
	int factorDefensa;
}caracter;


#endif
