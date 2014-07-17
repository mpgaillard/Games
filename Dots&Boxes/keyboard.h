#ifndef _KEYBOARD
#define _KEYBOARD


/**
*	ESTE ARCHIVO PERMITE GESTIONAR LA ENTRADA DESDE EL TECLADO,
*	CREANDO UN NIVEL DE INDEPENDENCIA DE LA APLICACION
**/


//***************CONSTANTES***************
/**
*	Cantidad m�xima de canales de entrada (teclas) que se podr�n procesar
**/
const int ki_MAX_COUNT_KEY=256;
/**
*	Tama�o m�ximo del Buffer de Eventos de Entrada
**/
const int ki_MAX_INPUT_BUFFER=128;

//*************ENUMERATIVOS***************
/**
*	Tipo enumerativo auxiliar para determinar los c�digos de las
*	posibles teclas que se obtendr�n del buffer de entrada
**/
enum eInputKeys
{
	eKey_Esc  = VK_ESCAPE,
	eKey_Up   = VK_UP,
	eKey_Down = VK_DOWN,
	eKey_Left = VK_LEFT,
	eKey_Right= VK_RIGHT,
	eKey_Space= VK_SPACE,
	eKey_Enter = 0x0D
};

//**************ESTRUCTURAS***************
/**
*	Define los elementos b�sicos para el control del teclado
*	como dispositivo de entrada.
**/
typedef struct KEYBOARD
{
	/**
		Almacena las teclas disponibles a utilizar.  Max: 256
		0-> indica que la tecla no ha sido presionada
		1-> indica que la tecla ha sido presionada
	*/
	bool mabKeys[ki_MAX_COUNT_KEY];

	/**
		Manipulador de la entrada est�ndar, librer�a Win32
	*/
	HANDLE mStdIn;
	/**
		Almacena de modo de entrada inicial, para restaurarlo al final
	*/
	DWORD mOldModeSaved;

}TKEYBOARD;

//**************FUNCIONES*****************
/**
*	Configura e Inicializa el estado del buffer de entrada para poder manipularlo
*	Salva el estado inicial del buffer para restaurarlo antes de salir
**/
bool Keyboard_Init(TKEYBOARD *);
/**
*	Restaura el modo de entrada orignal
**/
bool Keyboard_Deinit(TKEYBOARD *);
/**
*	Actualiza en cada frame las entradas de teclado, a partir del buffer de entrada
**/
void Keyboard_Update(TKEYBOARD *);
/**
*	Dada una entrada (tecla) determina si esta ha sido presionada
**/
bool Keyboard_Check(TKEYBOARD *, int);
/**
*	Analiza la tecla y actualiza el arreglo de teclas de la estructura, 
*	para an�lisis posteriores durante el juego
**/
void Keyboard_ProcessKey(TKEYBOARD *, KEY_EVENT_RECORD);



#endif _KEYBOARD