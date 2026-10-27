#include "menus.h"
#include <sstream>
#include <iomanip>

static vector<Elemento*> barra_arriba;
static string hora = "00:00:00";

void crear_barra_arriba(){
    barra_arriba.assign({
        new Sprite(373, 0, 1, bateria[0]),
        new TextoDinamico(365, 1, 0.5, hora, C2D_AlignRight)
    });
}

void renderizar_barra_arriba(){
    u8 niv_bateria = 0;
    u8 cargando = false;
    PTMU_GetBatteryLevel(&niv_bateria);
    PTMU_GetBatteryChargeState(&cargando);

    Sprite* bat = (Sprite*)barra_arriba[0];
    if(cargando) bat->sprite = bateria[5];
    else if(niv_bateria == 0) bat->sprite = bateria[0];
    else bat->sprite = bateria[niv_bateria - 1];

    u64 time = (osGetTime() % (24 * 3600 * 1000)) / 1000;
    u32 h = time / 3600;
    u32 min = (time % 3600) / 60;

    std::stringstream ss;
    ss << h << ":" << std::setw(2) << std::setfill('0') << min;
    hora = ss.str();

    for(auto i : barra_arriba) i->update();
    C2D_SceneBegin(top);
    for(auto i : barra_arriba) i->dibujar();
}