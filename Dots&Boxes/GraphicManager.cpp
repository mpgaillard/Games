#include "stdafx.h"
#include "GraphicManager.h"


//**************FUNCIONES*****************
/**
*	Configura e Inicializa el estado del buffer de caracteres y de color para poder manipularlo
**/
bool GraphicManager_Init(TGRAPHICS * lsGraphic)
{
	//Obtener el manipulador de la consola
	lsGraphic->mStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
	if (!lsGraphic->mStdOut)
		return false;	
	/**
	*	Inicializar el arreglo (buffer) de caracteres
	**/
	memset(lsGraphic->macBackBuffer, ' ', ki_CONSOLE_AREA);
	/**
	*	Inicializar el arreglo de color que se utilizará para mostrar los caracteres
	*	ForeColor: Blanco, BackColor: Negro
	**/
	memset(lsGraphic->macColorBuffer, 0x0F, ki_CONSOLE_AREA);
	/**
	*	Inicializar el color actual
	**/
	lsGraphic->mcCurrentColor=0x0F;
	/**
	*	Ocultar el cursor
	*/
    GraphicManager_ShowTheCursor(lsGraphic,false); 
	return true;
}
/**
*	Libera los recursos que se reservan en el Init
**/
bool GraphicManager_Deinit(TGRAPHICS * lsGraphic)
{
	/**
	*	Mostrar el cursor
	*/
    GraphicManager_ShowTheCursor(lsGraphic,true);
	return true;
}

//*************Caracteres******************
/**
*	Función que se encarga de escribir un carácter en el buffer, en una posición específica
*	e indicar con que color se pintará.
*	Parámetros:  
*	  liX,liY Coordenada de la posición donde se escribirá 
*	  lcValue Carácter a escribir
**/
void GraphicManager_WriteChar(TGRAPHICS * lsGraphic, int liX, int liY ,char lcValue)
{
	//x->j,  y->i
	int liIndex=liY*ki_CONSOLE_DIMX+liX;
	lsGraphic->macBackBuffer[liIndex]=lcValue;
	lsGraphic->macColorBuffer[liIndex]=lsGraphic->mcCurrentColor;
}
/**
*	Función que se encarga de escribir una cadena de caracteres en el buffer, 
*	desde una posición específica e indicar con que color se pintarán.
*	Parámetros:  
*	  liX,liY Coordenada de la posición desde donde se escribirá 
*	  lacStr  Cadena de caracteres
**/
void GraphicManager_WriteChars(TGRAPHICS * lsGraphic, int liX, int liY,char * lacString)
{
	int liIndex=liY*ki_CONSOLE_DIMX+liX;
	int liLen=strlen(lacString);
	if ((liIndex+liLen)<=ki_CONSOLE_AREA)
	{
		memcpy(&(lsGraphic->macBackBuffer[liIndex]), lacString, liLen);
		memset(&(lsGraphic->macColorBuffer[liIndex]),lsGraphic->mcCurrentColor,liLen);
	}
}
void GraphicManager_WriteChars(TGRAPHICS * lsGraphic, int liX, int liY,char * lacString, int liLen)
{
	int liIndex=liY*ki_CONSOLE_DIMX+liX;
	if ((liIndex+liLen)<=ki_CONSOLE_AREA)
	{
		memcpy(&(lsGraphic->macBackBuffer[liIndex]), lacString, liLen);
		memset(&(lsGraphic->macColorBuffer[liIndex]),lsGraphic->mcCurrentColor,liLen);
	}
}


//******************Colores**********************
/**
*	Se encarga de asignar el color deseado en la variable que representa el color actual
*	Parámetros:
*		leForeColor y leBackColor  -> indicando el color de la fuente y el background
**/
void GraphicManager_SetColor(TGRAPHICS * lsGraphic, eColor leFore, eColor leBack)
{
   unsigned char lcForeColor = (unsigned char)leFore;
   unsigned char lcBackColor = (unsigned char)leBack;
   lcBackColor <<=4;
   lsGraphic->mcCurrentColor = (lcForeColor | lcBackColor);

   
}

//******************MANIPULACION DEL BUFFER***********
/**
*    Visualizar en pantalla los caracteres que se almacenaron
*    en el arreglo que representa el buffer
*/
void GraphicManager_SwapBuffer(TGRAPHICS * lsGraphic)
{
    //indicar la coordenada por donde se comenzará a pintar
	COORD lCoord={0,0};
    //colocar el cursor en posición inicial (0,0)
	SetConsoleCursorPosition(lsGraphic->mStdOut,lCoord);
    //bloquear el refresh de la ventana
    LockWindowUpdate(GetConsoleWindow());
    
	/**
	*	lcColor almacena el color con el cual se está pintando, es la que nos permite saber
	*	cuando ocurre un cambio de color
	**/
    unsigned char lcColor= lsGraphic->macColorBuffer[0];
    
	//inicializar variables
    unsigned int  luiStart=0;
    unsigned int luiCharCount=0;
	DWORD liCount;
    
    /**
        recorrer los arreglos para ir mostrando en pantalla
        los caracteres en el color correspondiente
    */
    for (unsigned int luiIndex=0; luiIndex<ki_CONSOLE_AREA; luiIndex++)
    {
        /**
		*	Identificar una secuencia de caracteres del mismo color para escribir todo el bloque
		**/
		
		if (lsGraphic->macColorBuffer[luiIndex]!=lcColor)	//si el color cambia, escribo lo anterior
        {
            //establecer el color con el cual se escribira en la consola
			SetConsoleTextAttribute(lsGraphic->mStdOut, (WORD)lcColor);
            
            //escribir en consola el rango de caracter analizados del mismo color
            luiCharCount=luiIndex-luiStart;
            WriteConsole(lsGraphic->mStdOut, &(lsGraphic->macBackBuffer[luiStart]), luiCharCount,&liCount,NULL);
            
            //actualizamos la posicion donde nos quedamos y el color nuevo
            luiStart=luiIndex;
            lcColor= lsGraphic->macColorBuffer[luiIndex];
        }
    }
    /**
	*	Me falta escribir el último bloque de caracteres
	**/
    
    //actualizar el color con el cual se escribirá en pantalla
    SetConsoleTextAttribute(lsGraphic->mStdOut, (WORD)lcColor);
    
    //escribir en consola el rango de caracteres analizados del mismo color
	luiCharCount=ki_CONSOLE_AREA-luiStart;
    WriteConsole(lsGraphic->mStdOut, &(lsGraphic->macBackBuffer[luiStart]), luiCharCount,&liCount,NULL);




    //colocar en posición inicial
	SetConsoleCursorPosition(lsGraphic->mStdOut,lCoord);
    //quitar el bloqueo de refresh de la ventana
    LockWindowUpdate(NULL);
}

/**
*    Limpiar el buffer que representan los arreglos
*/
void GraphicManager_ClearBuffer(TGRAPHICS * lsGraphic)
{
    memset(lsGraphic->macBackBuffer,' ', ki_CONSOLE_AREA);
    memset(lsGraphic->macColorBuffer, lsGraphic->mcCurrentColor, ki_CONSOLE_AREA); 
}
/**
*   Permite deteminar si muestra o no el cursor en la pantalla
*/
void GraphicManager_ShowTheCursor(TGRAPHICS * lsGraphic, bool lbShow)
{
    CONSOLE_CURSOR_INFO lCursor;
    
	if (GetConsoleCursorInfo(lsGraphic->mStdOut, & lCursor))
    {
        lCursor.bVisible=lbShow;
        SetConsoleCursorInfo(lsGraphic->mStdOut, & lCursor);   
    }
}


