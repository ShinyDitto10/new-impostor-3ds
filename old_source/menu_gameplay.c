#include "menu_gameplay.h"
#include "gameplay.h"

Vector2 botonSeleccionado = {0, 0};

Vector2 idAPosicion(int id) {
    int fila = id / 3;
    int columna = id % 3;
    Vector2 posicion = { columna, fila };
    return posicion;
}

int posicionAid(Vector2 posicion){
    return 3 * posicion.y + posicion.x;
}

u32 colorBoton(Vector2 posicion, int id, config configPartida, bool mostrarImpostores) {
    if(mostrarImpostores && comprobarImpostor(id, configPartida.numImpostores)){
        return C2D_Color32(0x80, 0x80, 0xFF, 0xFF);
    } else{
        if(configPartida.numJugadores - 1 < id) return C2D_Color32(0x00, 0x00, 0x80, 0xFF);
        else if(posicionAid(posicion) == id) return C2D_Color32(0x80, 0x80, 0xFF, 0xFF);
        else return C2D_Color32(0x30, 0x30, 0xFF, 0xFF);
    }
}

void crearMenuGameplayArriba(config configPartida, bool mostrarImpostores){
    C2D_DrawRectSolid(14, 45, 0.0f, 118, 37, colorBoton(botonSeleccionado, 0, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(139, 45, 0.0f, 118, 37, colorBoton(botonSeleccionado, 1, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(264, 45, 0.0f, 118, 37, colorBoton(botonSeleccionado, 2, configPartida, mostrarImpostores));

	C2D_DrawRectSolid(14, 89, 0.0f, 118, 37, colorBoton(botonSeleccionado, 3, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(139, 89, 0.0f, 118, 37, colorBoton(botonSeleccionado, 4, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(264, 89, 0.0f, 118, 37, colorBoton(botonSeleccionado, 5, configPartida, mostrarImpostores));

	C2D_DrawRectSolid(14, 133, 0.0f, 118, 37, colorBoton(botonSeleccionado, 6, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(139, 133, 0.0f, 118, 37, colorBoton(botonSeleccionado, 7, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(264, 133, 0.0f, 118, 37, colorBoton(botonSeleccionado, 8, configPartida, mostrarImpostores));

	C2D_DrawRectSolid(14, 177, 0.0f, 118, 37, colorBoton(botonSeleccionado, 9, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(139, 177, 0.0f, 118, 37, colorBoton(botonSeleccionado, 10, configPartida, mostrarImpostores));
	C2D_DrawRectSolid(264, 177, 0.0f, 118, 37, colorBoton(botonSeleccionado, 11, configPartida, mostrarImpostores));
}