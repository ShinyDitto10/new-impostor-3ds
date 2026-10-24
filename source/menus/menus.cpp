#include "menus.h"
#include "main.h"

std::unordered_map<Menu, void(*)(void)> func_init_menu = {
    {Menu::PRINCIPAL, init_menu_principal},
    {Menu::CREDITOS , init_menu_creditos},
    {Menu::CONFIGURACION , init_menu_config},
    {Menu::PALABRAS , init_menu_palabras},
    {Menu::PARTIDA , init_menu_partida},
    {Menu::RESULTADOS , init_menu_resultados},
    {Menu::OPCIONES , init_menu_opciones}
};

void init_menu(Menu menu){
    C2D_TextBufClear(buffer_txt_estatico);
    
    for(auto i : menu_abajo) delete i;
    for(auto i : menu_arriba) delete i;
    menu_abajo.clear();
    menu_arriba.clear();
    menu_activo = menu;
    func_init_menu.at(menu)();
}