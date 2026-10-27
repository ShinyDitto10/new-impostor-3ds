#pragma once
#include "ui.h"
#include "main.h"

void init_menu(Menu menu);

void init_menu_principal();
void init_menu_creditos();
void init_menu_config();
void init_menu_palabras();
void init_menu_partida();
void init_menu_resultados();
void init_menu_opciones();

void loop_menu_principal();
void loop_menu_creditos();
void loop_menu_config();
void loop_menu_palabras();
void loop_menu_partida();
void loop_menu_resultados();
void loop_menu_opciones();

void crear_menu_nombres();
//void actualizar_menu_nombres();
void renderizar_menu_nombres();

void crear_barra_arriba();
void renderizar_barra_arriba();