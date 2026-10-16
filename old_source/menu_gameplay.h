#pragma once
#include <citro2d.h>
#include "gameplay.h"

typedef struct{
    int x;
    int y;
} Vector2;

extern Vector2 botonSeleccionado;


Vector2 idAPosicion(int id);
int posicionAid(Vector2 posicion);
u32 colorBoton(Vector2 posicion, int id, config configPartida, bool mostrarImpostores);
void crearMenuGameplayArriba(config configPartida, bool mostrarImpostores);