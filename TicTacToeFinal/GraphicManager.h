#ifndef _GRAPHIC_MANAGER
#define _GRAPHIC_MANAGER

/**
*	ESTE ARCHIVO PERMITE MANIPULAR LOS GRAFICOS (SALIDA|VISUALIZACION),
*	CREANDO UN NIVEL DE INDEPENDENCIA DE LA APLICACION
**/


//***************CONSTANTES***************
/**
*	Dimensiones de la consola disponible para graficar (ancho y alto)
**/
const int ki_CONSOLE_DIMX=80;
const int ki_CONSOLE_DIMY=24;
/**
*	Area de la consola para la graficaci�n
**/
const int ki_CONSOLE_AREA=ki_CONSOLE_DIMX*ki_CONSOLE_DIMY;

//*************ENUMERATIVOS***************
/**
*	Tipo enumerativo auxiliar para determinar los c�digos de los
*	posibles colores que se utilizar�n en la gesti�n de los gr�ficos
**/
enum eColor
{
	eBlack=0,
	eBlue,
	eGreen,
	eAqua,
	eRed,
	ePurple,
	eYellow,
	eWhite,
	eGray,
	eLightBlue,
	eLightGreen,
	eLightAqua,
	eLightRed,
	eLightPurple,
	eLightYellow,
	eLightWhite
};

//**************ESTRUCTURAS***************
/**
*	Define los elementos b�sicos para el control de los gr�ficos
**/

typedef struct sGRAPHICS
{
	/**
	*	Manipulador de la salida a consola
	**/
	HANDLE mStdOut;
	/**
	*	Arreglo que almacena los car�cteres que ser�n visualizados en la pantalla y en que posici�n
	*	Inicialmente se crea el arreglo vac�o (espacios en blanco)
	**/
	unsigned char macBackBuffer  [ki_CONSOLE_AREA];
	/**
	*	Arreglo que almacena el color con el cual se mostrar� cada caracter en la pantalla.
	*	Las posiciones coinciden con el buffer de caracteres.
	**/
	unsigned char macColorBuffer [ki_CONSOLE_AREA];
	/**
	*	Almacena el color actual que se est� actualizando
	**/
	unsigned char mcCurrentColor;

}TGRAPHICS;

//**************FUNCIONES*****************
/**
*	Configura e Inicializa el estado del buffer de caracteres y de color para poder manipularlo
**/
bool GraphicManager_Init(TGRAPHICS *);
/**
*	Libera los recursos que se reservan en el Init
**/
bool GraphicManager_Deinit(TGRAPHICS *);

//*************Caracteres******************
/**
*	Funci�n que se encarga de escribir un car�cter en el buffer, en una posici�n espec�fica
*	Par�metros:  
*	  liX,liY Coordenada de la posici�n donde se escribir� 
*	  lcValue Car�cter a escribir
**/
void GraphicManager_WriteChar(TGRAPHICS *,int,int,char);
/**
*	Funci�n que se encarga de escribir una cadena de caracteres en el buffer, 
*	desde una posici�n espec�fica.
*	Par�metros:  
*	  liX,liY Coordenada de la posici�n desde donde se escribir� 
*	  lacStr  Cadena de caracteres
**/
void GraphicManager_WriteChars(TGRAPHICS *, int,int,char *);
void GraphicManager_WriteChars(TGRAPHICS *, int,int,char *,int);

//******************Colores**********************
/**
*	Se encarga de asignar el color deseado en la variable que representa el color actual
*	Par�metros:
*		leForeColor y leBackColor  -> indicando el color de la fuente y el background
**/
void GraphicManager_SetColor(TGRAPHICS *, eColor, eColor);

//**************************************************************
/**
*    Visualizar en pantalla los caracteres que se almacenaron
*    en el arreglo que representa el buffer
*/    
void GraphicManager_SwapBuffer(TGRAPHICS *);
/**
*    Limpiar el buffer 
*/ 
void GraphicManager_ClearBuffer(TGRAPHICS *);
/**
*   Permite deteminar si muestra o no el cursor
*/
void GraphicManager_ShowTheCursor(TGRAPHICS *,bool);


#endif