#include "menus.h"
#include "gameplay.h"
#include "guardado.h"

static string num_impostores_str = "1";
static u8 nombre_seleccionado = 0;
static bool menu_guardar = false;
static bool menu_cargar = false;
static bool toca_cargar = false;

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
    init_menu(Menu::PALABRAS);
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

static void boton_slot(int slot){
    if(menu_cargar){
        cargar_config_partida(partida, slot);
        menu_cargar = false;
        toca_cargar = true;
        if(partida.num_jugadores <= nombre_seleccionado) nombre_seleccionado = partida.num_jugadores - 1;
    } else if(menu_guardar){
        guardar_config_partida(partida, slot);
        menu_guardar = false;
    }
}

void init_menu_config(){
    menu_arriba.assign({
        new Texto(200, 6, 1, "Jugadores", C2D_AlignCenter),
        new Rect(0, 215, 400, 25, (u32)Color::OSCURO),
        new Texto(200, 220, 0.5, "\uE006 Navegar  \uE000 Cambiar nombre  \uE001 Volver  \uE002 Eliminar", C2D_AlignCenter)
    });

    menu_abajo.assign({
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

        new Boton(100, 200, 120, 35, "Empezar partida", comenzar_partida, 0.6),

        new Rect(20, 30, 280, 200, (u32)Color::AZUL),
        new Rect(30, 40, 260, 180, (u32)Color::BG),
        new Texto(160, 50, 0.6, "Elige un archivo", C2D_AlignCenter),
        new Boton(50, 80, 105, 33, "Archivo 1", [](){boton_slot(0);}, 0.5),
        new Boton(165, 80, 105, 33, "Archivo 2", [](){boton_slot(1);}, 0.5),
        new Boton(50, 123, 105, 33, "Archivo 3", [](){boton_slot(2);}, 0.5),
        new Boton(165, 123, 105, 33, "Archivo 4", [](){boton_slot(3);}, 0.5),
        new Boton(50, 166, 105, 33, "Archivo 5", [](){boton_slot(4);}, 0.5),
        new Boton(165, 166, 105, 33, "Archivo 6", [](){boton_slot(5);}, 0.5),
    });
}

void loop_menu_config(){
    if(kDown & KEY_B){
        if(!menu_cargar && !menu_guardar) init_menu(Menu::PRINCIPAL);
        else{
            menu_cargar = false;
            menu_guardar = false;
        }
    }
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
    else if(kDown & KEY_L && !menu_cargar && menu_activo == Menu::CONFIGURACION){
        menu_guardar = true;
        for(int i = 16; i < 25; i++) menu_abajo[i]->activo = true;
    }
    else if(kDown & KEY_R && !menu_guardar && menu_activo == Menu::CONFIGURACION){
        menu_cargar = true;
        for(int i = 16; i < 25; i++) menu_abajo[i]->activo = true;
    }
    if(toca_cargar){
        Checklist<2>* c = (Checklist<2>*)menu_abajo[11];
        c->caja_activa = !partida.hay_pista;
        c = (Checklist<2>*)menu_abajo[14];
        c->caja_activa = !partida.impostores_se_conocen;
        toca_cargar = false;
    }
    if(!menu_cargar && !menu_guardar && menu_activo == Menu::CONFIGURACION) for(int i = 16; i < 25; i++) menu_abajo[i]->activo = false;

    num_impostores_str = std::to_string(partida.num_impostores);
    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_menu_nombres();
}