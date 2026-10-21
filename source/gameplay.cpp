#include "gameplay.h"
#include <fstream>
#include <algorithm>

vector<Palabra> lista_palabras;
Partida partida;

void leer_palabras_archivo(const char* ruta_archivo){
    std::ifstream archivo(ruta_archivo);
    while(!archivo.eof()){
        string linea_leida;
        Palabra palabra_leida;

        std::getline(archivo, linea_leida);
        palabra_leida.palabra = linea_leida;
        for(int i = 0; i < 3; i++){
            std::getline(archivo, linea_leida);
            palabra_leida.pistas[i] = linea_leida;
        }
        std::getline(archivo, linea_leida);
        
        lista_palabras.push_back(palabra_leida);
    }
    archivo.close();
}

void Partida::escoger_palabras(){
    int id_palabra = std::rand() % lista_palabras.size();
    palabra_jugadores = lista_palabras[id_palabra].palabra;
    if(palabra_jugadores.at(palabra_jugadores.size() - 1) == 0xD) palabra_jugadores.pop_back();

    int id_pista = std::rand() % 3;
    pista_impostor = lista_palabras[id_palabra].pistas[id_pista];
    if(pista_impostor.at(pista_impostor.size() - 1) == 0xD) pista_impostor.pop_back();
}

void Partida::escoger_impostores(){
    id_impostores.clear();
    int nuevo_impostor;
    for(int i = 0; i < num_impostores; i++){
        do{
            nuevo_impostor = std::rand() % num_jugadores;
        } while(std::find(id_impostores.begin(), id_impostores.end(), nuevo_impostor) != id_impostores.end());
        id_impostores.push_back(nuevo_impostor);
    }
}

bool Partida::jugador_es_impostor(int jugador){
    return(std::find(id_impostores.begin(), id_impostores.end(), jugador) != id_impostores.end());
}

void Partida::empezar_crono(){
    tiempo_inicial = osGetTime();
}

void Partida::escoger_primer_jugador(){
    empieza = std::rand() % num_jugadores;
}

static const char* nombres_default[12] = {
    "Jugador 1",
	"Jugador 2",
	"Jugador 3",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

Partida::Partida(){
    for(int i = 0; i < 12; i++){
        nombre_jugadores[i] = nombres_default[i];
    }
}