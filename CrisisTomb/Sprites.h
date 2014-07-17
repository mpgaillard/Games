#ifndef _SPRITES // Estas instrucciones son para agrupar (estilo namespace).
#define _SPRITES

#include "main.h"

#define PLAYERW 50
#define PLAYERH 70
#define ZOMBIEW 31
#define ZOMBIEH 68

enum EstadoEnemigo
{
	Ataque,
	Reposo,
	Evasion,
};


/*
Crear un objeto que almacena varios bitmaps para crear las animaciones. Esto se agrego en un struct por 
asunto de memoria.
*/
struct BitmapVectors
{
	vector<BITMAP*> bv;
};


typedef struct SPRITE
{
	int id_lista_imagenes;
	bool alive;
	bool sentido;
	bool jumptype;
	bool ExisteColisionPiso;
	bool ParticleEffect;
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
	float tiempoSalto;
	float tiempoMov;
	float tiempoAnim;
	float tiempoSufrimiento;
	float tiempoParaSaltar;
	short occupied_space;
	char nombre[30];
	int factorAtaque;
	int factorDefensa;
	SAMPLE* jump1;
	SAMPLE* scream;
	EstadoEnemigo estado;
}SPRITE;


#endif
