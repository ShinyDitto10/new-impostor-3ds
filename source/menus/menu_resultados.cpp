#include "ui.h"
#include "main.h"
#include "gameplay.h"

static string palabra_str = "La palabra era ";

void jugar_de_nuevo(){
    partida.escoger_palabras();
    partida.escoger_impostores();
    menu_activo = Menu::PALABRAS;
}

void volver_al_menu(){
    menu_activo = Menu::PRINCIPAL;
}

void loop_menu_resultados(){
    static vector<Elemento*> menu_resultados_arriba = {
        new Texto(200, 6, 0.75, "Los impostores eran:", C2D_AlignCenter)
    };

    //Para prevenir un memory leak MASIVO
    static bool menu_abierto = false;
    if(!menu_abierto){
        for(auto i : menu_nombres_gameplay){
            menu_resultados_arriba.push_back(i);
        }
        menu_abierto = true;
    }

    static vector<Elemento*> menu_resultados_abajo = {
        new TextoDinamico(160, 70, 0.8, palabra_str, C2D_AlignCenter),
        new Boton(70, 130, 180, 35, "Jugar de nuevo", jugar_de_nuevo, 0.6),
        new Boton(70, 175, 180, 35, "Volver al menú", volver_al_menu, 0.6)
    };

    palabra_str = "La palabra era\n" + partida.palabra_jugadores;

    for(int i = 0; i < MAX_JUGADORES; i++){
        Rect* r = (Rect*)menu_nombres_gameplay.at(i);
        TextoDinamico* t = (TextoDinamico*)menu_nombres_gameplay.at(12 + i);

        if(i >= partida.num_jugadores){
            r->color = (u32)Color::TRANS;
            t->color = (u32)Color::TRANS;
        } else if(partida.jugador_es_impostor(i)){
            r->color = (u32)Color::AZUL_CLARO;
            t->color = (u32)Color::BLANCO;
        } else{
            r->color = (u32)Color::AZUL;
            t->color = (u32)Color::BLANCO;
        }
    }

    renderizar_menu(menu_resultados_arriba, menu_resultados_abajo);
}