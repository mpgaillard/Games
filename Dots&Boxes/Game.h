#include "stdafx.h"


enum eEstado
{
	Iniciando = 0,
	EnProceso,
	Lleno,
	Terminado,
	EnError
};

enum estadoLinea
{
	Ninguno=0,
	Up,
	Down,
	Left,
	Right,
};


typedef struct GAME
{
	eEstado EstadoActual;
	estadoLinea eLinea;
	bool GanadorEncontrado;
	short posicion;
	bool turno;
	char cursor;
	double cursorClock;
	bool pos_o_linea;
	char linea_vertical[2];
	char win_box[6][6];
	short cant_cerradas_1;
	short cant_cerradas_2;
	TKEYBOARD Teclado;
	TGRAPHICS Pantalla;

}TGAME;

typedef struct PUNTOS
{
	bool right;
	bool left;
	bool up;
	bool down;
	bool marked;
}TPUNTOS;



void DrawLines(TGAME*);

void DrawMainframe(TGAME*);

int GameInit(TGAME*);

int GameDeInit(TGAME*);

int GameUpdate(TGAME*);

void DrawCursor(TGAME* Juego);

void DrawWins(TGAME* Juego);

int GameRender(TGAME*);

void CleanDot(TGAME* Juego);

void InitPointsBox(TGAME*);

void ChoosePos(TGAME* Juego);

void ChooseLine(TGAME* Juego);

void CheckForWin(TGAME* Juego);

bool CheckFull();