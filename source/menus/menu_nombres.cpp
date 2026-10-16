#include "ui.h"
#include "main.h"
#include "gameplay.h"

void crear_menu_nombres(){
    menu_nombres_gameplay = {
        new Rect(14, 45, 118, 37, (u32)Color::AZUL),
        new Rect(139, 45, 118, 37, (u32)Color::AZUL),
        new Rect(264, 45, 118, 37, (u32)Color::AZUL),
        new Rect(14, 89, 118, 37, (u32)Color::AZUL),
        new Rect(139, 89, 118, 37, (u32)Color::AZUL),
        new Rect(264, 89, 118, 37, (u32)Color::AZUL),
        new Rect(14, 133, 118, 37, (u32)Color::AZUL),
        new Rect(139, 133, 118, 37, (u32)Color::AZUL),
        new Rect(264, 133, 118, 37, (u32)Color::AZUL),
        new Rect(14, 177, 118, 37, (u32)Color::AZUL),
        new Rect(139, 177, 118, 37, (u32)Color::AZUL),
        new Rect(264, 177, 118, 37, (u32)Color::AZUL),
        
        new TextoDinamico(73, 50, 0.65, partida.nombre_jugadores[0], C2D_AlignCenter),
        new TextoDinamico(198, 50, 0.65, partida.nombre_jugadores[1], C2D_AlignCenter),
        new TextoDinamico(323, 50, 0.65, partida.nombre_jugadores[2], C2D_AlignCenter),
        new TextoDinamico(73, 94, 0.65, partida.nombre_jugadores[3], C2D_AlignCenter),
        new TextoDinamico(198, 94, 0.65, partida.nombre_jugadores[4], C2D_AlignCenter),
        new TextoDinamico(323, 94, 0.65, partida.nombre_jugadores[5], C2D_AlignCenter),
        new TextoDinamico(73, 138, 0.65, partida.nombre_jugadores[6], C2D_AlignCenter),
        new TextoDinamico(198, 138, 0.65, partida.nombre_jugadores[7], C2D_AlignCenter),
        new TextoDinamico(323, 138, 0.65, partida.nombre_jugadores[8], C2D_AlignCenter),
        new TextoDinamico(73, 182, 0.65, partida.nombre_jugadores[9], C2D_AlignCenter),
        new TextoDinamico(198, 182, 0.65, partida.nombre_jugadores[10], C2D_AlignCenter),
        new TextoDinamico(323, 182, 0.65, partida.nombre_jugadores[11], C2D_AlignCenter),
    };
}

void actualizar_menu_nombres(){
    for(int i = 0; i < MAX_JUGADORES; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        r->color = (u32)Color::TRANS;

        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);
        t->color = (u32)Color::TRANS;
    }
    for(int i = 0; i < partida.num_jugadores; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        r->color = (u32)Color::AZUL;

        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);
        t->color = (u32)Color::BLANCO;
    }
}