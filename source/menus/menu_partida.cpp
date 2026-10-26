#include "menus.h"
#include "gameplay.h"
#include <iomanip>
#include <sstream>

static string tiempo_ronda_str = "Tiempo: 0:00";

static void terminar_partida(){
    init_menu(Menu::RESULTADOS);
}

void init_menu_partida(){
    string primer_jugador_str = "Empieza " + partida.nombre_jugadores[partida.empieza];

    menu_abajo.assign({
        new Texto(160, 70, 0.8, primer_jugador_str.c_str(), C2D_AlignCenter),
        new TextoDinamico(160, 95, 0.8, tiempo_ronda_str, C2D_AlignCenter),
        new Boton(70, 130, 180, 35, "Terminar partida", terminar_partida, 0.6)
    });
}

void loop_menu_partida(){
    u64 tiempo_actual = osGetTime();
    u64 tiempo_ronda = (tiempo_actual - partida.tiempo_inicial) / 1000;
    u32 min = tiempo_ronda / 60;
    u32 sec = tiempo_ronda % 60;

    std::stringstream sb;
    sb << "Tiempo: " << min << ":" << std::setw(2) << std::setfill('0') << sec;
    tiempo_ronda_str = sb.str();

    for(int i = 0; i < MAX_JUGADORES; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);

        if(i >= partida.num_jugadores){
            r->color = (u32)Color::TRANS;
            t->color = (u32)Color::TRANS;
        } else{
            r->color = (u32)Color::AZUL;
            t->color = (u32)Color::BLANCO;
        }
    }

    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_menu_nombres();
}