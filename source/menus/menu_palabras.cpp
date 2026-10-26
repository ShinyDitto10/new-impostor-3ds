#include "menus.h"
#include "gameplay.h"
#include <algorithm>

static string texto_tarjeta = "";
static bool viendo_palabra = false;
static bool palabra_vista = false;
static vector<int> jugadores_visto_palabra;
static int nombre_seleccionado = 0;
static string texto_arriba = "Escoge tu nombre de la lista:";

void mostrar_palabra(){
    palabra_vista = true;
    menu_abajo[2]->activo = true;
    menu_abajo[1]->activo = true;
    menu_abajo[0]->activo = false;
    TextoDinamico* tarjeta = (TextoDinamico*)menu_abajo[3];

    if(partida.jugador_es_impostor(nombre_seleccionado)){
        tarjeta->color = (u32)Color::ROJO;
        texto_tarjeta = "IMPOSTOR";
        if(partida.hay_pista) texto_tarjeta += "\nPista: " + partida.pista_impostor;
        if(partida.impostores_se_conocen) texto_arriba = "Los impostores son:";
    } else{
        tarjeta->color = (u32)Color::VERDE;
        texto_tarjeta = partida.palabra_jugadores;
    }
}

void siguiente_jugador(){
    palabra_vista = false;
    viendo_palabra = false;
    jugadores_visto_palabra.push_back(nombre_seleccionado);
    texto_tarjeta = "";
    texto_arriba = "Escoge tu nombre de la lista:";

    menu_abajo[0]->activo = false;
    menu_abajo[1]->activo = false;
    menu_abajo[2]->activo = false;

    if(jugadores_visto_palabra.size() == partida.num_jugadores){
        jugadores_visto_palabra.clear();
        nombre_seleccionado = 0;
        partida.escoger_primer_jugador();
        partida.empezar_crono();
        init_menu(Menu::PARTIDA);
    }
}

void init_menu_palabras(){
    menu_arriba.assign({
        new TextoDinamico(200, 10, 0.75, texto_arriba, C2D_AlignCenter),
        new Rect(0, 215, 400, 25, (u32)Color::OSCURO),
        new Texto(200, 220, 0.5, "\uE006 Navegar  \uE000 Elegir nombre", C2D_AlignCenter)
    });

    menu_abajo.assign({
        new Boton(40, 40, 240, 150, "Pulsa para ver\ntu palabra", mostrar_palabra, 1),
        new Boton(70, 200, 180, 35, "Siguiente jugador", siguiente_jugador, 0.6),
        new Rect(40, 40, 240, 150, (u32)Color::AZUL),
        new TextoDinamico(160, 80, 1, texto_tarjeta, C2D_AlignCenter)
    });

    menu_abajo[0]->activo = false;
    menu_abajo[1]->activo = false;
    menu_abajo[2]->activo = false;
}

void loop_menu_palabras(){
    //Navegar el menu
    if(!viendo_palabra && menu_activo == Menu::PALABRAS){
        if(kDown & KEY_DLEFT && nombre_seleccionado % 3 != 0) nombre_seleccionado--;
        if(kDown & KEY_DRIGHT && nombre_seleccionado % 3 != 2 && nombre_seleccionado < partida.num_jugadores - 1) nombre_seleccionado++;
        if(kDown & KEY_DUP && nombre_seleccionado / 3 != 0) nombre_seleccionado -= 3;
        if(kDown & KEY_DDOWN && nombre_seleccionado < partida.num_jugadores - 3) nombre_seleccionado += 3;
        if(kDown & KEY_A && std::find(jugadores_visto_palabra.begin(), jugadores_visto_palabra.end(), nombre_seleccionado) == jugadores_visto_palabra.end()){
            viendo_palabra = nombre_seleccionado + 1;
            menu_abajo[0]->activo = true;
        }
    }

    for(int i = 0; i < MAX_JUGADORES; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);

        if(i >= partida.num_jugadores){
            r->color = (u32)Color::TRANS;
            t->color = (u32)Color::TRANS;
        } else if(i == nombre_seleccionado){
            r->color = (u32)Color::AZUL_CLARO;
            t->color = (u32)Color::BLANCO;
        } else if(palabra_vista && partida.impostores_se_conocen && partida.jugador_es_impostor(nombre_seleccionado) && partida.jugador_es_impostor(i)){
            r->color = (u32)Color::AZUL_CLARO;
            t->color = (u32)Color::BLANCO;
        } else{
            r->color = (u32)Color::AZUL;
            t->color = (u32)Color::BLANCO;
        }
    }

    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_menu_nombres();
}