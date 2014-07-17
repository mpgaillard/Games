#ifndef _LISTA // Estas instrucciones son para agrupar (estilo namespace).
#define _LISTA

#include "stdafx.h"

typedef struct NODO
{
	void* data;
	NODO* next;
}TNODO;

typedef struct LISTA
{
	TNODO* cabeza;
	int cantidad;
}TLISTA;

TLISTA* InicializarLista(TLISTA*);

TLISTA* CrearNodo(TLISTA*, void*, bool);

TLISTA* InsertarNodoCabeza(TLISTA*, TNODO*);

TLISTA* InsertarNodoCola(TLISTA*, TNODO*);

TNODO* BuscarNodo(TLISTA*, void*);

TNODO* BuscarNodoIndice(TLISTA*, int);

TLISTA* EliminarNodo(TLISTA*, void*);

TLISTA* VaciarLista(TLISTA*);

#endif