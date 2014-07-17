#include "Game.h"
#include "Particles.h"

int option = 0;
int option2 = 0;
std::string Options[2] = {"New Game", "Player"};
std::string Options2[2] = {"Mario","Luiggi"};

int InicializarJuego(TJUEGO* Juego){
	Juego->estado = Inicio;

	allegro_init();
	install_keyboard();
	install_timer();
	set_color_depth(COLORDEPTH);
	set_window_title("Crisis Tomb");

	int ret = set_gfx_mode(GFX_AUTODETECT, ANCHO, ALTO, 0, 0);
	if(ret != 0){
		allegro_message(allegro_error);
		return -3;
	}

	//install the joystick handler
    install_joystick(JOY_TYPE_AUTODETECT);
    poll_joystick();
	

	  //look for a joystick
    if (num_joysticks == 0)
    {
        textout_ex(screen,font,"No controller could be found",0,20,WHITE,0);
        while(!keypressed());
        return -1;
    }Juego->startposjoy1 = joy[0].stick[0].axis[0].pos;
	Juego->startposjoy2 = joy[1].stick[0].axis[0].pos;

	if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
		allegro_message("Error en inicializacion de sistema de sonido\n%s\n", allegro_error);
		return -2;
	}
	
	Juego->single_frame_bmp.push_back(load_bitmap("Menu.bmp", NULL));
	Juego->single_frame_bmp.push_back(load_bitmap("gOver.bmp", NULL));
	Juego->single_frame_bmp.push_back(load_bitmap("heart.bmp", NULL));

	Juego->fondo = load_bitmap("bg2.bmp", NULL);

	//Inicializar buffer virtual
	Juego->bufferVirtual = create_bitmap(ANCHOVIRTUAL, ALTOVIRTUAL);
	clear_bitmap(Juego->bufferVirtual);
	
	

	char temp[20];
	
	//Creacion vector de los sprites de los niveles
	for(int i=0; i<CANTNIVELES; i++){
		sprintf(temp, "bloque%d.bmp", i+1);
		Juego->level_bmp_sheet.push_back(load_bitmap(temp, NULL));
	}

	//Creacion de las lista de los bitmaps de los caracteres
	BitmapVectors* luiggi = new BitmapVectors();
	//Walk(0-4), run(5-12) n Duck(13)
	for(int i=0; i<14; i++){
		luiggi->bv.push_back(grabframe(load_bitmap("luiggi.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}
	//Jump(14-17)
	for(int i=22; i<26; i++){
		luiggi->bv.push_back(grabframe(load_bitmap("luiggi.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}
	//Special Jump(18-25)
	for(int i=14; i<22; i++){
		luiggi->bv.push_back(grabframe(load_bitmap("luiggi.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}

	//Creacion de las lista de los bitmaps de los caracteres
	BitmapVectors* mario = new BitmapVectors();
	//Walk(0-4), run(5-12) n Duck(13)
	for(int i=0; i<4; i++){
		mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}
	mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, 0));
	for(int i=4; i<12; i++){
		mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}
	mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, 21));

	//Jump(14-17)
	for(int i=17; i<21; i++){
		mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}
	//Special Jump(18-25)
	mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, 22));
	for(int i=26; i<33; i++){
		mario->bv.push_back(grabframe(load_bitmap("mario.bmp", NULL), PLAYERW, PLAYERH, 0, 0, 16, i));
	}

	BitmapVectors* zombie = new BitmapVectors();
	for(int i=0; i<7; i++){
		zombie->bv.push_back(grabframe(load_bitmap("zombie.bmp", NULL), ZOMBIEW, ZOMBIEH, 0, 0, 7, i));
	}


	Juego->caracteres_bmp.push_back(luiggi);
	Juego->caracteres_bmp.push_back(mario);
	Juego->caracteres_bmp.push_back(zombie);
	Juego->music.push_back(load_midi("title-screen.mid"));
	Juego->music.push_back(load_midi("level-2.mid"));
	Juego->music.push_back(load_midi("level-3.mid"));
	Juego->music.push_back(load_midi("star.mid"));
	Juego->game_over = load_sample("Death.wav");

	InicializarJugador(Juego);

	return 0;
}

void InicializarJugador(TJUEGO* Juego)
{
	Juego->jugador = &(Juego->el_jugador);
	Juego->jugador->alive = true;
	strcpy(Juego->jugador->nombre, "mario");
	Juego->jugador->width = PLAYERW;
	Juego->jugador->height = PLAYERH;
	Juego->jugador->x = Juego->jugador->posXInicial = 0 ;
	Juego->jugador->y = Juego->jugador->posYInicial = SCREEN_H;
	Juego->jugador->sentido = true;//Mirando hacia la derecha
	Juego->jugador->tiempoDisparo = clock();
	Juego->jugador->tiempoMov = clock();
	Juego->jugador->tiempoAnim = clock();
	Juego->jugador->tiempoSufrimiento = clock();
	Juego->jugador->hp = 3;
	Juego->jugador->curframe = 0;
	Juego->jugador->velocidadX = 150.0/1000.0;//50pps^2
	Juego->jugador->velocidadXInicial = 0;
	Juego->jugador->velocidadY = Juego->jugador->velocidadYInicial = 0;
	Juego->jugador->ParticleEffect = true;
	Juego->jugador->jump1 = load_sample("MarioJump.wav");
	Juego->jugador->scream = load_sample("hmscream.wav");
}

int ActualizarJuego(TJUEGO* Juego)
{
	if(Juego->estado==Inicio || Juego->estado==Opciones){
		MenuEleccion(Juego);
		return 0;
	}
	
	if(clock()-Juego->tiempo_transcurido>=1000){
		Juego->tiempo_transcurido = clock();
		Juego->tiempo_inicio--;
	}

	if(Juego->tiempo_inicio == 0 || Juego->jugador->y > Juego->bufferVirtual->h || Juego->jugador->hp == 0){
		Juego->estado = Fin;
		return 0;
	}

	if(Juego->jugador->x > ANCHOVIRTUAL){
		NextLevel(Juego);
	}

	//Si no estamos al final del nivel, inicializar un nuevo zombie cada 1000 pixeles
	if(Juego->zombiePos - Juego->jugador->x < 800 && Juego->zombiePos < 24000){
		InicializarZombie(Juego);
	}

	//Borrar el jugador
	borrarsprite(Juego->jugador, Juego);

	//Borrar los zombies
	list<SPRITE*>::iterator it;
	SPRITE* zombieActual;
	 for ( it=Juego->caracteres.begin() ; it != Juego->caracteres.end(); it++ ){
		 zombieActual = *it;
		 borrarsprite(zombieActual, Juego);
	 }

	//Si el efecto de las particulas esta activo, borrar las particulas
	if(Juego->jugador->ParticleEffect){
		for(int i=0; i<MAXPARTICLES; i++){
			blit(Juego->fondo, Juego->bufferVirtual, particulas[i].x-PARTICLE_RAD, particulas[i].y-PARTICLE_RAD, particulas[i].x-PARTICLE_RAD, particulas[i].y-PARTICLE_RAD, PARTICLE_RAD*3, PARTICLE_RAD*3);//Juego->single_frame_bmp[0]->w, Juego->single_frame_bmp[0]->h);
		}
		ActualizarParticulas(Juego);
	}
	
	//Borrar tiempo que queda
	masked_blit(Juego->fondo, Juego->bufferVirtual, Juego->x, Juego->y, Juego->x, Juego->y, TEXTW, TEXTH);

	//Borrar vidas
	masked_blit(Juego->fondo, Juego->bufferVirtual, Juego->x+ANCHO-VIDAINIT, Juego->y, Juego->x+ANCHO-VIDAINIT, Juego->y, VIDAINIT, Juego->single_frame_bmp[2]->h);

	ActualizarJugador(Juego);

	ActualizarZombie(Juego);
	
	
	//Scroll right
	if(Juego->jugador->sentido && (Juego->jugador->x+Juego->jugador->width) > Juego->x+(SCREEN_W/2))
	{
		Juego->x += (Juego->jugador->x+Juego->jugador->width)-(Juego->x+(SCREEN_W/2));
		if(Juego->x > ANCHOVIRTUAL-SCREEN_W) Juego->x = ANCHOVIRTUAL-SCREEN_W;
	}
	//Scroll left
	if(!Juego->jugador->sentido && (Juego->jugador->x+Juego->jugador->width+STEP < Juego->x+(SCREEN_W/2))){
		Juego->x -= (Juego->x+(SCREEN_W/2))-(Juego->jugador->x+Juego->jugador->width+STEP);
		if(Juego->x < 0) Juego->x = 0;
	}
	//Scroll up
	if(Juego->jugador->y+Juego->jugador->height < Juego->y+(SCREEN_H/2))
	{
		Juego->y -= (Juego->y+(SCREEN_H/2))-(Juego->jugador->y+Juego->jugador->height);
		if(Juego->y < 0) Juego->y = 0;
	}

	//Scroll down
	if(Juego->jugador->y > Juego->y+(SCREEN_H/2))
	{
		Juego->y += Juego->jugador->y-(Juego->y+(SCREEN_H/2));
		if(Juego->y > ALTOVIRTUAL-SCREEN_H) Juego->y = ALTOVIRTUAL-SCREEN_H;
	}




	return 0;
}

int RenderizarJuego(TJUEGO* Juego){

	if(Juego->estado== Inicio || Juego->estado==Opciones){
		MenuRenderizado(Juego);
		return 0;
	}
	else if(Juego->estado == Fin){
		GameOverScreen(Juego);
	}

		//drawframe(load_bitmap(Juego->jugador->nombre, NULL), Juego->bufferVirtual, Juego->jugador->x, Juego->jugador->y, Juego->jugador->width, Juego->jugador->height, 0, 0, 8, Juego->jugador->curframe);
	
	 BitmapVectors* jugador;
	 list<BitmapVectors*>::iterator it;
	 it = Juego->caracteres_bmp.begin();
	 if(strcmp(Juego->jugador->nombre, "luiggi") == 0){//Dibujar luigi si se selecciono luigi
		jugador = *it;
	 }
	 else{
		it++;
		jugador= *it;//Seleccionar mario
	 }

	 if(Juego->jugador->sentido)//Si esta mirando hacia el este, se dibuja asi
		draw_sprite(Juego->bufferVirtual, jugador->bv[Juego->jugador->curframe], Juego->jugador->x, Juego->jugador->y);
	 else
		 draw_sprite_h_flip(Juego->bufferVirtual, jugador->bv[Juego->jugador->curframe], Juego->jugador->x, Juego->jugador->y);


	 list<SPRITE*>::iterator it2;
	 SPRITE* zombieActual;
	 BitmapVectors* zombie;
	 list<BitmapVectors*>::iterator it3;
	 it3 = Juego->caracteres_bmp.begin();
	 it3++;
	 it3++;
	 zombie = *it3;
	 //Dibujar los enemigos vivos de la lista
	 for ( it2=Juego->caracteres.begin() ; it2 != Juego->caracteres.end(); it2++ ){
		 zombieActual = *it2;
		 if(!zombieActual->sentido)
			draw_sprite(Juego->bufferVirtual, zombie->bv[zombieActual->curframe], zombieActual->x, zombieActual->y);
		 else draw_sprite_h_flip(Juego->bufferVirtual, zombie->bv[zombieActual->curframe], zombieActual->x, zombieActual->y);
	 }

	 if(Juego->jugador->ParticleEffect)
		 DibujarParticulas(Juego);

	 //Imprimir tiempo que le queda al jugador
	textprintf_ex(Juego->bufferVirtual, font, Juego->x, Juego->y, makecol(255,255,255), -1, "Time remaining: %d", Juego->tiempo_inicio);

	//Imprimir vidas
	for(int i=0; i<Juego->jugador->hp; i++)
	{
		draw_sprite(Juego->bufferVirtual, Juego->single_frame_bmp[2], Juego->x+(i*Juego->single_frame_bmp[2]->w)+(ANCHO-VIDAINIT), Juego->y);
	}

	acquire_screen();
	blit(Juego->bufferVirtual, screen, Juego->x, Juego->y, 0, 0, ANCHO-1, ALTO-1);
	release_screen();

	return 0;
}

int FinalizarJuego(TJUEGO* Juego){

	return 0;
}

void borrarsprite(SPRITE* spr, TJUEGO* Juego){

	//Dibujar la parte del fondo donde esta el sprite actual y del tamaño del sprite

	blit(Juego->fondo, Juego->bufferVirtual, spr->x, spr->y, spr->x, spr->y, spr->width, spr->height);

}

void AplicarVelocidad(SPRITE* entidad, float tiempo, bool velX, bool gravedad)
{
	double tiempoTranscurido;
	tiempoTranscurido = (clock() - tiempo);
	
	if(velX)
		entidad->x = (double)entidad->velocidadX*(tiempoTranscurido) + entidad->posXInicial;

	if(gravedad)//y = posInicial + v*t + 1/2(a*t^2)
		entidad->y = entidad->posYInicial + ((double)entidad->velocidadY*(tiempoTranscurido)) + (0.5*ACEL_GRAVEDAD*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);

}

void ActualizarJugador(TJUEGO* Juego)
{
	int d1, d2, d3, jump, special_jump, run;

	//read the joystick
	poll_joystick();
	d1 = joy[0].stick[0].axis[0].d1;//Movement to the left
	d2 = joy[0].stick[0].axis[0].d2;//Movement to the right
	d3 = joy[0].stick[0].axis[1].d2;//Down
	jump  = joy[0].button[0].b;//Jump
	run =  joy[0].button[2].b;//Run faster
	special_jump  = joy[0].button[3].b;//Special Jump

	double TimeFrame = clock() - Juego->jugador->tiempoMov;
	Juego->jugador->tiempoMov += TimeFrame;

	if(d1){
		Juego->jugador->sentido = false;
	}
	else if(d2)
		Juego->jugador->sentido = true;
	else if(Juego->jugador->ExisteColisionPiso){
		Juego->jugador->curframe = 0;
		Juego->jugador->tiempoMov = clock();
	}

	//Buscar colisiones con el piso
	if((Juego->jugador->ExisteColisionPiso=BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+(TILEW/2), Juego->jugador->y+Juego->jugador->height, true, false) == false) && (Juego->jugador->ExisteColisionPiso=BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+Juego->jugador->width-(TILEW/2), Juego->jugador->y+Juego->jugador->height, true, false)) == false)
	{
		AplicarVelocidad(Juego->jugador, Juego->jugador->tiempoSalto, false, true);
		if(!Juego->jugador->jumptype)
			JumpAnim(Juego->jugador);
		else SpecialJumpAnim(Juego->jugador);
	}
	else{//Si esta pisando algo
		Juego->jugador->ExisteColisionPiso = true;
		Juego->jugador->posYInicial = Juego->jugador->y - TILEH/4;
		Juego->jugador->tiempoSalto = clock();
		Juego->jugador->velocidadY=0;
		if(jump){
			Juego->jugador->tiempoSalto = clock();
			Juego->jugador->jumptype = 0;
			Juego->jugador->y -= TILEH/4;
			Juego->jugador->velocidadY = -300.0/1000.0;//220 pixeles/segundo
			play_sample(Juego->jugador->jump1, 128, 128, 1000, FALSE);	
		}
		if(special_jump){
			Juego->jugador->tiempoSalto = clock();
			Juego->jugador->jumptype = true;
			Juego->jugador->y -= TILEH/4;
			Juego->jugador->velocidadY = (-300.0/1000.0)*1.5;//220 pixeles/segundo
			play_sample(Juego->jugador->jump1, 128, 128, 1000, FALSE);	
		}
	}

	//Buscar colision techo
	if(BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+(TILEW/2), Juego->jugador->y+(TILEH/4), false, true) || BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+Juego->jugador->width-(TILEW/2), Juego->jugador->y+(TILEH/4), false, true)){
		Juego->jugador->tiempoSalto=clock();
		Juego->jugador->velocidadY = 0;
		Juego->jugador->posYInicial = Juego->jugador->y;
	}

	//Buscar colisiones Este
	if(Juego->jugador->sentido && BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+Juego->jugador->width-5, Juego->jugador->y+Juego->jugador->height-TILEH/2, false, false) == false){
		if(d2){
			if(!run){
				Juego->jugador->x += Juego->jugador->velocidadX*TimeFrame;
				WalkAnim(Juego->jugador);
			}
			else {
				Juego->jugador->x += Juego->jugador->velocidadX*TimeFrame*2;
				RunAnim(Juego->jugador);
			}
		}
	}

	//Buscar colisiones Oeste
	if(!Juego->jugador->sentido && BuscarColisionMundo(Juego, Juego->jugador, Juego->jugador->x+5, Juego->jugador->y+Juego->jugador->height-TILEH/2, false, false) == false){
		if(d1){
			if(!run){
				Juego->jugador->x += Juego->jugador->velocidadX*TimeFrame*-1;
				WalkAnim(Juego->jugador);
			}
			else {
				Juego->jugador->x += Juego->jugador->velocidadX*TimeFrame*2*-1;
				RunAnim(Juego->jugador);
			}
		}
	}

	if(d3 && !d1 && !d2)
		Juego->jugador->curframe = 13;

}

void InicializarZombie(TJUEGO* Juego)
{
	SPRITE* zombie = new SPRITE();
	
	zombie->alive = true;

	strcpy(zombie->nombre, "zombie");
	zombie->width = ZOMBIEW;
	zombie->height = ZOMBIEH;
	zombie->x = zombie->posXInicial = Juego->zombiePos;
	zombie->y = zombie->posYInicial = 0;
	if(rand()%3 < 2)//2/3 probabilidad de mirar hacia la izq.
		zombie->sentido = false;//Mirando hacia la izquierda
	else zombie->sentido = true;
	zombie->tiempoDisparo = clock();
	zombie->tiempoMov = clock();
	zombie->tiempoAnim = clock();
	zombie->tiempoSalto = clock();
	zombie->tiempoSufrimiento = clock();
	zombie->hp = 1;
	zombie->curframe = 0;
	zombie->velocidadX = 100.0/1000.0;//100pps
	zombie->velocidadXInicial = 0;
	zombie->velocidadY = zombie->velocidadYInicial = 0;
	zombie->estado = Ataque;
	zombie->scream = load_sample("Mudo.wav");
	Juego->zombiePos += 1000;
	Juego->caracteres.push_back(zombie);
}
void ActualizarZombie(TJUEGO* Juego)
{
	if(Juego->caracteres.empty()) return;
	int jump = rand()%2;
	 list<SPRITE*>::iterator it;
	 SPRITE* zombieActual;

	 //Actualizar todos los enemigos que estisten
	 for ( it=Juego->caracteres.begin() ; it != Juego->caracteres.end(); it++ ){
		 zombieActual = *it;
		double TimeFrame = clock() - zombieActual->tiempoMov;
		zombieActual->tiempoMov += TimeFrame;
	

		//Buscar colision con el jugador
		if(ExisteColision(zombieActual, Juego->jugador, 0) && clock() - Juego->jugador->tiempoSufrimiento>1000){
			Juego->jugador->hp--;
			Juego->jugador->tiempoSufrimiento = clock();
			play_sample(Juego->jugador->scream, 128, 128, 1000, FALSE);
		}
		
		 //Buscar colisiones con el piso
		if((zombieActual->ExisteColisionPiso=BuscarColisionMundo(Juego, zombieActual, zombieActual->x+(TILEW/2), zombieActual->y+zombieActual->height, true, false) == false) && (zombieActual->ExisteColisionPiso=BuscarColisionMundo(Juego, zombieActual, zombieActual->x+zombieActual->width-(TILEW/2), zombieActual->y+zombieActual->height, true, false)) == false)
		{
			AplicarVelocidad(zombieActual, zombieActual->tiempoSalto, false, true);
		}
		else{//Si esta pisando algo
			zombieActual->ExisteColisionPiso = true;
			zombieActual->posYInicial = zombieActual->y - TILEH/4;
			zombieActual->tiempoSalto = clock();
			zombieActual->velocidadY=0;
			/*if(jump){
				zombieActual->tiempoSalto = clock();
				zombieActual->jumptype = 0;
				zombieActual->y -= TILEH/4;
				zombieActual->velocidadY = -300.0/1000.0;//220 pixeles/segundo
			}*/
		
		}
		bool colisionPared=false;
		//Buscar colisiones Oeste
		if(!zombieActual->sentido && (colisionPared = BuscarColisionMundo(Juego, zombieActual, zombieActual->x+5, zombieActual->y+zombieActual->height-TILEH/2, false, false)) == false){

				zombieActual->x += zombieActual->velocidadX*TimeFrame*-1;
				ZombieAnim(zombieActual);
		}
		else if(!zombieActual->sentido && colisionPared){
			zombieActual->sentido = true;
		}
		
		//Buscar colisiones Este
		if(zombieActual->sentido && (colisionPared = BuscarColisionMundo(Juego, zombieActual, zombieActual->x+zombieActual->width-5, zombieActual->y+zombieActual->height-TILEH/2, false, false)) == false){
				
				zombieActual->x += zombieActual->velocidadX*TimeFrame;
				ZombieAnim(zombieActual);
		}
		else if(zombieActual->sentido && colisionPared){
			zombieActual->sentido = false;
		}


		 if(zombieActual->y > ALTOVIRTUAL || zombieActual->y < 0 || zombieActual->x > ANCHOVIRTUAL || zombieActual->x < 0){
			 Juego->caracteres.remove(zombieActual);
			 return;
		 }
	 
	 }

	

}

void WalkAnim(SPRITE* caracter)
{
	//Si esta en el aire no puede caminar
	if(caracter->ExisteColisionPiso == false)
		return;

	if(clock()-caracter->tiempoAnim > 150){
		caracter->curframe++;
		caracter->tiempoAnim = clock();
	}

	if(caracter->curframe > 4)
		caracter->curframe = 0;
}


void RunAnim(SPRITE* caracter){
		//Si esta en el aire no puede correr
	if(caracter->ExisteColisionPiso == false)
		return;

	if(clock()-caracter->tiempoAnim > 60){
		if(caracter->curframe<5) caracter->curframe = 4;
		caracter->curframe++;
		caracter->tiempoAnim = clock();
	}

	if(caracter->curframe > 12)
		caracter->curframe = 0;

}

void JumpAnim(SPRITE* caracter){
	if(clock()-caracter->tiempoAnim > 100){
		if(caracter->curframe<14) caracter->curframe = 13;
		caracter->curframe++;
		caracter->tiempoAnim = clock();
	}

	if(caracter->curframe > 17)
		caracter->curframe = 14;

}

void SpecialJumpAnim(SPRITE* caracter){
	if(clock()-caracter->tiempoAnim > 100){
		if(caracter->curframe<18) caracter->curframe = 17;
		caracter->curframe++;
		caracter->tiempoAnim = clock();
	}

	if(caracter->curframe > 23)
		caracter->curframe = 18;

}

void ZombieAnim(SPRITE* caracter)
{
	//Si esta en el aire no puede caminar
	if(caracter->ExisteColisionPiso == false)
		return;

	if(clock()-caracter->tiempoAnim > 100){
		caracter->curframe++;
		caracter->tiempoAnim = clock();
	}

	if(caracter->curframe > 6)
		caracter->curframe = 0;
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

void CargarNivel(TJUEGO* Juego)
{
	//A cada vez que se carga un nivel, borrar cualquier niveles anteriores de la memoria que ha sido cargado
	if(!Juego->levels.empty()){
		Juego->levels.clear();
	}

	char temp[20];
	short level_arr[32000];
	sprintf(temp, "level%d.txt", Juego->cur_level);

	ifstream level;
	level.open (temp);

	//Si no se pudo cargar el nivel, salir del juego
	if(level.fail()){
		exit(-10);
	}

	int ArrPos=0;
	for (int posY=0; posY<ALTOVIRTUAL; posY+=TILEH){
		for(int posX=0; posX<ANCHOVIRTUAL; posX+=TILEW){
				level >> level_arr[ArrPos];
				if(level_arr[ArrPos] > 0 && level_arr[ArrPos] <= 64){//Los 0 en el arreglo corresponden al fondo
					//Empezando con el frame 0 en la lista de bitmap que es igual a 1 en el archivo XML
					//textprintf(screen, font, 0+TILEW, 0+TILEW, WHITE, "hi %d", Juego->levels[0][ArrPos]-1);
					//rest(100);
					drawframe(Juego->level_bmp_sheet[Juego->cur_level], Juego->bufferVirtual, posX, posY, TILEW, TILEH, 0, 0, 8, level_arr[ArrPos]-1);
				}
			ArrPos++;
		}
	}


	level.close();

Juego->levels.push_back(level_arr);

}


void drawframe(BITMAP *source, BITMAP *dest,
               int x, int y, int width, int height,
               int startx, int starty, int columns, int frame)
{
    //calculate frame position
    int framex = startx + (frame % columns) * width;
    int framey = starty + (frame / columns) * height;
    //draw frame to destination bitmap
   
	masked_blit(source,dest,framex,framey,x,y,width,height);
}

bool BuscarColisionMundo(TJUEGO* Juego, SPRITE* caracter, int x, int y, bool piso, bool techo)
{ 

	int block = MapBlockValue(x, y, TILEW, TILEH, ANCHOVIRTUAL/TILEW);
	if(block >= (ANCHOVIRTUAL/TILEW)*(ALTOVIRTUAL/TILEH)) return false;

	if(Juego->levels[0][block]>=9 && Juego->levels[0][block]<=49){
		if(piso)
			caracter->y -= y%TILEH;//Le restamos la cantidad por la cual nos pasamos
		if(techo)
			caracter->y += y%TILEH;
		if(Juego->levels[0][block]>=44 && Juego->levels[0][block]<=50 && clock()-caracter->tiempoSufrimiento > 1000){
			caracter->hp--;
			caracter->tiempoSufrimiento = clock();
			play_sample(caracter->scream, 128, 128, 1000, FALSE);	
		}
		return true;
	}
	return false;
}

int MapBlockValue(int x, int y, int width, int height, int columns)
{
	int xblock = x/width;
	int yblock = columns*(y/height);
	return xblock+yblock;
}


void DibujarParticulas(TJUEGO* Juego)
{

	for(int i=0; i<MAXPARTICLES; i++){
		circlefill(Juego->bufferVirtual, particulas[i].x, particulas[i].y, PARTICLE_RAD, particulas[i].color);
	}
}
void ActualizarParticulas(TJUEGO* Juego){
	int centroX = Juego->jugador->x+(Juego->jugador->width/2);
	int centroY = Juego->jugador->y;

	for(int i=0; i<MAXPARTICLES; i++){
		//Attraccion(&particulas[i], Juego);
		//if(abs(centroX - particulas[i].x) >70 || abs(centroY - particulas[i].y)>70){
			InicializarParticulas(Juego);
		//}

	}

	
	

}

void Attraccion(particle* A, TJUEGO* Juego)
{
  
	int centroX = Juego->jugador->x+(Juego->jugador->width/2);
	int centroY = Juego->jugador->y;

	bool VXPos = false;
	bool VYPos = false;

	if(abs(centroX-A->x) > 150)
		A->vx *= -1;

	if(abs(centroX-A->y) > 150)
		A->vy *=-1;
	
	if(A->vx >0) VXPos = true;
	if(A->vy > 0) VYPos = true;

	AplicarVelocidadParticulas(A, A->tiempo, VXPos, VYPos);

}

void AplicarVelocidadParticulas(particle* particula, float tiempo, bool VXPos, bool VYPos)
{
	double tiempoTranscurido;
	tiempoTranscurido = (clock() - tiempo);

		if(!VXPos)
			particula->x = particula->xViejo + ((double)particula->vx*(tiempoTranscurido)) + (0.5*ACEL_PARTICULA*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);
		else{
			particula->x = particula->xViejo - ((double)particula->vx*(tiempoTranscurido)) + (-1.0*0.5*ACEL_PARTICULA*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);
		}

		if(!VYPos)
			particula->y = particula->yViejo + ((double)particula->vy*(tiempoTranscurido)) + (0.5*ACEL_PARTICULA*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);
		else{
			particula->y = particula->yViejo + ((double)particula->vy*(tiempoTranscurido)) + (-1.0*0.5*ACEL_PARTICULA*PPM*(tiempoTranscurido*tiempoTranscurido))/(1000.0*1000.0);
		}

}

void InicializarParticulas(TJUEGO* Juego)
{
	int red, green, blue;
	int negX, negY;
	int centroX = Juego->jugador->x+(Juego->jugador->width/2);
	int centroY = Juego->jugador->y;


	for(int i=0; i<MAXPARTICLES; i++){
		red=rand()%255;
		green=rand()%255; 
		blue=rand()%255;
		negX = rand()%2;
		negY = rand()%2;
		particulas[i].mass = MASS_P;
		particulas[i].ax = 0;
		particulas[i].ay = 0;
		particulas[i].x = centroX;
		particulas[i].y = particulas[i].yViejo = centroY;
		particulas[i].vx = 150.0/1000.0;
		particulas[i].vy = 150.0/1000.0;
		particulas[i].color = makecol(red, green, blue);
		particulas[i].tiempo = clock();
		if(negX) {
			particulas[i].xViejo = particulas[i].x  += rand()%30;
			particulas[i].vx *= -1;
		}
		else particulas[i].xViejo = particulas[i].x -= rand()%30;
		if(negY) {
			particulas[i].yViejo = particulas[i].y += rand()%30;
			particulas[i].vy *= -1;
		}
		else particulas[i].yViejo = particulas[i].y -= rand()%30;
	}
}

void MenuEleccion(TJUEGO* Juego)
{
	//si estamos en el menu entonces nos movemos con las teclas arriba y abajo
	if(Juego->estado == Inicio)
	{
		if(key[KEY_UP])
			option--;

		if(key[KEY_DOWN])
			option++;

		if(option>1){
			option=0;
		}
		
		else if(option<0){
			option=1;
		}

		if(key[KEY_ENTER])
		{
			
			if(option == 0)
			{
				//Si elegimos la opcion 1 el juego entra en el Menu de los Jugadores
				//Llenar el bufferVirtual con el fondo
				masked_blit(Juego->fondo, Juego->bufferVirtual, 0, 0, 0, 0, ANCHOVIRTUAL, ALTOVIRTUAL);
				Juego->estado = EnProceso;
				Juego->tiempo_inicio = 120;
				Juego->tiempo_transcurido = clock();
				play_midi(Juego->music[1], 0);
				Juego->x = 0;
				Juego->y = SCREEN_H;
				Juego->score = 0;
				Juego->zombiePos = 1000;
				Juego->cur_level = 0;
				CargarNivel(Juego);
				draw_sprite(Juego->fondo, Juego->bufferVirtual, 0, 0);
				Juego->jugador->tiempoSalto = clock();
				Juego->jugador->y = SCREEN_H;
				Juego->jugador->velocidadYInicial = Juego->jugador->velocidadY = 0;
				Juego->jugador->x = 0;
				Juego->jugador->hp = 3;
				if(!Juego->caracteres.empty()) Juego->caracteres.clear();

			}

			if(option == 1)
			{
				//Si elegimos la opcion 2 el juego entra en las opciones para elegir los cartones
				Juego->estado = Opciones;
			//	clear_to_color(Juego->bufferVirtual, makecol(0,0,0));
			}

			rest(100);
		}
	}

	//Menu Opciones
	if(Juego->estado == Opciones)
	{
		if(key[KEY_UP])
			option2--;

		if(key[KEY_DOWN])
			option2++;

		if(key[KEY_ENTER])
		{
			
			if(option2 == 0)
			{
				strcpy(Juego->jugador->nombre, "mario");
				Juego->estado = Inicio;
				//clear_to_color(Juego->bufferVirtual, makecol(0,0,0));
			}

			if(option2 == 1)
			{
				strcpy(Juego->jugador->nombre, "luiggi");
				Juego->estado = Inicio;
				//clear_to_color(Juego->bufferVirtual, makecol(0,0,0));
			}

		
			rest(100);
		}
	} 
		
	//Si presionamos la tecla backspace en el menu, regresamos al menu principal
	if(key[KEY_BACKSPACE])
		{
			if(Juego->estado == Opciones)
			{
				Juego->estado = Inicio;
			//	clear_to_color(Juego->bufferVirtual, makecol(0,0,0));
			}
		}

	//Condiciones para elegir en el menu y asi poder renderizar correctamente
	if(option < 0)
		option = 0;
	else if(option > 1)
		option = 1;

	if(option2 < 0)
		option2 = 0;
	else if(option2 > 1)
		option2 = 1;

	rest(100);
}

/*
	Esta Funcion es la encargada de darle vida al menu inicial
	Aqui se hace todo lo visual referente al Menu.
	Dependiendo en que estado este el juego esta imprime las opciones 
	correspondientes.
*/

void MenuRenderizado(TJUEGO* Juego)
{
	
	//Dibujar fondo menu
	draw_sprite(Juego->bufferVirtual, Juego->single_frame_bmp[0], 0,0);


	if(Juego->estado == Inicio)
	{
	for(int i = 0; i < 2; i++)
		textout_centre_ex(Juego->bufferVirtual, font, Options[i].c_str(), ANCHO / 2, (i*40) + 412, makecol(255, 255, 255), -1);
		textout_centre_ex(Juego->bufferVirtual, font, Options[option].c_str(), ANCHO / 2, (option*40) + 412, makecol(255, 255, 0), -1);
	}

	if(Juego->estado == Opciones)
	{
	for(int i = 0; i < 2; i++)
		textout_centre_ex(Juego->bufferVirtual, font, Options2[i].c_str(), ANCHO / 2, (i*30) + 415, makecol(255, 255, 255), -1);
		textout_centre_ex(Juego->bufferVirtual, font, Options2[option2].c_str(), ANCHO / 2, (option2*30) + 415, makecol(255, 255, 0), -1);
	}
	
	acquire_screen();
	blit(Juego->bufferVirtual, screen, 0, 0, 0, 0, ANCHO, ALTO);	
	release_screen();
	
}

void GameOverScreen(TJUEGO* Juego)
{
	draw_sprite(Juego->bufferVirtual, Juego->single_frame_bmp[1], Juego->x+100, Juego->y);
	stop_midi();
	play_sample(Juego->game_over, 128, 128, 1000, FALSE);	

	acquire_screen();
	blit(Juego->bufferVirtual, screen, Juego->x, Juego->y, 0, 0, Juego->single_frame_bmp[1]->w, Juego->single_frame_bmp[1]->h);
	release_screen();
	rest(5000);
	Juego->estado = Inicio;
	//exit(INT_MAX);
}

void NextLevel(TJUEGO* Juego)
{
	Juego->cur_level++;
	clear_bitmap(Juego->bufferVirtual);
	Juego->fondo = load_bitmap("bg3.bmp", NULL);
	draw_sprite(Juego->bufferVirtual, Juego->fondo, 0, 0);
	CargarNivel(Juego);
	play_midi(Juego->music[Juego->cur_level+1], 0);
	draw_sprite(Juego->fondo, Juego->bufferVirtual, 0, 0);
	Juego->jugador->x=0;
	Juego->jugador->y =0;
	Juego->x = 0;
	Juego->y = 0;
	Juego->jugador->tiempoSalto = clock();
	Juego->score += Juego->tiempo_inicio;
	Juego->tiempo_inicio = 120;
	Juego->tiempo_transcurido = clock();
	Juego->zombiePos = 1000;
	Juego->caracteres.clear();

}

bool ExisteColision(SPRITE* primero, SPRITE* segundo, int esquina){

	  //retornar pos + ancho/alto de ambos sprites
  int width1 = primero->x + primero->width;
  int height1 = primero->y + primero->height;
  int width2 = segundo->x + segundo->width;
  int height2 = segundo->y + segundo->height;

  //ver si la esquina del primero esta dentro del segundo
  if (inside(primero->x, primero->y, segundo->x + esquina, 
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