#include "stdafx.h"


enum eEstado
{
	Iniciando = 0,
	EnProceso,
	Terminado,
	EnPausa,
	EnError
};

typedef struct GAME
{
	eEstado EstadoActual;
	bool GanadoresEncontrados;
	short cant_jugadores;
	short contador;
	short cant_ganadas1;
	short cant_ganadas2;
	short posicion;
	char player1;
	char player2;
	char cursor;
	bool turno;
	bool computadora;
	TKEYBOARD Teclado;
	TGRAPHICS Pantalla;
	float cursorClock;
	char message[50];
	char player1_score[10];
	char player2_score[10];
}TGAME;

void DrawCursor(TGAME*, vector<char> &);

void DrawSymbols(TGAME*);

void DrawMainframe(TGAME*);

int GameInit(TGAME*);

int GameDeInit(TGAME*);

int GameUpdate(TGAME*);

int GameRender(TGAME*);

void PlayerSetup(TGAME*);

void simbolo(char &, char &);

bool esLegal(short pos, const vector<char>& );

char hayUnGanador(const vector<char>&, TGAME* );

short jugadaComputadora(vector<char> matriz, TGAME* );

void llenarPosicion(vector<char>& matriz, TGAME*, int posicion);

int getVectorPos();

void blank();