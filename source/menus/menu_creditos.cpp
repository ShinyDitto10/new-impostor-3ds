#include "ui.h"
#include "main.h"

void loop_menu_creditos(){
    static vector<Elemento*> menu_creditos_arriba = {
        new Texto(200, 50, 1.5f, "IMPOSTOR", C2D_AlignCenter),
        new Texto(10, 220, .5f, "ver 0.0.0", 0),
        new Texto(390, 220, .5f, "Creado por ShinyDitto10", C2D_AlignRight)
    };

    static vector<Elemento*> menu_creditos_abajo = {
        new Texto(160, 5, .8f, "Créditos", C2D_AlignCenter),
        new Texto(10, 40, .6f, "Impostor 3DS\nCreado con libctru\n y citro2d", 0),
        new Texto(10, 100, .6f, "- ShinyDitto10:\n   Programador", 0),
        new Texto(10, 137, .6f, "- mayi2323:\n   Lista de palabras", 0),
        new Texto(10, 180, .6f, "Si encuentras algún bug\no tienes alguna sugerencia,", 0),
        new Texto(10, 215, .6f, "escanea el QR y ve al proyecto en github", 0),
        new Sprite(185, 45, 1, codigo_qr)
    };

    if(kDown & KEY_B) menu_activo = Menu::PRINCIPAL;
    renderizar_menu(menu_creditos_arriba, menu_creditos_abajo);
}