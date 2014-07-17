#ifndef _GAME // Estas instrucciones son para agrupar (estilo namespace).
#define _GAME

#include "main.h"
#include "Sprites.h"


#define COLORDEPTH 32
#define ANCHO 800
#define ALTO 600
#define CANT_BITMAPS_ENEMIGOS 2
#define POSINICIALTORREX 10
#define W_TORRE 126
#define W_SPAWN 35
#define W_SPAWN2 49
#define PISO 44
#define ACEL_GRAVEDAD 9.8//294 pixeles
#define PPM 9.1
#define PIVOT_X 5
#define PIVOT_Y 31
#define PI 3.14159265 
#define RED makecol(255, 0, 0)
#define GREEN makecol(0, 255, 0)
#define TOTALTIEMPOEXPLOSION 500
#define MAXDISTANCE 800 //altura torre(455) + distancia desde extremo derecho de torre(660) hasta final de pantalla

enum eEstadoJuego
{
	Inicio,
	EnProceso,
	Pausa,
	Fin,
	Exit,
	EnError
};

struct coordenadas
{
	short x;
	short y;
};

typedef struct JUEGO
{
	BITMAP* buffer;
	vector<BITMAP*> single_frame_bitmaps;
	TLISTA multi_frame_bitmaps;
	caracter LaTorre;
	caracter* Torre;
	caracter LaArma;
	caracter* arma;
	bool space_list[8];
	short cant_enemigos;
	short wave;
	long score;
	eEstadoJuego estado;
	TLISTA enemigos;
	TLISTA proyectiles;
	list<caracter*> proyectil_enemigo;
	MIDI* music;
	SAMPLE* gun_shot;
	SAMPLE* explosion;
	SAMPLE* scream;
	SAMPLE* enemy_fire;
	SAMPLE* towerblowup;
	time_t tiempo_enemigos;
	time_t tiempo_transcurido;
	time_t tiempo_explosion;
	bool existe_explosion;
	coordenadas coords_explosion;
	time_t tiempoDisparo;
}TJUEGO;

int InicializarJuego(TJUEGO*);

int ActualizarJuego(TJUEGO*);

int RenderizarJuego(TJUEGO*);

int FinalizarJuego(TJUEGO*);

void InicializarTorre(TJUEGO*);

void InicializarEnemigo(TJUEGO*, int factor);

void AplicarGravedad(caracter* , float , bool);

void borrarsprite(caracter* spr, TJUEGO* Juego);

//void Disparar(*caracter, int, TJUEGO*);

float GetAngleArma(TJUEGO*);

bool inside(int x,int y,int left,int top,int right,int bottom);

void DispararProyectil(TJUEGO* Juego);

void MoverProyectiles(TJUEGO* Juego);

void ResetJuego(TJUEGO*);

BITMAP* grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame);

//void BuscarColisiones(TJUEGO* Juego);

bool BuscarColisionProyectil(TJUEGO* Juego, caracter* proyectil);

void MoverEnemigos(TJUEGO* Juego);

void MoverProyectilesEnemigos(TJUEGO* Juego);

void MegaMove(TJUEGO*, caracter*);

void SpawnMove(TJUEGO*, caracter*);

void Attack(TJUEGO*, caracter*);

bool ExisteColision(caracter* , caracter* , int);

 bool inside(int ,int ,int ,int ,int ,int );

 void DrawExplosion(TJUEGO* Juego, int x, int y);

 void CrearBalaEnemigo(TJUEGO* Juego, int, int, int);

 void BuscarColisionTorre(TJUEGO* Juego);
//void CargarBitmaps(TJUEGO*);

#endif