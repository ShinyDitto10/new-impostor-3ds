#include "menus.h"
#include "gameplay.h"
#include "guardado.h"

static string lista_archivos = "";
static vector<string> archivos;
static vector<bool> botones_pulsados;

static void boton_importar(u8 boton){
    if(archivos.size() <= boton || botones_pulsados[boton]) return;
    botones_pulsados[boton] = true;
    string ruta_archivo = "sdmc:/3ds/Impostor3DS/palabras/" + archivos[boton];
    leer_palabras_archivo(ruta_archivo.c_str());
    lista_archivos += "\nSe importó un archivo";
}

void init_menu_opciones(){
    menu_abajo.assign({
        new TextoDinamico(160, 20, 0.5, lista_archivos, C2D_AlignCenter),
    });

    archivos = leer_carpeta("sdmc:/3ds/Impostor3DS/palabras");

    if(archivos.size() == 0){
        lista_archivos = "No se encontraron archivos de palabras\n\
        Para usar tus propias palabras, pon los archivos en la carpeta\n\
        sdmc:/3ds/Impostor3DS/palabras/\n";
    }
    else{
        lista_archivos = "sdmc:/3ds/Impostor3DS/palabras/\n";
        int i = 0;
        for(auto arch : archivos){
            botones_pulsados.push_back(false);
            menu_abajo.push_back(new Rect(15, 35 * i + 50, 290, 25, (u32)Color::AZUL));
            menu_abajo.push_back(new Texto(20, 35 * i + 55, 0.6, arch.c_str(), 0));
            menu_abajo.push_back(new Boton(225, 35 * i + 52, 70, 21, "Importar", [i](){boton_importar(i);}, 0.6));
            string ruta_archivo = "sdmc:/3ds/Impostor3DS/palabras/" + arch;
            i++;
        }
    }
}

void loop_menu_opciones(){
    if(archivos.size() != 0 && menu_activo == Menu::OPCIONES){
        for(u8 i = 0; i < archivos.size(); i++) if(botones_pulsados[i]) menu_abajo[3 + i * 3]->activo = false;
    }

    if(kDown & KEY_B) init_menu(Menu::PRINCIPAL);
    renderizar_menu(menu_arriba, menu_abajo);
    renderizar_barra_arriba();
}