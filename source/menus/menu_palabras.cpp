#include "ui.h"
#include "main.h"
#include "gameplay.h"
#include <algorithm>

static string texto_tarjeta = "";
static bool viendo_palabra = false;
static bool palabra_vista = false;
static vector<int> jugadores_visto_palabra;
static int nombre_seleccionado = 0;
static u32 color_tarjeta;

void mostrar_palabra(){
    palabra_vista = true;
    if(partida.jugador_es_impostor(nombre_seleccionado)){
        color_tarjeta = (u32)Color::ROJO;
        texto_tarjeta = "IMPOSTOR";
        if(partida.hay_pista){
            texto_tarjeta += "\nPista: " + partida.pista_impostor;
        }
    } else{
        color_tarjeta = (u32)Color::VERDE;
        texto_tarjeta = partida.palabra_jugadores;
    }
}

void siguiente_jugador(){
    palabra_vista = false;
    viendo_palabra = false;
    jugadores_visto_palabra.push_back(nombre_seleccionado);
    texto_tarjeta = "";

    if(jugadores_visto_palabra.size() == partida.num_jugadores){
        menu_activo = Menu::PARTIDA;
        jugadores_visto_palabra.clear();
        nombre_seleccionado = 0;
        partida.escoger_primer_jugador();
        partida.empezar_crono();
    }
}

void loop_menu_palabras(){
    static vector<Elemento*> menu_palabras_arriba = {
        new Texto(200, 10, 0.75, "Escoge tu nombre de la lista:", C2D_AlignCenter),
        new Texto(200, 220, 0.5, "Cruceta: Navegar  A: Elegir nombre", C2D_AlignCenter)
    };

    //Para prevenir un memory leak MASIVO
    static bool menu_abierto = false;
    if(!menu_abierto){
        for(auto i : menu_nombres_gameplay){
            menu_palabras_arriba.push_back(i);
        }
        menu_abierto = true;
    }

    static vector<Elemento*> menu_palabras_abajo = {
        new Boton(40, 40, 240, 150, "Pulsa para ver\ntu palabra", mostrar_palabra, 1),
        new Boton(70, 200, 180, 35, "Siguiente jugador", siguiente_jugador, 0.6),
        new Rect(40, 40, 240, 150, (u32)Color::AZUL),
        new TextoDinamico(160, 80, 1, texto_tarjeta, C2D_AlignCenter)
    };

    //Navegar el menu
    if(!viendo_palabra){
        menu_palabras_abajo[0]->activo = false;
        menu_palabras_abajo[1]->activo = false;
        menu_palabras_abajo[2]->activo = false;
        if(kDown & KEY_DLEFT && nombre_seleccionado % 3 != 0) nombre_seleccionado--;
        if(kDown & KEY_DRIGHT && nombre_seleccionado % 3 != 2 && nombre_seleccionado < partida.num_jugadores - 1) nombre_seleccionado++;
        if(kDown & KEY_DUP && nombre_seleccionado / 3 != 0) nombre_seleccionado -= 3;
        if(kDown & KEY_DDOWN && nombre_seleccionado < partida.num_jugadores - 3) nombre_seleccionado += 3;
        if(kDown & KEY_A && std::find(jugadores_visto_palabra.begin(), jugadores_visto_palabra.end(), nombre_seleccionado) == jugadores_visto_palabra.end()){
            viendo_palabra = nombre_seleccionado + 1;
            menu_palabras_abajo[0]->activo = true;
        }
    // Palabra ya escogida (toca darle a siguiente jugador)
    } else if(palabra_vista){
        menu_palabras_abajo[2]->activo = true;
        menu_palabras_abajo[1]->activo = true;
        menu_palabras_abajo[0]->activo = false;
        TextoDinamico* tarjeta = (TextoDinamico*)menu_palabras_abajo[3];
        tarjeta->color = color_tarjeta;
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

    //if(kDown & KEY_B) menu_activo = Menu::PRINCIPAL;
    renderizar_menu(menu_palabras_arriba, menu_palabras_abajo);
}