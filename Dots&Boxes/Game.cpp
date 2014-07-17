
#include "stdafx.h"

short pos_cursor[2] = {30, 6};//pos inicial x = 30, y = 6
TPUNTOS puntos[7][7];

int GameInit(TGAME* Juego)
{	
	if(!Keyboard_Init(&(Juego->Teclado)))
		return -1;
	
	
	if(!GraphicManager_Init(&(Juego->Pantalla)))
		return -2;

	
	Juego->EstadoActual = Iniciando;
	Juego->cursorClock = clock();
	Juego->cursor = char(177);
	Juego->pos_o_linea = false;
	Juego->turno = false;
	Juego->cant_cerradas_1 = Juego->cant_cerradas_2 = 0;
	Juego->GanadorEncontrado = false;
	InitPointsBox(Juego);


	return 0;
}


int GameDeInit(TGAME* Juego)
{
	if(Keyboard_Deinit(&(Juego->Teclado)))
		return -1;
	if(!GraphicManager_Deinit(&(Juego->Pantalla)))
		return -2;

	Juego = NULL;
	return 0;
}

int GameUpdate(TGAME* Juego){
	Juego->GanadorEncontrado = CheckFull();

	Keyboard_Update(&(Juego->Teclado));

	if(Keyboard_Check(&(Juego->Teclado), eKey_Esc)){
		Juego->EstadoActual = Terminado;
		return 0;
	}

	if(Juego->GanadorEncontrado){
		Juego->EstadoActual = Lleno;
		if(Keyboard_Check(&(Juego->Teclado), eKey_Enter)){
			GameInit(Juego);
		}
		return 0;
	}

	if(!(Juego->pos_o_linea)){
		CheckForWin(Juego);
	}

	if(Keyboard_Check(&(Juego->Teclado), eKey_Enter) && Juego->pos_o_linea==false){
		Sleep(200);
		Juego->pos_o_linea = !(Juego->pos_o_linea);
		//GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1], ' ');
		if(pos_cursor[0]<48)
			Juego->eLinea = Right;
		else Juego->eLinea = Left;
	}

	else if(Keyboard_Check(&(Juego->Teclado), eKey_Enter) && Juego->pos_o_linea==true){
		Juego->pos_o_linea = !(Juego->pos_o_linea);
	//	GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1], ' ');
	}


	return 0;
}




void DrawMainframe(TGAME* Juego){
	GraphicManager_SetColor(&(Juego->Pantalla), eBlack, eLightAqua);

	
	
	GraphicManager_WriteChars(&(Juego->Pantalla), 33, 1, "Punto y Caja");

	for(int i=0, posY=6; i<7; i++, posY+=2){
		for(int j=0, posX=30; j<7; j++, posX+=3){
			GraphicManager_WriteChar(&(Juego->Pantalla), posX, posY, char(167));
		}
	}

	if(Juego->EstadoActual == Lleno){
		if(Juego->cant_cerradas_1>Juego->cant_cerradas_2)
			GraphicManager_WriteChars(&(Juego->Pantalla), 5, 21, "Gano el jugador 1, presione Enter si quiere jugar de nuevo");
		else if(Juego->cant_cerradas_1<Juego->cant_cerradas_2){
			GraphicManager_WriteChars(&(Juego->Pantalla), 5, 21, "Gano el jugador 2, presione Enter si quiere jugar de nuevo");
		}
		else{
			GraphicManager_WriteChars(&(Juego->Pantalla), 5, 21, "Hay empate, presione Enter si quieren intentar de nuevo");
		}
	}
	

}



void DrawLines(TGAME* Juego){

	for(int i=0, posY=6; i<7; i++, posY+=2){
		for(int j=0, posX=30; j<7; j++, posX+=3){
			if(puntos[i][j].left) {
				GraphicManager_WriteChar(&(Juego->Pantalla), posX-2, posY, char(196));
				GraphicManager_WriteChar(&(Juego->Pantalla), posX-1, posY, char(196));
			}
			if(puntos[i][j].right){
				GraphicManager_WriteChar(&(Juego->Pantalla), posX+1, posY, char(196));
				GraphicManager_WriteChar(&(Juego->Pantalla), posX+2, posY, char(196));
			}
			if(puntos[i][j].down){
				GraphicManager_WriteChar(&(Juego->Pantalla), posX, posY+1, char(179));
			}
			if(puntos[i][j].up){
				GraphicManager_WriteChar(&(Juego->Pantalla), posX, posY-1, char(179));
			}
		}
	}
}

void DrawWins(TGAME* Juego){
	for(int i=0, posY=7; i<6; i++, posY+= 2){
		for(int j=0, posX= 32; j<6; j++, posX+=3){
			GraphicManager_WriteChar(&(Juego->Pantalla), posX, posY, Juego->win_box[i][j]);
		}
	}
}

int GameRender(TGAME* Juego){
	GraphicManager_ClearBuffer(&(Juego->Pantalla));
	//Primero, dibujamos el marco.
	DrawMainframe(Juego);
	
	//Luego, dibujamos el cursor, las lineas y quien se gano cada celda
	DrawCursor(Juego);
	DrawLines(Juego);
	DrawWins(Juego);
	GraphicManager_SwapBuffer(&(Juego->Pantalla));

	return 0;
}


void DrawCursor(TGAME* Juego){
	double difClock=clock();
	if(Juego->turno == false){
		GraphicManager_SetColor(&(Juego->Pantalla), eRed, eLightAqua);
	}
	else{
		GraphicManager_SetColor(&(Juego->Pantalla), eYellow, eLightAqua);
	}

	if(Juego->pos_o_linea==false){
		ChoosePos(Juego);
		Juego->eLinea = Ninguno;
		GraphicManager_WriteChars(&(Juego->Pantalla), 0, 22, "Presione Enter para elejir el punto.");
	}
	if(Juego->pos_o_linea==false && difClock-(Juego->cursorClock) >500){
		GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1], Juego->cursor);
		Juego->cursorClock = clock();
		if (Juego->cursor==char(177)) Juego->cursor= char(167);
		else Juego->cursor = char(177);
	}
	if(Juego->pos_o_linea==true){
		ChooseLine(Juego);
		GraphicManager_WriteChars(&(Juego->Pantalla), 0, 22, "Usa las flechas para elejir las direcciones, Space para tomar la decision.");
	}
	

	Sleep(100);
}

void InitPointsBox(TGAME* Juego){
	for(int i=0; i<7; i++){
		for(int j=0;j<7; j++){
			puntos[i][j].down = false;
			puntos[i][j].up = false;
			puntos[i][j].left = false;
			puntos[i][j].right = false;
			puntos[i][j].marked = false;
		}
	}

	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			Juego->win_box[i][j] = ' ';
		}
	}
}

void ChoosePos(TGAME* Juego){
	if(Keyboard_Check(&(Juego->Teclado), eKey_Right)){
		pos_cursor[0]+=3;
		if(pos_cursor[0]>48)
			pos_cursor[0]=30;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Left)){
		pos_cursor[0]-=3;
		if(pos_cursor[0]<30)
			pos_cursor[0]=48;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Down)){
		pos_cursor[1]+=2;
		if(pos_cursor[1]>18)
			pos_cursor[1]=6;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Up)){
		pos_cursor[1]-=2;
		if(pos_cursor[1]<6)
			pos_cursor[1]=18;
	}
}

void ChooseLine(TGAME* Juego){
	if(Juego->turno == false){
		GraphicManager_SetColor(&(Juego->Pantalla), eRed, eLightAqua);
	}
	else{
		GraphicManager_SetColor(&(Juego->Pantalla), eYellow, eLightAqua);
	}

	if(Keyboard_Check(&(Juego->Teclado), eKey_Right) && pos_cursor[0] != 48){
		Juego->eLinea = Right;
		
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Left) && pos_cursor[0] != 30){
		Juego->eLinea = Left;

	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Down) && pos_cursor[1] != 18){
		Juego->eLinea = Down;
		
		
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Up) && pos_cursor[1] != 6){
		Juego->eLinea = Up;
		
	}

	//Switch que dibuja la linea
	switch ( Juego->eLinea )
      {
		 
         case Right:
			CleanDot(Juego);
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0]+1, pos_cursor[1], char(196));
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0]+2, pos_cursor[1], char(196));
            break;
         case Left:
         	CleanDot(Juego);
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0]-2, pos_cursor[1], char(196));
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0]-1, pos_cursor[1], char(196));
            break;
		case Down:
			CleanDot(Juego);
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1]+1, char(179));
            break;
		case Up:
			CleanDot(Juego);
			GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1]-1, char(179));
            break;
         default:
            CleanDot(Juego);
      }
	//Switch para elejir permanentemente una posicion
	if(Keyboard_Check(&(Juego->Teclado), eKey_Space)){
		Juego->pos_o_linea = false;
		Juego->turno = !(Juego->turno);
		switch ( Juego->eLinea )
		 {
         case Right:
			 if(puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-27)/3].left==false){
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-30)/3].right=true;
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-27)/3].left=true;
			 }
			 else Juego->turno = !(Juego->turno);
            break;
         case Left:
			if(puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-33)/3].right==false){
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-30)/3].left=true;
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-33)/3].right=true;
			}
			else Juego->turno = !(Juego->turno);
            break;
		case Down:
			if(puntos[(pos_cursor[1]-4)/2][(pos_cursor[0]-30)/3].up==false){
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-30)/3].down=true;
				puntos[(pos_cursor[1]-4)/2][(pos_cursor[0]-30)/3].up=true;
			}
			else Juego->turno = !(Juego->turno);
            break;
		case Up:
			if(puntos[(pos_cursor[1]-8)/2][(pos_cursor[0]-30)/3].down==false){
				puntos[(pos_cursor[1]-6)/2][(pos_cursor[0]-30)/3].up=true;
				puntos[(pos_cursor[1]-8)/2][(pos_cursor[0]-30)/3].down=true;
			}
			else Juego->turno = !(Juego->turno);
            break;
		}
	}
}

//Limpiar las lineas alrededor del punto
void CleanDot(TGAME* Juego){
	GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1]+1, ' ');
	
	GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1]-1, ' ');

	GraphicManager_WriteChars(&(Juego->Pantalla), pos_cursor[0]+1, pos_cursor[1], "  ");
		
	GraphicManager_WriteChars(&(Juego->Pantalla), pos_cursor[0]-2, pos_cursor[1], "  ");
}

void CheckForWin(TGAME* Juego){
	bool seCerro = false;

	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			//Si se forma un cuadrado y lo puntos no han sido marcado, se marcan.
			if(puntos[i][j].right && puntos[i][j+1].down && puntos[i+1][j].right && puntos[i+1][j].up){
				if(!(puntos[i][j].marked && puntos[i][j+1].marked && puntos[i][j].marked && puntos[i][j].marked)){
					puntos[i][j].marked = puntos[i][j+1].marked = puntos[i][j].marked = puntos[i][j].marked = true;
					
					seCerro = true;
					if(!(Juego->turno)) {
						Juego->win_box[i][j] = '2';
						Juego->cant_cerradas_1++;
					}
					else {
						Juego->win_box[i][j] = '1';
						Juego->cant_cerradas_2++;
					}
				}
			}
		}
	}
	if (seCerro) Juego->turno = !(Juego->turno);//Si cerro una caja, el jugador juega de nuevo
}

//Esta funcion verifica si todos los puntos han sido marcados
bool CheckFull(){
	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			if(!(puntos[i][j].right && puntos[i][j+1].down && puntos[i+1][j].right && puntos[i+1][j].up)){
				if(!(puntos[i][j].marked && puntos[i][j+1].marked && puntos[i][j].marked && puntos[i][j].marked)){
					return false;
				}
			}
		}
	}
	return true;
}