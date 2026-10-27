#include "menus.h"

static void boton_jugar(){
    init_menu(Menu::CONFIGURACION);
}

static void boton_creditos(){
    init_menu(Menu::CREDITOS);
}

static void boton_opciones(){
    init_menu(Menu::OPCIONES);
}

void init_menu_principal(){
    menu_arriba.assign({
        new Texto(200, 50, 1.5f, "IMPOSTOR", C2D_AlignCenter),
        new Rect(0, 215, 400, 25, (u32)Color::OSCURO),
        new Texto(10, 220, .5f, "ver 0.1.0", 0),
        new Texto(390, 220, .5f, "Creado por ShinyDitto10", C2D_AlignRight)
    });

    menu_abajo.assign({
        new Boton(22, 20, 276, 80, "Jugar", boton_jugar, 1.0),
        new Boton(22, 115, 82, 80, "Opciones", boton_opciones, 0.75),
        new Boton(119, 115, 82, 80, "Cómo\njugar", nullptr, 0.75),
        new Boton(216, 115, 82, 80, "Créditos", boton_creditos, 0.75),
        new Texto(160, 220, 0.5f, "Pulsa START para salir", C2D_AlignCenter)
    });
}

void loop_menu_principal(){
    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_barra_arriba();
}