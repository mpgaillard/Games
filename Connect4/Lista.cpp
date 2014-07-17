#include "StdAfx.h"
#include "Lista.h"

TLISTA* InicializarLista(TLISTA* Nueva)
{
	Nueva->cabeza = new TNODO();
	Nueva->cantidad = 0;

	return Nueva;
}

TLISTA* InsertarNodoCabeza(TLISTA* Lista, TNODO* Nodo)
{
	Nodo->next = Lista->cabeza;

	Lista->cabeza = Nodo;

	Lista->cantidad++;

	return Lista;
}

TLISTA* InsertarNodoCola(TLISTA* Lista, TNODO* Nodo)
{
	TNODO* Actual;
	for(Actual = Lista->cabeza; Actual->next != NULL; Actual = Actual->next);

	Actual->next = Nodo;

	Lista->cantidad++;

	return Lista;
}

TLISTA* CrearNodo(TLISTA* Lista, void* data, bool EnCola)
{
	TNODO* Nuevo = new TNODO();
	Nuevo->data = data;
	Nuevo->next = NULL;

	if(EnCola)
		return InsertarNodoCola(Lista, Nuevo);
	else
		return InsertarNodoCabeza(Lista, Nuevo);
}

TNODO* BuscarNodo(TLISTA* Lista, void* data)
{
	TNODO* Actual;
	for(Actual = Lista->cabeza; Actual->data != data; Actual = Actual->next);

	if(Actual->data == data)
		return Actual;

	return NULL;
}

TNODO* BuscarNodoIndice(TLISTA* Lista, int indice)
{
	TNODO* Actual;
	int i=0;
	for(Actual = Lista->cabeza->next; i++ != indice && Actual != NULL; Actual = Actual->next);

	return Actual;
}

TLISTA* EliminarNodo(TLISTA* Lista, void* data)
{
	//- 10 para Mercy.
	TNODO* Actual = Lista->cabeza;

	if(Lista->cabeza->data != data)
		for(; Actual->next != NULL && Actual->next->data != data; Actual = Actual->next);

	if(Actual->next == NULL)
		return NULL;

	// Hemos encontrado el nodo...
	TNODO* bubble;

	if(Actual == Lista->cabeza)
	{
		bubble = Actual;
		Lista->cabeza = Actual->next;
	}
	else
	{
		bubble = Actual->next;
		Actual->next = Actual->next->next;
	}

	delete bubble;

	Lista->cantidad--;

	return Lista;
}

TLISTA* VaciarLista(TLISTA* Lista)
{
	TNODO* Actual = Lista->cabeza;

	while(Lista->cabeza != NULL)
	{
		EliminarNodo(Lista, Lista->cabeza->data);
	}

	return Lista;
}