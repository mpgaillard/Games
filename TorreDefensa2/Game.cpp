#include "Game.h"
#include "tinyxml.h"


int InicializarJuego(TJUEGO* Juego)
{
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();
	srand(time(NULL));

	set_color_depth(COLORDEPTH);

	if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
		allegro_message("Error en inicializacion de sistema de sonido\n%s\n", allegro_error);
		return -2;
	}

	int ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO, ALTO, 0, 0);
	if(ret != 0){
		allegro_message(allegro_error);
		return -3;
	}

	Juego->music = load_midi("enter_sandman.mid");
	Juego->gun_shot = load_sample("gun_shoot_metal.wav");
	Juego->scream = load_sample("hmscream.wav");
	Juego->explosion = load_sample("BOOM.wav");
	Juego->towerblowup = load_sample("towerblowup.wav");
	play_midi(Juego->music, INT_MAX);

	Juego->buffer = create_bitmap(SCREEN_W,SCREEN_H);
	clear_bitmap(Juego->buffer);
	
	Juego->tiempoDisparo = clock();

	//Bitmaps[0] corresponde al fondo
	Juego->single_frame_bitmaps.push_back(load_bitmap("fondo.bmp", NULL));
	//Bitmap[1] corresponde a la torre
	Juego->single_frame_bitmaps.push_back(load_bitmap("torre (1).bmp", NULL));
	//Bitmap[2] corresponde al arma
	Juego->single_frame_bitmaps.push_back(load_bitmap("arma.bmp", NULL));
	//Bitmap[3] corresponde al crosshair
	Juego->single_frame_bitmaps.push_back(load_bitmap("crosshair.bmp", NULL));
	//Bitmap[4] corresponde al proyectil
	Juego->single_frame_bitmaps.push_back(load_bitmap("bala.bmp", NULL));
	//Bitmap[5] corresponde a la explosion
	Juego->single_frame_bitmaps.push_back(load_bitmap("explode.bmp", NULL));

	set_mouse_sprite(Juego->single_frame_bitmaps[3]);
	set_mouse_sprite_focus(Juego->single_frame_bitmaps[3]->w/2, Juego->single_frame_bitmaps[3]->h/2);
	

	InicializarLista(&(Juego->multi_frame_bitmaps));
	InicializarLista(&(Juego->proyectiles));
	InicializarLista(&(Juego->enemigos));

	BitmapVectors* enemy1 = new BitmapVectors();
	char temp[20];
	for(int i=0; i<8; i++){
		sprintf(temp, "enemy1f%d.bmp", i+1);
		enemy1->bv.push_back(load_bitmap(temp, NULL));
	}

	BITMAP* temp_bitmap2 = load_bitmap("spawn2.bmp", NULL);
	BitmapVectors* enemy2 = new BitmapVectors();
	for(int i=0; i<5; i++){
		sprintf(temp, "spawn_%d.bmp", i+1);
		enemy2->bv.push_back(load_bitmap(temp, NULL));
	}

	//Spawn Attack
	for(int i=0; i<5; i++){
		enemy2->bv.push_back(grabframe(temp_bitmap2, W_SPAWN2, temp_bitmap2->h, 0, 0, 5, i));
	}

	BitmapVectors* attack_mega = new BitmapVectors();
	for(int i=0; i<5; i++){
		sprintf(temp, "attack_%d.bmp", i+1);
		attack_mega->bv.push_back(load_bitmap(temp, NULL));
	}


	BitmapVectors* spawn_attack = new BitmapVectors();
	spawn_attack->bv.push_back(load_bitmap("spawn_attack.bmp", NULL));

	BitmapVectors* plane = new BitmapVectors();
	for(int i=0; i<4; i++){
		sprintf(temp, "plane%d.bmp", i);
		plane->bv.push_back(load_bitmap(temp, NULL));
	}

	CrearNodo(&(Juego->multi_frame_bitmaps), enemy1, true);
	CrearNodo(&(Juego->multi_frame_bitmaps), enemy2, true);
	CrearNodo(&(Juego->multi_frame_bitmaps), plane, true);
	CrearNodo(&(Juego->multi_frame_bitmaps), attack_mega, true);
	CrearNodo(&(Juego->multi_frame_bitmaps), spawn_attack, true);

	ResetJuego(Juego);

	
	//delete temp_bitmap2;
//	delete enemy2;
//	delete plane;
}

int ActualizarJuego(TJUEGO* Juego)
{
	//Game Over Rapido
	if(Juego->estado == Fin){
		Juego->Torre->curframe++;
		rest(100);
		if(Juego->Torre->curframe == 4){
			FONT *Font1= load_font("font1.pcx", NULL, NULL);
			textout_centre_ex(screen, Font1, "GAME OVER!", SCREEN_W/2, SCREEN_H/2, makecol(255, 0, 0), -1); 
			rest(4000);
			Juego->estado = Exit;
		}
		return 0;
	}

	if(Juego->Torre->hp == 0){
		//play_sample(Juego->towerblowup, 128, 128, 1000, FALSE);
		Juego->estado = Fin;
		return 0;
	}

	if(Juego->Torre->hp ==25){
		Juego->Torre->curframe = 1;
	}

	if((mouse_b & 1) && clock()-Juego->tiempoDisparo > 800){
		DispararProyectil(Juego);
		Juego->tiempoDisparo = clock();
		//allegro_message("it works");
	}

	

	//Si la cantidad de enemigos que han salido es menor que la cantidad por ola y si han pasado 3 segundos menos una constante por ola
	//Inicializar un nuevo enemigo
	if((Juego->cant_enemigos < Juego->wave + 10) && clock()-Juego->tiempo_transcurido >= 3000-(Juego->wave*200)){
		InicializarEnemigo(Juego, Juego->wave+1);
		Juego->tiempo_transcurido = clock();
	}

	//Si han salido todos los enemigos y todos estan muertos
	else if(Juego->cant_enemigos >= Juego->wave + 10 && Juego->enemigos.cantidad == 0){
		Juego->wave++;
		Juego->cant_enemigos = 0;
		FONT *Font1= load_font("font1.pcx", NULL, NULL);
		textprintf_centre_ex(screen, Font1, SCREEN_W/2, SCREEN_H/2, makecol(0, 255, 0), -1, "Wave %d", Juego->wave+1); 
		destroy_font(Font1);
		rest(4000);
	}


	//Funcion que mueve el proyectil disparado por el cañon
	MoverEnemigos(Juego);
	MoverProyectiles(Juego);
	MoverProyectilesEnemigos(Juego);
	BuscarColisionTorre(Juego);

	return 0;
}


void InicializarTorre(TJUEGO* Juego)
{
	Juego->Torre = &(Juego->LaTorre);
	Juego->Torre->width = W_TORRE;
	Juego->Torre->height = Juego->single_frame_bitmaps[1]->h;
	Juego->Torre->x = POSINICIALTORREX;
	Juego->Torre->y = SCREEN_H - Juego->Torre->height - PISO;
	Juego->Torre->hp = 100.0;
	Juego->Torre->curframe = 0;

	Juego->arma = &(Juego->LaArma);
	Juego->arma->width = Juego->single_frame_bitmaps[2]->w;
	Juego->arma->height = Juego->single_frame_bitmaps[2]->h;
	Juego->arma->x = Juego->Torre->x + Juego->Torre->width/2;
	Juego->arma->y = Juego->Torre->y - Juego->arma->height + PIVOT_Y + 28;
	Juego->arma->curframe = 0;
}

int RenderizarJuego(TJUEGO* Juego)
{

	BITMAP* TorreActual = grabframe(Juego->single_frame_bitmaps[1], W_TORRE, Juego->Torre->height, 0, 0, 8, Juego->Torre->curframe); 
	BitmapVectors* enemigo_bm;
	

	//Fondo
	draw_sprite(Juego->buffer, Juego->single_frame_bitmaps[0], 0, 0);
	
	rectfill(Juego->buffer, 10, 10, 10+Juego->Torre->hp, 22, GREEN);
	rectfill(Juego->buffer, 10+Juego->Torre->hp, 10, 10+Juego->Torre->hp+(100-Juego->Torre->hp), 22, RED);  
	textprintf_ex(Juego->buffer, font, 10, 24, makecol(255, 255, 255), -1, "Score: %d", Juego->score); 

	//dibujar los enemigos visibles
	caracter* enemigoActual;
	for(int i=0; i<Juego->enemigos.cantidad; i++)
	{
		enemigoActual = (caracter*)BuscarNodoIndice(&(Juego->enemigos), i)->data;
		
		if(enemigoActual != NULL){
			enemigo_bm = (BitmapVectors*)(BuscarNodoIndice(&(Juego->multi_frame_bitmaps),enemigoActual->id_lista_imagenes)->data);
			draw_sprite(Juego->buffer, enemigo_bm->bv[enemigoActual->curframe], enemigoActual->x, enemigoActual->y);
		}
	}
	
	draw_sprite(Juego->buffer, TorreActual, Juego->Torre->x, Juego->Torre->y); 
	pivot_sprite(Juego->buffer, Juego->single_frame_bitmaps[2], Juego->arma->x, Juego->arma->y, PIVOT_X,PIVOT_Y, ftofix(GetAngleArma(Juego)*0.7111));
	
	

	//dibujar los proyectiles
	caracter* proyectilActual;
	for(int i=0; i<Juego->proyectiles.cantidad; i++)
	{
		proyectilActual = (caracter*)BuscarNodoIndice(&(Juego->proyectiles), i)->data;
		//draw_sprite(Juego->buffer, Juego->single_frame_bitmaps[4], proyectilActual->x, proyectilActual->y);
		if(proyectilActual != NULL)
			pivot_sprite(Juego->buffer, Juego->single_frame_bitmaps[4], proyectilActual->x, proyectilActual->y, proyectilActual->width/2, proyectilActual->height/2, ftofix(proyectilActual->anguloAtaque*0.7111));
		
	}

	//Dibujar los proyectiles del enemigo
	 list<caracter*>::iterator it;
	 caracter* proyectilActualEnemigo;
	

	 for ( it=Juego->proyectil_enemigo.begin() ; it != Juego->proyectil_enemigo.end(); it++ ){
		 proyectilActualEnemigo = *it;
		 enemigo_bm = (BitmapVectors*)BuscarNodoIndice(&(Juego->multi_frame_bitmaps), proyectilActualEnemigo->id_lista_imagenes)->data;
		 draw_sprite(Juego->buffer, enemigo_bm->bv[proyectilActualEnemigo->curframe], proyectilActualEnemigo->x, proyectilActualEnemigo->y);
		 //draw_sprite(Juego->buffer, Juego->single_frame_bitmaps[proyectilActualEnemigo->id_lista_imagenes], proyectilActualEnemigo->x, proyectilActualEnemigo->y);
	 }


	if(Juego->existe_explosion && clock()-Juego->tiempo_explosion<300)
		DrawExplosion(Juego, Juego->coords_explosion.x, Juego->coords_explosion.y); 
	else if(clock()-Juego->tiempo_explosion >= 300)
		Juego->existe_explosion = false;

	show_mouse(Juego->buffer);
	acquire_screen();
	blit(Juego->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	release_screen();

	return 0;
}

int FinalizarJuego(TJUEGO* Juego)
{
	remove_mouse();
	remove_keyboard();
	allegro_exit();

	return 0;
}

void ResetJuego(TJUEGO* Juego)
{
	Juego->estado = EnProceso; 
	blit(Juego->single_frame_bitmaps[0], Juego->buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	InicializarTorre(Juego);
	
	if(Juego->proyectiles.cantidad>0)
		VaciarLista(&Juego->proyectiles);
	
	//Inicializar todos las casillas que puede ocupar un enemigo como falso
	for(int i=0; i<8; i++) Juego->space_list[i] = false;

	Juego->wave = 0;
	Juego->tiempo_enemigos = 3000;
	Juego->cant_enemigos = 0;
	Juego->tiempo_transcurido = -3000;
	Juego->existe_explosion = false;
	Juego->score = 0;

	position_mouse(SCREEN_W/2, Juego->arma->y+Juego->arma->height/2-PIVOT_Y);
}


void InicializarEnemigo(TJUEGO* Juego, int factor)
{
	BitmapVectors* enemigo_bmp;

	short random = rand()%CANT_BITMAPS_ENEMIGOS;
	vector<short> opciones; 

		caracter* enemigo = new caracter();
		enemigo->alive = true;

		for(int i=0; i<8; i++){
			if (Juego->space_list[i] == false){
				opciones.push_back(i);
			}
		}

		if(opciones.size() == 0) return;

		Juego->cant_enemigos++;

		
		enemigo->occupied_space = opciones[rand()%opciones.size()];//El espacio que ocupa el enemigo
		Juego->space_list[enemigo->occupied_space] = true;//Marcar verdadero el espacio ocupado

		//Inicializar un enemigo tipo megaman
		if(random == 0){
			enemigo_bmp = (BitmapVectors*)(BuscarNodoIndice(&(Juego->multi_frame_bitmaps),0)->data);
			enemigo->id_lista_imagenes = 0;
			enemigo->x = enemigo->posXInicial = 400+(50*enemigo->occupied_space);
			enemigo->y = 0;
			enemigo->velocidadX = enemigo->velocidadY = 0;
			enemigo->posYInicial = 0;
			enemigo->tiempoActual = clock();
			enemigo->factorAtaque = factor;
			enemigo->curframe = 0;
			enemigo->width = enemigo_bmp->bv[1]->w;
			enemigo->height = enemigo_bmp->bv[0]->h;
			CrearNodo(&Juego->enemigos, enemigo, true);
		}

		//Inicializar un enemigo tipo spawn
		else if(random == 1){
			enemigo_bmp = (BitmapVectors*)(BuscarNodoIndice(&(Juego->multi_frame_bitmaps),1)->data);
			enemigo->id_lista_imagenes = 1;
			enemigo->factorAtaque = factor;
			enemigo->curframe = 0;
			enemigo->tiempoActual = clock();
			enemigo->tiempoMov = clock();
			enemigo->width = enemigo_bmp->bv[1]->w;
			enemigo->height = enemigo_bmp->bv[2]->h;
			enemigo->x  = enemigo->posXInicial = SCREEN_W-enemigo->width;
			enemigo->y = enemigo->posYInicial = SCREEN_H-PISO-enemigo->height;
			enemigo->velocidadY = 0;
			enemigo->velocidadX = -300/1000.0;//300 pixeles por segundo
			CrearNodo(&Juego->enemigos, enemigo, true);
		}

}

void MoverEnemigos(TJUEGO* Juego){
	
	caracter* enemigoActual;
	for(int i=0; i < Juego->enemigos.cantidad; i++){
		enemigoActual = (caracter*)BuscarNodoIndice(&(Juego->enemigos), i)->data;
		if(enemigoActual == NULL) return;
		switch(enemigoActual->id_lista_imagenes)
		{
			case 0:
				MegaMove(Juego, enemigoActual);
				break;
			case 1:
				SpawnMove(Juego, enemigoActual);
				break;
			default:
				break;
		}
	}

}

void SpawnMove(TJUEGO* Juego, caracter* Spawn){
	if(Spawn->x > 400+(Spawn->occupied_space*50)){
		AplicarGravedad(Spawn, Spawn->tiempoActual, false);
		
		if(clock()-Spawn->tiempoMov> 180){
			Spawn->tiempoMov = clock();
			Spawn->curframe++;
		}
	}
	else {
		//Spawn->curframe = 0;
		Attack(Juego, Spawn);
		return;
	}
	
		if(Spawn->curframe >= 5) Spawn->curframe = 0;
	
}

void Attack(TJUEGO* Juego, caracter* Entidad){
	switch(Entidad->id_lista_imagenes)
	{
	case 0:
		if(clock()- Entidad->tiempoMov > 150){
			Entidad->tiempoMov = clock();
			Entidad->curframe++;
		}
		if(Entidad->curframe > 7){
			Entidad->curframe = 0;
			CrearBalaEnemigo(Juego, 3, Entidad->x, Entidad->y);
		}
		break;

	case 1:
		if(Entidad->curframe < 5){
			Entidad->curframe = 5;
		}

		if(clock()- Entidad->tiempoMov > 500){
			Entidad->tiempoMov = clock();
			Entidad->curframe++;
		}
		if(Entidad->curframe > 9){
			CrearBalaEnemigo(Juego, 4, Entidad->x, Entidad->y);
			Entidad->curframe = 5;
		}
		break;
	default:
		break;
	}



}


void MegaMove(TJUEGO* Juego, caracter* MegaMan){
	//Mover de acuerdo a las leyes de la gravedad
	if(MegaMan->y+MegaMan->height < SCREEN_H - PISO){
		AplicarGravedad(MegaMan, MegaMan->tiempoActual, true);
	}
	else//Si ya esta en posicion de ataque, atacar
		Attack(Juego, MegaMan);
}

void CrearBalaEnemigo(TJUEGO* Juego, int tipo, int x, int y)
{//Creacion de una nueva bala y agregar a la lista de proyectil_enemigo
	caracter* NuevaBala = new caracter();
	NuevaBala->id_lista_imagenes = tipo;
	NuevaBala->x = NuevaBala->posXInicial = x;
	NuevaBala->y = NuevaBala->posYInicial = y;
	NuevaBala->velocidadYInicial = NuevaBala->velocidadY = 0;
	NuevaBala->velocidadXInicial = NuevaBala->velocidadX = -250/1000.0;
	NuevaBala->tiempoActual = clock();
	NuevaBala->tiempoMov = clock();
	NuevaBala->curframe = 0;
	Juego->proyectil_enemigo.push_back(NuevaBala);
}

void MoverProyectilesEnemigos(TJUEGO* Juego){
	
	 list<caracter*>::iterator it;
	 caracter* proyectilActual;
	
	 //Buscar en todos los nodos de la lista, agregar x = vx*t y si es una bala de tipo megaman, cambiar el frames cada cierto tiempo
	 for ( it=Juego->proyectil_enemigo.begin() ; it != Juego->proyectil_enemigo.end(); it++ ){
		 proyectilActual = *it;
		 AplicarGravedad(proyectilActual, proyectilActual->tiempoActual, false);

		 if(proyectilActual->id_lista_imagenes == 3 && clock()-proyectilActual->tiempoMov>150){
			 proyectilActual->tiempoMov = clock();
			 proyectilActual->curframe++;
			 if(proyectilActual->curframe > 4)  proyectilActual->curframe=0;
		 }
	 }
    
}

void BuscarColisionTorre(TJUEGO* Juego){
	 list<caracter*>::iterator it;
	 caracter* proyectil;
	
	 //Buscar en todos los nodos de la lista, agregar x = vx*t y si es una bala de tipo megaman, cambiar el frames cada cierto tiempo
	 for ( it=Juego->proyectil_enemigo.begin() ; it != Juego->proyectil_enemigo.end(); it++ ){
		 proyectil = *it;
		if(proyectil->x < Juego->Torre->x + Juego->Torre->width){
			Juego->proyectil_enemigo.remove(proyectil);
			play_sample(Juego->explosion, 140, 64, 1000, FALSE);
			Juego->Torre->hp--;
			Juego->tiempo_explosion = clock();
			Juego->existe_explosion = true;
			Juego->coords_explosion.x = proyectil->x;
			Juego->coords_explosion.y = proyectil->y;
			return;
		}
	}

}

void DrawExplosion(TJUEGO* Juego, int x, int y)
{
	rotate_sprite(Juego->buffer, Juego->single_frame_bitmaps[5], x+rand()%20-20, y+rand()%20-20, itofix(rand()%256));
}

void AplicarGravedad(caracter* entidad, float tiempo, bool gravedad)
{
	double tiempoTranscurido;
	tiempoTranscurido = (clock() - tiempo);
	
	
	entidad->x = (double)entidad->velocidadX*(tiempoTranscurido) + entidad->posXInicial;

	if(gravedad)//y = posInicial + v*t + 1/2(a*t^2)
		entidad->y = entidad->posYInicial + ((double)entidad->velocidadY*(tiempoTranscurido)) + (0.5*ACEL_GRAVEDAD*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);

}


void borrarsprite(caracter* spr, TJUEGO* Juego){
	//Dibujar la parte del fondo donde esta el sprite actual y del tamaño del sprite
	blit(Juego->single_frame_bitmaps[0], Juego->buffer, spr->x, spr->y, spr->x, spr->y, spr->width, spr->height);
}


BITMAP* grabframe(BITMAP *source, 
                  int width, int height, 
                  int startx, int starty, 
                  int columns, int frame)
{
    BITMAP *temp = create_bitmap(width,height);

    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    

	blit(source, temp, x, y, 0, 0, width, height);

    return temp;
}

float GetAngleArma(TJUEGO* Juego)
{
	float x = mouse_x-(Juego->arma->x+Juego->arma->width);//Distancias entre el extremo derecho del arma a donde se hace el click
	float y = mouse_y-(Juego->arma->y+Juego->arma->height/2);

	float angle;
	if (x==0) return 0;

	angle = atan(y/x)*180/PI;

	if (angle > 50)
		return 50;
	else if(angle < -50)
		return -50;

	return angle;
}

void DispararProyectil(TJUEGO* Juego)
{
	float x, y, distance, porcentaje, velEscalar, vfinal;
	x = mouse_x-(Juego->arma->x+Juego->arma->width);
	y = mouse_y-(Juego->arma->y+Juego->arma->height/2);

	if(x<75) x = 75;
	if(x>SCREEN_W)  x = SCREEN_W;
	if(y>SCREEN_H) y = SCREEN_H;
	if(y<0) y = 0;

	play_sample(Juego->gun_shot, 128, 128, 1000, FALSE);	//BOOM
	distance = sqrt((x*x) + (y*y));
	porcentaje = distance/MAXDISTANCE;//distancia divido por 800pixeles/segundo para retornar un porcentaje
	velEscalar = porcentaje*MAXDISTANCE;

	caracter* proyectil = new caracter();
	proyectil->anguloAtaque = GetAngleArma(Juego);
	//proyectil->posXInicial = Juego->Torre->x+Juego->Torre->width+Juego->arma->width-(Juego->arma->width-(Juego->arma->width)*cos(proyectil->anguloAtaque*PI/180.0));
	proyectil->posXInicial = Juego->Torre->x+Juego->Torre->width+Juego->arma->width-(Juego->arma->width-(Juego->arma->width)*cos(proyectil->anguloAtaque*PI/180.0));
	proyectil->posYInicial = Juego->arma->y+(Juego->arma->width)*sin(proyectil->anguloAtaque*PI/180.0)-10;// + Juego->arma->width/2;
	proyectil->tiempoDisparo = proyectil->tiempoActual = clock();
	proyectil->velocidadXInicial = (velEscalar/(float)1000.0)*cos(proyectil->anguloAtaque*PI/180.0);
	proyectil->velocidadX = proyectil->velocidadXInicial;//vel en x es constante
	proyectil->velocidadY  = (velEscalar/(float)1000.0)*sin(proyectil->anguloAtaque*PI/180.0);
	proyectil->alive = true;
	proyectil->curframe = 0;
	proyectil->width = Juego->single_frame_bitmaps[4]->w;
	proyectil->height = Juego->single_frame_bitmaps[4]->h;
	vfinal = sqrt (proyectil->velocidadY*proyectil->velocidadY*1000*1000 + 2*ACEL_GRAVEDAD*PPM*((SCREEN_H-PISO) - proyectil->posYInicial));//vf^2 = vo^2 + 2a(xf-xi)
	
	if((vfinal+proyectil->velocidadY)*(float)1000.0 == 0)
		proyectil->velocidadY += 1;

	proyectil->tiempoVivo = (((SCREEN_H-PISO)-proyectil->posYInicial)*2)/(vfinal+proyectil->velocidadY)*(float)1000.0;//t = ((x-xo)*2)/(vo+v) 
	



	CrearNodo(&(Juego->proyectiles), proyectil, true);
}

void MoverProyectiles(TJUEGO* Juego)
{
	double tiempoTranscurido;


	for(int i=0; i<Juego->proyectiles.cantidad; i++)
	{
		caracter* proyectilActual = (caracter*)BuscarNodoIndice(&(Juego->proyectiles), i)->data;
		if(proyectilActual == NULL) return;//Si el nodo es nulo, retornar

		tiempoTranscurido = (clock() - proyectilActual->tiempoDisparo);
		AplicarGravedad(proyectilActual, proyectilActual->tiempoDisparo, true);

		if(proyectilActual->anguloAtaque != 90 && clock()-proyectilActual->tiempoActual > proyectilActual->tiempoVivo/(90-proyectilActual->anguloAtaque))//Si el tiempo transcurido es mayor al tiempo de vida de la bala entre la dif de angulos que quedan
		{
			proyectilActual->tiempoActual = clock();
			proyectilActual->anguloAtaque++;
		}

		if(BuscarColisionProyectil(Juego, proyectilActual)) return;

	}

}



bool BuscarColisionProyectil(TJUEGO* Juego, caracter* proyectil)
{

	for(int i=0; i<Juego->enemigos.cantidad; i++){
		caracter* enemigoActual = (caracter*)BuscarNodoIndice(&(Juego->enemigos), i)->data; 
		if(enemigoActual!= NULL && proyectil != NULL){
			if (ExisteColision(enemigoActual, proyectil, 0)){
				Juego->space_list[enemigoActual->occupied_space] = false;
				play_sample(Juego->scream, 128, 128, 1000, FALSE);	//BOOM
				EliminarNodo(&Juego->enemigos, enemigoActual);//Si hay colision, quitar nodo del enemigo de la lista
				Juego->score += 10;
				//EliminarNodo(&Juego->proyectiles, proyectil);
				return true;
			}
		}
	}

	if (proyectil!= NULL && proyectil->y + proyectil->height > SCREEN_H-PISO)
	{
		play_sample(Juego->explosion, 128, 128, 1000, FALSE);	//BOOM
		Juego->tiempo_explosion = clock();
		Juego->existe_explosion = true;
		Juego->coords_explosion.x = proyectil->x;
		Juego->coords_explosion.y = proyectil->y;
		//DrawExplosion(Juego, proyectil->x, proyectil->y);
		EliminarNodo(&Juego->proyectiles, proyectil);//Si choco con el suelo, quitar nodo de proyectil de la lista
		return true;
	}
	else if(proyectil!= NULL && proyectil->x + proyectil->width > SCREEN_W || proyectil->y < 0 ){
		EliminarNodo(&Juego->proyectiles, proyectil);//Si la bala se va fuera de la pantalla, borrarlo de la lista
		return true;
	}
	return false;
}

bool ExisteColision(caracter* primero, caracter* segundo, int esquina){

	  //retornar pos + ancho/alto de ambos sprites
  int width1 = primero->x + primero->width;
  int height1 = primero->y + primero->height;
  int width2 = segundo->x + segundo->width;
  int height2 = segundo->y + segundo->height;

  //ver si la esquina del primero esta dentro del segundo
  if (inside(primero->x, primero->y, primero->x + esquina, 
        segundo->y + esquina, width2 - esquina, height2 - esquina))
      return true;
  if (inside(primero->x, height1, segundo->x + esquina, 
        segundo->y + esquina, width2 - esquina, height2 - esquina))
      return true;
  if (inside(width1, primero->y, segundo->x + esquina, 
        segundo->y + esquina, width2 - esquina, height2 - esquina))
      return true;
  if (inside(width1, height1, segundo->x + esquina, 
        segundo->y + esquina, width2 - esquina, height2 - esquina))
      return true;

  //no collisions?
  return false;
}

 bool inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return true;
    else
        return false;
}

/*void CargarBitmaps(TJUEGO* Juego)
{

	TiXmlDocument bitmaps("bitmaps.xml");
	//bitmaps.LoadFile(".\\Data\\bitmaps.xml");

	if(bitmaps.LoadFile())
	{
		TiXmlNode *nodoBitmap = bitmaps.FirstChild("backgrounds");
		
	if(nodoBitmap->NoChildren()) {
		allegro_message("No se pudo cargar los bitmaps!");
		exit(-1);
	}

	TiXmlNode *nodoBitmapActual;

	for(nodoBitmapActual = nodoBitmap->FirstChild(); nodoBitmapActual; nodoBitmapActual = nodoBitmapActual->NextSibling())
	{
	//	CrearNodo(&(Juego->Sprites), load_bitmap(nodoSpriteActual->ToElement()->Attribute("ruta"), NULL), true);
		Juego->bitmaps.push_back(load_bitmap(nodoBitmapActual->ToElement()->Attribute("ruta"), NULL));
	}

	}
	else{
		allegro_message("Error. No se ha podido cargar el XML ni crear uno nuevo. ");
		exit(-1);
	}
}*/