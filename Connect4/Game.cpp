
#include "stdafx.h"



int GameInit(TGAME* Juego)
{	
	if(!Keyboard_Init(&(Juego->Teclado)))
		return -1;
	
	

	if(!GraphicManager_Init(&(Juego->Pantalla)))
		return -2;

	srand ( time(NULL) );
	GraphicManager_SetColor(&(Juego->Pantalla), eWhite, eAqua);
	Juego->EstadoActual = Iniciando;
	Juego->turno = false;//false == player1
	Juego->player1 = '1';
	Juego->player2 = '2';
	memset(Juego->mensaje_final, ' ', sizeof(Juego->mensaje_final));
	Juego->cant_discos = 0;
	Juego->GanadorEncontrado = false;
	llenarEnBlanco(Juego->matriz);
	llenarEnBlanco(Juego->matriz_IA);

	return 0;
}


int GameUpdate(TGAME* Juego){

	Keyboard_Update(&(Juego->Teclado));

	if(Keyboard_Check(&(Juego->Teclado), eKey_Esc)){
		Juego->EstadoActual = Terminado;
		return 0;
	}

	if(Juego->GanadorEncontrado){
		if(Juego->turno)
			strcpy(Juego->mensaje_final, "Gano el jugador #1, presione ENTER si quiere jugar de nuevo, sino ESC");
		else strcpy(Juego->mensaje_final, "Gano el jugador #2, presione ENTER si quiere jugar de nuevo, sino ESC");
		if(Keyboard_Check(&(Juego->Teclado), eKey_Enter))
			GameInit(Juego);
		return 0;
	}

	if(Juego->cant_discos >= 42){
		strcpy(Juego->mensaje_final, "Empate, presione ENTER si quiere jugar de nuevo, sino ESC");
		if(Keyboard_Check(&(Juego->Teclado), eKey_Enter))
			GameInit(Juego);
		return 0;
		
	}
	//Si no existe jugador de computador o si existe uno y es el turno del usuario
	if(!(Juego->computadora) || (Juego->computadora && !(Juego->turno))){
		if(Keyboard_Check(&(Juego->Teclado), eKey_1)){
				Juego->GanadorEncontrado = llenarCol(0, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_2)){
				Juego->GanadorEncontrado = llenarCol(1, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_3)){
				Juego->GanadorEncontrado = llenarCol(2, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_4)){
				Juego->GanadorEncontrado = llenarCol(3, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_5)){
				Juego->GanadorEncontrado = llenarCol(4, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_6)){
				Juego->GanadorEncontrado = llenarCol(5, Juego, Juego->matriz);
				Sleep(400);
		}

		else if(Keyboard_Check(&(Juego->Teclado), eKey_7)){
				Juego->GanadorEncontrado = llenarCol(6, Juego, Juego->matriz);
				Sleep(400);
		}
		
	}
	else if(Juego->computadora && Juego->turno){
		Juego->GanadorEncontrado = llenarCol(JugadaComputadora(Juego), Juego, Juego->matriz);

	}

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

void DrawMainframe(TGAME* Juego){
	GraphicManager_SetColor(&(Juego->Pantalla), eWhite, eAqua);
	char lineaActual[COLUMNAS]={0};	


	for(int i=0; i< FILAS; i++){
		if(i==0){
			GraphicManager_WriteChars(&(Juego->Pantalla), 0, i, "Digite un numero entre 1 y 7 que coresponde a la columna deseada");
		}
		else if(i>=8 && i<14){
			GraphicManager_WriteChars(&(Juego->Pantalla), 20, i, "|   |   |   |   |   |   |   |");
		}
		else if(i==14){
			GraphicManager_WriteChars(&(Juego->Pantalla), 20, i, "_____________________________");
		}
		else if(i==15){
			GraphicManager_WriteChars(&(Juego->Pantalla), 20, i, "  1   2   3   4   5   6   7");
		}

		else{
			GraphicManager_WriteChars(&(Juego->Pantalla), 0, i, lineaActual, COLUMNAS);
		}


		memset(lineaActual, ' ', sizeof(lineaActual)); 
	}
	
	GraphicManager_WriteChars(&(Juego->Pantalla), 0, 23, Juego->mensaje_final, sizeof(Juego->mensaje_final));
	
	
}


void DrawSymbols(TGAME* Juego){
	int posXPantalla = 22;//Pos inicial para dibujar los simbolos en X
	int posYPantalla = 8;//Pos inicial para dibujar los simbolos en Y

	for(int i=0; i<6; i++, posYPantalla++){
		for(int j=0, posXPantalla = 22; j<7; j++, posXPantalla+=4){
			if(Juego->matriz[i][j]==Juego->player1) {
				GraphicManager_SetColor(&(Juego->Pantalla), eRed, eLightRed);
			}
			else if(Juego->matriz[i][j]==Juego->player2){
				GraphicManager_SetColor(&(Juego->Pantalla), eLightGreen, eBlue);
			}
			else {
				GraphicManager_SetColor(&(Juego->Pantalla), eWhite, eAqua);
			}

			GraphicManager_WriteChar(&(Juego->Pantalla), posXPantalla, posYPantalla, Juego->matriz[i][j]);
		}
	}
	GraphicManager_SetColor(&(Juego->Pantalla), eWhite, eAqua);
}

int GameRender(TGAME* Juego){
	GraphicManager_ClearBuffer(&(Juego->Pantalla));
	//Primero, dibujamos el marco.
	DrawMainframe(Juego);
	
	//Luego, dibujamos los simbolos
	DrawSymbols(Juego);
	GraphicManager_SwapBuffer(&(Juego->Pantalla));

	return 0;
}

//Llenar la matriz en blanco
void llenarEnBlanco(char matriz[6][7]){
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			matriz[i][j] = ' ';
		}
	}

}

bool llenarCol(int pos, TGAME* Juego, char matriz[6][7]){
	char simbolo;
	//Simbolo coje el caracter del jugador 1 o del jugador 2
	if(!(Juego->turno))simbolo = Juego->player1;
	else simbolo = Juego->player2;

	int i;
	for(i=5; i>=0; i--){
		if(matriz[i][pos] == ' '){
			matriz[i][pos] = simbolo;
			Juego->turno = !(Juego->turno);
				Juego->cant_discos++;
			break;
		}
	}
	return hayGanador(Juego, matriz, pos, i);
}



bool hayGanador(TGAME* Juego, char matriz[6][7], int posX, int posY){
	//Chequea si hay un ganador en cada jugada, mas eficiente que hayGanador

		char simbolo;//Simbolo que se va a imprimir en la pantalla
		if (!(Juego->turno)) simbolo = Juego->player2;//Inverso ya que la funcion anterior roto para hacer el cambio de jugador
		else simbolo = Juego->player1;

		int xRightLim = posX+3;
		int xLeftLim = posX-3;
		int yUpperLim = posY-3;
		int yLowerLim = posY+3;

		int sum1=1;
		//Derecha
			for(int i=posX+1; i<=xRightLim; i++){
				if(matriz[posY][i] != simbolo)
					break;
				
				if(i>6)break;
				
				if(i==xRightLim){
					return true;
				}
				sum1++;
			
		}

		//Izquierda
			for(int i=posX-1; i>=xLeftLim; i--){
				if(matriz[posY][i] != simbolo)
					break;
				
				if(i<0) break;

				if(i==xLeftLim)
					return true;
				
				sum1++;
			}

		int sum2=1;
		//Hacia abajo
			for(int i=posY+1; i<=yLowerLim; i++){
				if(matriz[i][posX] != simbolo)
					break;
				if(i>5) break;

				if(i==yLowerLim)
					return true;
				sum2++;
			}

		//Hacia arriba
			for(int i=posY-1; i>=yUpperLim; i--){
				if(matriz[i][posX] != simbolo)
					break;
				if(i<0) break;

				if(i==yUpperLim)
					return true;
				sum2++;
			}
		

		int sum3=1;
		//Hacia arriba a la derecha
			for(int i=posY-1, j=posX+1; i>=yUpperLim && j<=xRightLim; i--, j++){
				if(matriz[i][j] != simbolo)
					break;

				if(i<0 || j>6) break;

				if(i==yUpperLim)
					return true;

				sum3++;
			}
		

		int sum4 = 1;
		//Hacia arriba a la izquierda
			for(int i=posY-1, j=posX; i>=yUpperLim-1 && j>=xLeftLim; i--, j--){
				if(matriz[i][j] != simbolo)
					break;

				if(i<0 || j<0) break;

				if(i==yUpperLim)
					return true;

				sum4++;
			}
		

		//Hacia abajo a la derecha
			for(int i=posY+1, j=posX+1; i<=yLowerLim && j<=xRightLim; i++, j++){
				if(matriz[i][j] != simbolo)
					break;

				if(i>5 || j>6) break;

				if(i==yLowerLim)
					return true;

				sum4++;
			}
		

		//Hacia abajo a la izquierda
			for(int i=posY+1, j=posX-1; i<=yLowerLim && j>=xLeftLim; i++, j--){
				if(matriz[i][j] != simbolo)
					break;

				if(i>5 || j<0) break;

				if(i==yLowerLim)
					return true;
				
				sum3++;
			
			}
		


		if(sum1==4 || sum2==4 || sum3==4 || sum4==4){
			return true;
		}
		else{
			return false;
		}
}

void PlayerSetup(TGAME* Juego){
	int cant_jugadores;
	cout << "Cuantas personas van a jugar(1 o 2); ";
	cin >> cant_jugadores;

	if(cant_jugadores==1){
		Juego->computadora = true;
		cout << "\nSuerte!";
		Sleep(2000);
	}
	else{
		Juego->computadora = false;
		

	}
	
}

void CopyMainMatrix(TGAME* Juego){
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			Juego->matriz_IA[i][j] = Juego->matriz[i][j];
		}
	}
}

int JugadaComputadora(TGAME* Juego){
	//LLenar el arreglo donde vamos a darle puntos a las columnas con 0
	int max, maxPoint;
	
	for(int i=0; i<7; i++){
		Juego->mejor_col[i]=0;
	}
	
	for (int i=0; i<10000; i++){//Hacer esta prueba 1000 veces;
		CopyMainMatrix(Juego);//Copiar todo los caracters de la matriz principal a nuesta copia
		if((max=FillMatrix(Juego)) > 0){//Llenar la matriz con valores aleatorios
			Juego->turno = true;	
			return max;
		}

	}
	
	maxPoint = 0;
	max = Juego->mejor_col[0];//Asumimos que el primero es el mayor
	//Buscar la jugada que es mas probable que nos salga bien
	for(int i=1; i<7; i++){
		if(max < Juego->mejor_col[i]){
			max = Juego->mejor_col[i];
			maxPoint = i;
		}
	}

	Juego->turno = true;
	return maxPoint;
}

int FillMatrix(TGAME* Juego){
	Juego->turno = true;

	int first_move = rand()%7;
	if(llenarColComp(first_move, Juego, Juego->matriz_IA))
		return first_move;
	int move;
	for(int i=0; i<60; i++){
		move = rand()%7;
		if(llenarColComp(move, Juego, Juego->matriz_IA)){
			if(Juego->turno){
				Juego->mejor_col[move]--;
			}
			else
				Juego->mejor_col[move]++;
			break;
		}
		Juego->mejor_col[move]++;
	}

	return -1;
}

bool llenarColComp(int pos, TGAME* Juego, char matriz[6][7]){
	char simbolo;
	//Simbolo coje el caracter del jugador 1 o del jugador 2
	if(!(Juego->turno))simbolo = Juego->player1;
	else simbolo = Juego->player2;

	int i;
	for(i=5; i>=0; i--){
		if(matriz[i][pos] == ' '){
			matriz[i][pos] = simbolo;
			Juego->turno = !(Juego->turno);
			break;
		}
	}
	return hayGanador(Juego, matriz, pos, i);
}
