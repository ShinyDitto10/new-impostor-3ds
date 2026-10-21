#include "ui.h"
#include "main.h"
#include "gameplay.h"
#include "guardado.h"

static string lista_archivos = "Pulsa A para leer la carpeta\nsdmc:/3ds/Impostor3DS/palabras/";

void loop_menu_opciones(){
    static vector<Elemento*> menu_opciones_abajo = {
        new TextoDinamico(160, 20, 0.5, lista_archivos, C2D_AlignCenter),
    };
    static vector<Elemento*> menu_opciones_arriba;

    if(kDown & KEY_A){
        vector<string> archivos = leer_carpeta("sdmc:/3ds/Impostor3DS/palabras");
        if(archivos.size() == 0) lista_archivos = "No se encontraron archivos en\nsdmc:/3ds/Impostor3DS/palabras/";
        else{
            lista_archivos = "Se han cargado los siguientes archivos de la SD:\n";
            for(auto i : archivos){
                lista_archivos += i + "\n";
                string ruta_archivo = "sdmc:/3ds/Impostor3DS/palabras/" + i;
                leer_palabras_archivo(ruta_archivo.c_str());
            }
        }
    }

    if(kDown & KEY_B) menu_activo = Menu::PRINCIPAL;
    renderizar_menu(menu_opciones_arriba, menu_opciones_abajo);
}