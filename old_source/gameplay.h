#pragma once
#include <stdio.h>
#include <stdbool.h>

extern FILE *palabrasTXT;

typedef struct {
    char palabraJugadores[30];
    char pistaImpostor[30];
} palabrasPartida;

typedef struct{
    int numJugadores;
    int numImpostores;
    bool pistaImpostor;
    bool conocerImpostor;
} config;

extern int IDimpostores[6];
extern bool palabraVistaJugador[];
extern int numPalabrasVistas;
extern int viendoPalabra;
extern int id;
extern char textoTarjeta[50];
extern bool partidaTerminada;


bool abrirPalabrasTXT();
int getNumPalabrasArchivo();
palabrasPartida escogerPalabrasPartida(int numPalabrasArchivo);
void cerrarPalabrasTXT();
bool comprobarImpostoresRepetidos(int numJugadores, int numImpostores);
void crearImpostores(int numJugadores, int numImpostores);
bool comprobarImpostor(int numJugador, int numImpostores);
void crearTextoTarjeta(int numJugador, config configPartida, palabrasPartida palabras);
void borrarTextoTarjeta();
u32 colorTextoTarjeta(int numJugador, int numImpostores);
int primerJugador(int numJugadores);