#pragma once
#include <string>
#include <vector>
#include <3ds.h>
using std::string, std::vector;

#define RUTA_ARCHIVO_PALABRAS "romfs:/palabras_impostor.txt"
#define MAX_JUGADORES 12
#define MIN_JUGADORES 3

struct Palabra{
    string palabra;
    string pistas[3];
};

class Partida{
    public:
    u8 num_jugadores = 3;
    u8 num_impostores = 1;
    bool hay_pista = true;
    bool impostores_se_conocen = false;

    string palabra_jugadores;
    string pista_impostor;

    string nombre_jugadores[12];
    vector<int> id_impostores;

    u8 empieza;
    u64 tiempo_inicial;

    void escoger_palabras();
    void escoger_impostores();
    bool jugador_es_impostor(int jugador);
    void empezar_crono();
    void escoger_primer_jugador();
    Partida();
};

extern vector<Palabra> lista_palabras;
extern Partida partida;

void leer_palabras_archivo(const char* ruta_archivo);