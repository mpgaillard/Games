#include "stdafx.h"


enum eEstado
{
	Iniciando = 0,
	EnProceso,
	Lleno,
	Terminado,
	EnError
};

typedef struct GAME
{
	eEstado EstadoActual;
	bool GanadorEncontrado;
	short cant_discos;
	short posicion;
	bool computadora;
	char player1;
	char player2;
	int mejor_jugada;
	int mejor_col[7];
	char matriz[6][7];
	char matriz_IA[6][7];
	char mensaje_final[80];
	bool turno;
	TLISTA Ganadores;
	TKEYBOARD Teclado;
	TGRAPHICS Pantalla;
}TGAME;

void DrawSymbols(TGAME*);

void DrawMainframe(TGAME*);

int GameInit(TGAME*);

int GameDeInit(TGAME*);

int GameUpdate(TGAME*);

int GameRender(TGAME*);

void PlayerSetup(TGAME*);

void simbolo(char &, char &);

bool esLegal(short pos, const char matriz[6][7]);

char hayUnGanador(const char matriz[6][7]);

void llenarEnBlanco(char matriz[6][7]);

bool llenarCol(int pos, TGAME* Juego, char matriz[6][7]);

bool llenarColComp(int pos, TGAME* Juego, char matriz[6][7]);

bool hayGanador(TGAME* Juego, char matriz[6][7], int posX, int posY);

void PlayerSetup(TGAME* Juego);

void CopyMainMatrix(TGAME*);

int FillMatrix(TGAME*);

int JugadaComputadora(TGAME* );