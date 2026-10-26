#include "menus.h"
#include "gameplay.h"

void jugar_de_nuevo(){
    partida.escoger_palabras();
    partida.escoger_impostores();
    init_menu(Menu::PALABRAS);
}

void volver_al_menu(){
    init_menu(Menu::PRINCIPAL);
}

void init_menu_resultados(){
    string palabra_str = "La palabra era\n" + partida.palabra_jugadores;

    menu_arriba.assign({
        new Texto(200, 6, 0.75, "Los impostores eran:", C2D_AlignCenter)
    });

    menu_abajo.assign({
        new Texto(160, 70, 0.8, palabra_str.c_str(), C2D_AlignCenter),
        new Boton(70, 130, 180, 35, "Jugar de nuevo", jugar_de_nuevo, 0.6),
        new Boton(70, 175, 180, 35, "Volver al menú", volver_al_menu, 0.6)
    });
}

void loop_menu_resultados(){
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

    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_menu_nombres();
}