#pragma once
#include "gameplay.h"

void crear_carpeta_juego();
void guardar_config_partida(const Partida& partida, int slot);
bool cargar_config_partida(Partida& partida, int slot);

vector<string> leer_carpeta(const char* carpeta);