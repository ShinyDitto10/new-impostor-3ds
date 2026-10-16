#include "ui.h"
#include "main.h"

static void boton_jugar(){
    menu_activo = Menu::CONFIGURACION;
}

static void boton_creditos(){
    menu_activo = Menu::CREDITOS;
}

void loop_menu_principal(){
    static vector<Elemento*> menu_principal_arriba = {
        new Texto(200, 50, 1.5f, "IMPOSTOR", C2D_AlignCenter),
        new Texto(10, 220, .5f, "ver 0.1.0", 0),
        new Texto(390, 220, .5f, "Creado por ShinyDitto10", C2D_AlignRight)
    };

    static vector<Elemento*> menu_principal_abajo = {
        new Boton(22, 20, 276, 80, "Jugar", boton_jugar, 1.0),
        new Boton(22, 115, 82, 80, "Opciones", nullptr, 0.75),
        new Boton(119, 115, 82, 80, "Cómo\njugar", nullptr, 0.75),
        new Boton(216, 115, 82, 80, "Créditos", boton_creditos, 0.75),
        new Texto(160, 220, 0.5f, "Pulsa START para salir", C2D_AlignCenter)
    };

    renderizar_menu(menu_principal_arriba, menu_principal_abajo);
}