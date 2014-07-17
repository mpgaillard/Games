
#include "stdafx.h"

int pos_cursor[2] = {36, 12};//pos inicial x = 36, y = 12
vector<char> matriz (9,' ');

int GameInit(TGAME* Juego)
{	
	if(!Keyboard_Init(&(Juego->Teclado)))
		return -1;
	
	if(!GraphicManager_Init(&(Juego->Pantalla)))
		return -2;

	Juego->EstadoActual = Iniciando;
	Juego->GanadoresEncontrados = false;
	Juego->contador = 0;
	Juego->cursorClock = clock();
	Juego->cursor = char(177);
	memset(Juego->message, ' ', sizeof(Juego->message));
	blank();
	GraphicManager_SetColor(&(Juego->Pantalla), eBlack, eWhite);

	if(Juego->cant_jugadores==1){
		if(Juego->player1=='X')
				Juego->turno = false;
			else{
				Juego->turno = true;
			}
	}
	else{
		if(Juego->player1=='X'){
			Juego->turno = false;
			cout<< "Inicia Player1";
		}
		else{
			Juego->turno = true;
			cout<< "Inicia Player2";
		}
	}

	return 0;
}


int GameUpdate(TGAME* Juego){
	int posicionVect = getVectorPos();
	Keyboard_Update(&(Juego->Teclado));//actualizar la memoria caché

	if(Keyboard_Check(&(Juego->Teclado), eKey_Esc))
	{
		Juego->EstadoActual = Terminado;
		return 0;
	}

	if(Juego->GanadoresEncontrados== true || Juego->contador>=9 || hayUnGanador(matriz, Juego) != 'N'){
		Juego->EstadoActual = EnPausa;
		strcpy(Juego->message, "Presione Enter para seguir jugando");
		if(Keyboard_Check(&(Juego->Teclado), eKey_Enter)){
			GameInit(Juego);
		}
		return 0;
	}

	

	

	if(Juego->cant_jugadores==1 && Juego->turno==1){
		llenarPosicion(matriz, Juego, posicionVect);
		Juego->contador++;
		Juego->turno = false;
	}

	else if(Keyboard_Check(&(Juego->Teclado), eKey_Enter) || Keyboard_Check(&(Juego->Teclado), eKey_X) || Keyboard_Check(&(Juego->Teclado), eKey_O))
	{
		if(esLegal(posicionVect, matriz) && !Juego->turno){
				llenarPosicion(matriz, Juego, posicionVect);
				Juego->turno = true;
				Juego->contador++;
		}
		else if(esLegal(posicionVect, matriz) && Juego->turno){
			llenarPosicion(matriz, Juego, posicionVect);
				Juego->turno = false;
				Juego->contador++;
		}
	}


	return 0;
}


void DrawCursor(TGAME* Juego, vector<char> &matriz){
	float difClock=clock();

	if(!(Juego->turno))GraphicManager_SetColor(&(Juego->Pantalla), eRed, eWhite);
	else GraphicManager_SetColor(&(Juego->Pantalla), eBlue, eWhite);

	GraphicManager_WriteChar(&(Juego->Pantalla), pos_cursor[0], pos_cursor[1], Juego->cursor);
	
	if(Keyboard_Check(&(Juego->Teclado), eKey_Right)){
		pos_cursor[0]+=4;
		if(pos_cursor[0]>44)
			pos_cursor[0]=36;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Left)){
		pos_cursor[0]-=4;
		if(pos_cursor[0]<36)
			pos_cursor[0]=44;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Down)){
		pos_cursor[1]+=2;
		if(pos_cursor[1]>16)
			pos_cursor[1]=12;
	}
	else if(Keyboard_Check(&(Juego->Teclado), eKey_Up)){
		pos_cursor[1]-=2;
		if(pos_cursor[1]<12)
			pos_cursor[1]=16;
	}

	if(difClock-(Juego->cursorClock) >400){
		Juego->cursorClock = clock();
		if (Juego->cursor==char(177)) Juego->cursor= matriz[getVectorPos()];
		else Juego->cursor = char(177);
	}

	Sleep(100);
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
	
	GraphicManager_SetColor(&(Juego->Pantalla), eBlack, eWhite);

	char lineaActual[COLUMNAS]={0};
	char fraseInicio[] = "Digite un numero entre 0 y 8 que coresponde a la posicion deseada";	
	string p1 = convertInt(Juego->cant_ganadas1);
	string p2 = convertInt(Juego->cant_ganadas2);

	for(int i=0; i< FILAS; i++){
		if(i==0){
			GraphicManager_WriteChars(&(Juego->Pantalla), 0, i, "Elige entre 0 y 8 que coresponde a la posicion deseada o usando flechas");
		}
		else if(i==3){
			GraphicManager_WriteChars(&(Juego->Pantalla), 5, i, "Player 1:");
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "0 | 1 | 2");
		}
		else if(i==4){
			GraphicManager_WriteChars(&(Juego->Pantalla), 9, i, strcpy(Juego->player1_score, p1.c_str()));
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "---------");
		}
		
		else if(i==5){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "3 | 4 | 5");
		}
		else if(i==6){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "---------");
			GraphicManager_WriteChars(&(Juego->Pantalla), 5, i, "Player 2:");
		}
		else if(i==7){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "6 | 7 | 8"); 
			GraphicManager_WriteChars(&(Juego->Pantalla), 9, i, strcpy(Juego->player2_score, p2.c_str()), sizeof(Juego->player2_score));
		}
		else if(i==12){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "  |   |  ");
		}
		else if(i==13){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "---------");
		}
		else if(i==14){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "  |   |  ");
		}
		else if(i==15){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "---------");
		}
		else if(i==16){
			GraphicManager_WriteChars(&(Juego->Pantalla), 36, i, "  |   |  "); 
		}

		else{
			GraphicManager_WriteChars(&(Juego->Pantalla), 0, i, lineaActual, COLUMNAS);
		}

		
		memset(lineaActual, ' ', sizeof(lineaActual)); 
	}
	GraphicManager_WriteChars(&(Juego->Pantalla), 0, 23, Juego->message, sizeof(Juego->message));
}


void DrawSymbols(TGAME* Juego){
	int posInicialX = 36;
	int posInicialY = 12;

	for(int i=0; i<9; i++){
		if(i%3==0 && i>0){
			posInicialY+=2;
			posInicialX=36;
		}

		GraphicManager_WriteChar(&(Juego->Pantalla), posInicialX, posInicialY, matriz[i]);
		posInicialX+=4;

	}

}

int GameRender(TGAME* Juego){
	GraphicManager_ClearBuffer(&(Juego->Pantalla));
	//Primero, dibujamos el marco.
	DrawMainframe(Juego);
	
	//Luego, dibujamos las entidades.
	//Apostadores y sus apuestas
	//Perros
	DrawSymbols(Juego);
	DrawCursor(Juego, matriz);
	GraphicManager_SwapBuffer(&(Juego->Pantalla));

	return 0;
}

void PlayerSetup(TGAME* Juego){
	Juego->posicion = 0;
	Juego->cant_ganadas1 = 0;
	Juego->cant_ganadas2 = 0;
	cout << "Cuantas personas van a jugar(1 o 2); ";
	cin >> Juego->cant_jugadores;

	if(Juego->cant_jugadores==1){
		Juego->computadora = true;
		cout << "Que simbolo deseas usar(X o O): ";
		simbolo(Juego->player1, Juego->player2);
	
	}
	else{
		Juego->computadora = false;
		cout << "Que simbolo deseas usar Player1(X o O): ";
		simbolo(Juego->player1, Juego->player2);
	
		if(Juego->player1=='X'){
			cout<< "Inicia Player1";
		}
		else{
			cout<< "Inicia Player2";
		}
		Sleep(2000);
	}
	

}

void simbolo(char &player1, char &player2){
	char temp;
	cin >> temp;
	temp = toupper(temp);
	if (temp == 'X'){
		player1 = 'X';
		player2 = 'O';
	}
	else{
		player1 = 'O';
		player2 = 'X';
	}
}

void llenarPosicion(vector<char>& matriz, TGAME* Juego, int posicion){
	if (!(Juego->turno)){//Jugador1
		matriz[posicion] = Juego->player1;
	}
	else{
		if(Juego->computadora){
			//cout << jugadaComputadora(matriz); 
			matriz[jugadaComputadora(matriz, Juego)] = Juego->player2;
		}
		else{
			matriz[posicion] = Juego->player2;
		}
	}
}

bool esLegal(short pos, const vector<char>& matriz){
	if(matriz[pos] == ' ')
		return true;
	else {
		return false;
	}
}

char hayUnGanador(const vector<char>& matriz, TGAME* Juego){
	short matriz_ganadora[8][3] = {{0, 1, 2},
						{3,4,5},
						{6,7,8},
						{0,3,6},
						{1,4,7},
						{2,5,8},
						{0,4,8},
						{2,4,6}};
	for(int i=0; i<8; i++){
		if(matriz[matriz_ganadora[i][0]] == matriz[matriz_ganadora[i][1]] && matriz[matriz_ganadora[i][1]] == matriz[matriz_ganadora[i][2]]){
			if(matriz[matriz_ganadora[i][0]] != ' '){
				if(Juego->player1 == 'X' && matriz[matriz_ganadora[i][0]] == 'X') Juego->cant_ganadas1++;
				else if(Juego->player1 == 'O' && matriz[matriz_ganadora[i][0]] == 'O') Juego->cant_ganadas1++;
				else Juego->cant_ganadas2++;

				Juego->GanadoresEncontrados = true;
				return matriz[matriz_ganadora[i][0]];
			}
		}
	}
	return 'N';
}

short jugadaComputadora(vector<char> matriz, TGAME* Juego){
	/* Si la computadora puede ganar en la siguiente jugada*/
	int cant_jugador1 = Juego->cant_ganadas1;
	
	for(short i=0; i<9; i++){
		if(esLegal(i, matriz)){
			matriz[i]=Juego->player2;
			if(hayUnGanador(matriz, Juego)==Juego->player2){
				Juego->cant_ganadas1 = cant_jugador1;
				Juego->cant_ganadas2--;
				Juego->GanadoresEncontrados = false;
				return i;
			}
			matriz[i]=' ';
		}
	}

	/* Si el humano puede ganar en la siguiente jugada*/
	for(short i=0; i<9; i++){
		if(esLegal(i, matriz)){
			matriz[i]=Juego->player1;
			if(hayUnGanador(matriz, Juego)==Juego->player1){
				Juego->cant_ganadas1 = cant_jugador1;
				Juego->GanadoresEncontrados = false;
				return i;
			}
			matriz[i]=' ';
		}
	}

	//Lista de mejores jugadas
	short mejoresJugadas[9] = {4, 0, 2, 8, 6, 3, 5, 1, 7}; 
	for (int i=0; i<9; i++){
		if(esLegal(mejoresJugadas[i], matriz)){
			return mejoresJugadas[i];
		}
	}

	return 0;
}

int getVectorPos(){
	int initX = 36;
	int initY=12;
	int pos=0;
	while(initX<pos_cursor[0]){
		initX+=4;
		pos++;
	}
	while(initY<pos_cursor[1]){
		initY+=2;
		pos+=3;
	}
	return pos;
}

void blank(){
	for(int i=0; i<9; i++){
		matriz[i] = ' ';
	}
}