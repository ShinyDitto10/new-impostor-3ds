#include <stdbool.h>
#include "citro2d.h"
#include <stdlib.h>
#include <string.h>

#include "gameplay.h"

FILE *palabrasTXT;
int IDimpostores[6] = {-1, -1, -1, -1, -1, -1};
bool palabraVistaJugador[] = {false, false, false, false, false, false, false, false, false, false, false, false};
int numPalabrasVistas = 0;
int viendoPalabra = -1;
int id;
char textoTarjeta[50] = "   ";
bool partidaTerminada = false;


bool abrirPalabrasTXT() {
    palabrasTXT = fopen("romfs:/palabras_impostor.txt", "r");
    if(palabrasTXT == NULL) return false;
    else return true;
}

int getNumPalabrasArchivo() {
    char buffer[10];
    int numPalabrasArchivo;
    fgets(buffer, 10, palabrasTXT);
    sscanf(buffer, "%d", &numPalabrasArchivo);
    if(numPalabrasArchivo > 0) return numPalabrasArchivo;
    else return -1;
}

palabrasPartida escogerPalabrasPartida(int numPalabrasArchivo) {
    char buffer[30];

    int palabraID = rand() % numPalabrasArchivo;
    for(int i = 0; i < (1 + palabraID * 5); i++){
        fgets(buffer, 30, palabrasTXT);
    }
    fgets(buffer, 30, palabrasTXT);
    char palabraJugadores[30];
    strncpy(palabraJugadores, buffer, 29);

    int pistaID = rand() % 3;
    for(int i = 0; i < pistaID - 1; i++){
        fgets(buffer, 30, palabrasTXT);
    }
    fgets(buffer, 30, palabrasTXT);
    char pistaImpostor[30];
    strncpy(pistaImpostor, buffer, 29);
    
    palabrasPartida palabrasPartida;
    strncpy(palabrasPartida.palabraJugadores, palabraJugadores, 29);
    strncpy(palabrasPartida.pistaImpostor, pistaImpostor, 29);
    return palabrasPartida;
}

void cerrarPalabrasTXT() {
    fclose(palabrasTXT);
}

bool comprobarImpostoresRepetidos(int numJugadores, int numImpostores){
    for(int i = 0; i < numImpostores - 1; i++){
        for(int j = i + 1; j < numImpostores; j++){
            if(IDimpostores[i] == IDimpostores[j]) return true;
        }
    }
    return false;
}

void crearImpostores(int numJugadores, int numImpostores){
    do{
        for(int i = 0; i < numImpostores; i++){
            IDimpostores[i] = rand() % numJugadores;
        }
    } while(comprobarImpostoresRepetidos(numJugadores, numImpostores));
}

bool comprobarImpostor(int numJugador, int numImpostores){
    for(int i = 0; i < numImpostores; i++){
        if(IDimpostores[i] == numJugador) return true;
    }
    return false;
}

void crearTextoTarjeta(int numJugador, config configPartida, palabrasPartida palabras){
    if(comprobarImpostor(numJugador, configPartida.numImpostores)){
        if(configPartida.pistaImpostor) sprintf(textoTarjeta, "IMPOSTOR\nPista: %s", palabras.pistaImpostor);
        else sprintf(textoTarjeta, "IMPOSTOR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    }
    else{
        sprintf(textoTarjeta, "%s", palabras.palabraJugadores);
    }
    if(strlen(textoTarjeta) > 2) textoTarjeta[strlen(textoTarjeta) - 2] = '\0';
}

void borrarTextoTarjeta() {
    sprintf(textoTarjeta, "                                               ");
}

u32 colorTextoTarjeta(int numJugador, int numImpostores) {
    if(comprobarImpostor(numJugador, numImpostores)){
        return C2D_Color32(0xDF, 0x34, 0x39, 0xFF);
    } else return C2D_Color32(0x45, 0xD0, 0xBF, 0xFF);
}

int primerJugador(int numJugadores) {
    return rand() % numJugadores;
}