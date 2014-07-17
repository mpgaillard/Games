#include "stdafx.h"
#include "keyboard.h"


//**************FUNCIONES*****************
/**
*	Configura e Inicializa el estado del buffer de entrada para poder manipularlo
*	Salva el estado inicial del buffer para restaurarlo antes de salir
**/
bool Keyboard_Init(TKEYBOARD * lsKeyboard)
{
	/**
		Obtener el manipulador de la entrada estándar
	*/
	lsKeyboard->mStdIn=GetStdHandle(STD_INPUT_HANDLE);
	if (lsKeyboard->mStdIn==INVALID_HANDLE_VALUE)
	{
		printf("Error: Obtener Manipulador de Entrada.\n");
		return false;
	}
	/**
		Salvar el modo de entrada incial, haciendo una copia de su valor.
		Este modo será restaurado cuando la ejecución termine.
	*/
	if (!GetConsoleMode(lsKeyboard->mStdIn, &(lsKeyboard->mOldModeSaved) ))
    {
		printf("Error: Obtener Modo de Entrada.\n");
		return false;
	}
	/**
		Hablitar la ventana para capturar los eventos de entrada
	*/
	DWORD lNewMode=ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(lsKeyboard->mStdIn, lNewMode))
	{
		printf("Error: Habilitar la ventana para capturar los eventos de entrada.\n");
		return false;
	}
	/**
		Inicializar el arreglo de teclas a 0 para indicar que ninguna tecla ha sido presionada
	*/
	memset(lsKeyboard->mabKeys, 0, ki_MAX_COUNT_KEY);
	return true;
}
/**
*	Restaura el modo de entrada orignal
**/
bool Keyboard_Deinit(TKEYBOARD * lsKeyboard)
{
	/**
		Restaurar el modo de entrada original
	*/
	if (!SetConsoleMode(lsKeyboard->mStdIn, lsKeyboard->mOldModeSaved))
	{
		printf("Error: Restaurar el mode de entrada.\n");
		return false;
	}
	return true;
}
/**
*	Actualiza en cada frame las entradas de teclado, a partir del buffer de entrada
**/
void Keyboard_Update(TKEYBOARD * lsKeyboard)
{
	/**
		Variable para almacenar el buffer de entrada recibida
	*/
	INPUT_RECORD laInputBuffer[ki_MAX_INPUT_BUFFER];
	/**
		Variable para almacenar la cantidad de eventos ocurridos
	*/
	DWORD liNumEvents=0;
	/**
		Verificamos si han ocurrido eventos		
		liNumEvents: se pasa por referencia pues devuelve la cantidad de eventos ocurridos.
	*/
	if (!PeekConsoleInput(lsKeyboard->mStdIn, laInputBuffer, ki_MAX_INPUT_BUFFER, &liNumEvents))
	{
		printf("Error: Verificar eventos de entrada.\n");
		return;
	}
	/**
		Validar si han ocurrido eventos de teclado
	*/
	if (liNumEvents<=0)
		return;

	/**
		Leer los eventos que se cargaron en el buffer y limpiar el buffer
	*/
	if (!ReadConsoleInput(lsKeyboard->mStdIn, laInputBuffer,ki_MAX_INPUT_BUFFER,&liNumEvents))
	{
		printf("Error: Leer eventos del buffer de entrada.\n");
		return;
	}
	/**
		Procesar las entradas recibidas, para actualizar el arreglo de teclas
	*/
	for (int liIndex=0; liIndex<liNumEvents; liIndex++)
	{
		if (laInputBuffer[liIndex].EventType==KEY_EVENT)// si la entrada se refiere a un evento de teclado
		{
			Keyboard_ProcessKey(lsKeyboard, laInputBuffer[liIndex].Event.KeyEvent);
		}
	}
}
/**
*	Analiza la tecla y actualiza el arreglo de teclas de la estructura, 
*	para análisis posteriores durante el juego
**/
void Keyboard_ProcessKey(TKEYBOARD * lsKeyboard, KEY_EVENT_RECORD lKey)
{
	if (lKey.wVirtualKeyCode>=0 && lKey.wVirtualKeyCode<ki_MAX_COUNT_KEY)
		lsKeyboard->mabKeys[lKey.wVirtualKeyCode]=(lKey.bKeyDown==true);
}
/**
*	Dada una entrada (tecla) determina si esta ha sido presionada
**/
bool Keyboard_Check (TKEYBOARD * lsKeyboard, int liEntry)
{
	return  lsKeyboard->mabKeys[liEntry];
}

