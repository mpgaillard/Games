#ifndef _PARTICLES // Estas instrucciones son para agrupar (estilo namespace).
#define _PARTICLES

#include "main.h"
#define MAXPARTICLES 10
#define MASS_P 10
#define MASS_JUGADOR 300
#define PARTICLE_RAD 3
#define ACEL_PARTICULA 5

struct particle{
    double mass;
    double x, y;
	double xViejo, yViejo;
    double ax;
    double ay;
    double vx;
    double vy;
	float tiempo;
	int color;
}particulas[MAXPARTICLES];

void DibujarParticulas(TJUEGO*);
void ActualizarParticulas(TJUEGO*);
void InicializarParticulas(TJUEGO*);
void Attraccion(particle*, TJUEGO*);
void AplicarVelocidadParticulas(particle* particula, float tiempo, bool, bool);

#endif