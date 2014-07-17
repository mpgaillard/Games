#ifndef _GAME // Estas instrucciones son para agrupar (estilo namespace).
#define _GAME

#include "main.h"
#include "Sprites.h"


#define COLORDEPTH 32
#define ANCHO 800
#define ALTO 600
#define ANCHOVIRTUAL 24000
#define ALTOVIRTUAL 1200
#define WHITE makecol(255, 255, 255)
#define TILEW 30
#define TILEH 30
#define ACEL_GRAVEDAD 9.8
#define PPM 60.0
#define CANTNIVELES 2
#define STEP 40
#define TEXTW 200
#define TEXTH 30
#define VIDAINIT 45

enum eEstadoJuego
{
	Inicio,
	Opciones,
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
	BITMAP* bufferVirtual;
	BITMAP* fondo;
	vector<short*> levels;
	vector<BITMAP*> level_bmp_sheet;
	vector<BITMAP*> single_frame_bmp;
	list<BitmapVectors*> caracteres_bmp;
	list<SPRITE*> caracteres; 
	SPRITE* jugador;
	SPRITE el_jugador;
	eEstadoJuego estado;
	vector<MIDI*> music;
	SAMPLE* game_over;
	time_t tiempo_enemigos;
	time_t tiempo_transcurido;
	int tiempo_inicio;
	int cur_level;
	int score;
	int startposjoy1;
	int startposjoy2;
	int zombiePos;
	int x, y;//Posiciones Actuales de Buffer Virtual
	bool existe_explosion;
	coordenadas coords_explosion;
}TJUEGO;

int InicializarJuego(TJUEGO*);

int ActualizarJuego(TJUEGO*);

int RenderizarJuego(TJUEGO*);

int FinalizarJuego(TJUEGO*);

void InicializarJugador(TJUEGO*);

void InicializarZombie(TJUEGO* );

void ActualizarJugador(TJUEGO*);

void ActualizarZombie(TJUEGO* );

void borrarsprite(SPRITE* spr, TJUEGO* Juego);

void AplicarVelocidad(SPRITE* , float ,bool, bool);

void borrarsprite(SPRITE* spr, TJUEGO* Juego);

void DibujarMenu(TJUEGO* Juego);

void CargarNivel(TJUEGO* Juego);

BITMAP* grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame);

void drawframe(BITMAP *source, BITMAP *dest, int x, int y, int width, int height, int startx, int starty, int columns, int frame);

bool BuscarColisionMundo(TJUEGO* Juego, SPRITE* caracter, int x, int y, bool piso, bool techo);

int MapBlockValue(int x, int y, int width, int height, int columns);

void WalkAnim(SPRITE* caracter);

void JumpAnim(SPRITE* caracter);

void RunAnim(SPRITE* caracter);

void SpecialJumpAnim(SPRITE* caracter);

void MenuRenderizado(TJUEGO*);

void MenuEleccion(TJUEGO*);

void GameOverScreen(TJUEGO*);

void NextLevel(TJUEGO*);

void ZombieAnim(SPRITE* caracter);

bool ExisteColision(SPRITE* primero, SPRITE* segundo, int esquina);

bool inside(int x,int y,int left,int top,int right,int bottom);

#endif