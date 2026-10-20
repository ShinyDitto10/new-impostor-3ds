#include "ui.h"
#include "main.h"
#include "gameplay.h"
#include "guardado.h"

static string num_impostores_str = "1";
static u8 nombre_seleccionado = 0;

static void anadir_jugador(){
    if(partida.num_jugadores < MAX_JUGADORES) partida.num_jugadores++;
    if(partida.nombre_jugadores[partida.num_jugadores - 1] == ""){
        partida.nombre_jugadores[partida.num_jugadores - 1] = "Jugador " + std::to_string(partida.num_jugadores);
    }
}

static void quitar_jugador(){
    if(partida.num_jugadores > MIN_JUGADORES){
        if(partida.num_jugadores <= partida.num_impostores + 2) partida.num_impostores--;
        if(nombre_seleccionado >= partida.num_jugadores - 1) nombre_seleccionado--;
        partida.num_jugadores--;
    }
}

static void anadir_impostor(){
    if(partida.num_jugadores > partida.num_impostores + 2) partida.num_impostores++;
}

static void quitar_impostor(){
    if(partida.num_impostores > 1) partida.num_impostores--;
}

static void activar_pista(){
    partida.hay_pista = true;
}

static void desactivar_pista(){
    partida.hay_pista = false;
}

static void activar_se_conocen(){
    partida.impostores_se_conocen = true;
}

static void desactivar_se_conocen(){
    partida.impostores_se_conocen = false;
}

static void comenzar_partida(){
    partida.escoger_palabras();
    partida.escoger_impostores();
    menu_activo = Menu::PALABRAS;
}

static void cambiar_nombre(int jugador){
    SwkbdState swkbd{};
    SwkbdButton boton_ok{};
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, 15);
    swkbdSetHintText(&swkbd, "Escribe tu nombre");
    swkbdSetButton(&swkbd, boton_ok, "Aceptar", true);
    char buffer[16];
    swkbdInputText(&swkbd, buffer, 16);
    partida.nombre_jugadores[jugador] = buffer;
}

static void eliminar_jugador(int jugador){
    if(partida.num_jugadores <= MIN_JUGADORES) return;
    for(int i = jugador; i < partida.num_jugadores; i++){
        partida.nombre_jugadores[i] = partida.nombre_jugadores[i + 1];
    } quitar_jugador();
}

void loop_menu_config(){
    static vector<Elemento*> menu_config_arriba = {
        new Texto(200, 6, 1, "Jugadores", C2D_AlignCenter),
        new Rect(0, 215, 400, 25, (u32)Color::OSCURO),
        new Texto(200, 220, 0.5, "\uE006 Navegar  \uE000 Cambiar nombre  \uE001 Volver  \uE002 Eliminar", C2D_AlignCenter)
    };

    //Para prevenir un memory leak MASIVO
    static bool menu_abierto = false;
    if(!menu_abierto){
        for(auto i : menu_nombres_gameplay){
            menu_config_arriba.push_back(i);
        }
        menu_abierto = true;
    }

    static vector<Elemento*> menu_config_abajo = {
        new Rect(0, 0, 320, 25, (u32)Color::OSCURO),
        new Texto(160, 5, 0.5, "\uE004 Guardar config. \uE005 Cargar config.", C2D_AlignCenter),

        new Boton(15, 35, 139, 50, "Añadir\nJugador", anadir_jugador, 0.75),
        new Boton(166, 35, 139, 50, "Quitar\nJugador", quitar_jugador, 0.75),

        new Rect(15, 95, 290, 25, (u32)Color::AZUL),
        new Texto(20, 100, 0.6, "Número de impostores", 0),
        new Boton(235, 97, 21, 21, "-", quitar_impostor, 0.6),
        new Boton(279, 97, 21, 21, "+", anadir_impostor, 0.6),
        new TextoDinamico(265, 100, 0.6, num_impostores_str, 0),

        new Rect(15, 130, 290, 25, (u32)Color::AZUL),
        new Texto(20, 135, 0.6, "Pista para los impostores", 0),
        new Checklist<2>({
            Checkbox(235, 132, 30, 21, "Sí", activar_pista, 0.6),
            Checkbox(270, 132, 30, 21, "No", desactivar_pista, 0.6)
        }),

        new Rect(15, 165, 290, 25, (u32)Color::AZUL),
        new Texto(20, 170, 0.6, "Los impostores se conocen", 0),
        new Checklist<2>({
            Checkbox(235, 167, 30, 21, "Sí", activar_se_conocen, 0.6),
            Checkbox(270, 167 , 30, 21, "No", desactivar_se_conocen, 0.6)
        }, 1),

        new Boton(100, 200, 120, 35, "Empezar partida", comenzar_partida, 0.6)
    };

    if(kDown & KEY_B) menu_activo = Menu::PRINCIPAL;
    if(kDown & KEY_DLEFT && nombre_seleccionado % 3 != 0) nombre_seleccionado--;
    if(kDown & KEY_DRIGHT && nombre_seleccionado % 3 != 2 && nombre_seleccionado < partida.num_jugadores - 1) nombre_seleccionado++;
    if(kDown & KEY_DUP && nombre_seleccionado / 3 != 0) nombre_seleccionado -= 3;
    if(kDown & KEY_DDOWN && nombre_seleccionado < partida.num_jugadores - 3) nombre_seleccionado += 3;

    for(int i = 0; i < MAX_JUGADORES; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);

        if(i >= partida.num_jugadores){
            r->color = (u32)Color::TRANS;
            t->color = (u32)Color::TRANS;
        }
        else if(i == nombre_seleccionado){
            r->color = (u32)Color::AZUL_CLARO;
            t->color = (u32)Color::BLANCO;
        }
        else{
            r->color = (u32)Color::AZUL;
            t->color = (u32)Color::BLANCO;
        }
    }

    if(kDown & KEY_A && nombre_seleccionado < partida.num_jugadores) cambiar_nombre(nombre_seleccionado);
    else if(kDown & KEY_X && nombre_seleccionado < partida.num_jugadores) eliminar_jugador(nombre_seleccionado);
    else if(kDown & KEY_L) guardar_config_partida(partida, 0);
    else if(kDown & KEY_R){
        cargar_config_partida(partida, 0);
        Checklist<2>* c = (Checklist<2>*)menu_config_abajo[11];
        c->caja_activa = !partida.hay_pista;
        c = (Checklist<2>*)menu_config_abajo[14];
        c->caja_activa = !partida.impostores_se_conocen;
    }

    num_impostores_str = std::to_string(partida.num_impostores);
    renderizar_menu(menu_config_arriba, menu_config_abajo);
}