/*IMPOSTOR v0.0.0
Creado por ShinyDitto10*/

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gameplay.h"
#include "menu_gameplay.h"
#include "ui.h"


int menuActivo = MENU_PRINCIPAL;

int jugadorComienza = 0;
u64 tiempoTranscurrido;
u64 tickInicial = 0;

typedef struct{
	int minutos;
	int segundos;
} Tiempo;


C2D_TextBuf g_dynamicBuf;
C2D_Text g_dynamicText[17];

Tiempo contadorTiempo;

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Preparar librerías
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	romfsInit();

	//Iniciar sprites
	C2D_SpriteSheet spritesT3X = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	C2D_Sprite codigoQR;
	C2D_SpriteFromSheet(&codigoQR, spritesT3X, 0);
	C2D_SpriteSetPos(&codigoQR, 185, 45);
	C2D_SpriteSetScale(&codigoQR, 1, 1);

	//Crear semilla RNG
    srand(time(NULL));

	config configPartida = { .numJugadores = 3,
	                         .numImpostores = 1,
							 .pistaImpostor = true,
							 .conocerImpostor = false};

	//Preparar palabras
	abrirPalabrasTXT();
	int numPalabrasArchivo = getNumPalabrasArchivo();
	palabrasPartida palabrasRonda = escogerPalabrasPartida(numPalabrasArchivo);

	// Preparar pantallas
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Preparar texto estático
	prepararTextoEstatico();
	g_dynamicBuf = C2D_TextBufNew(4096);

	bool pulsandoPantalla = false;
	bool pulsandoPantallaUltimoFrame;

	char nombreJug[12][20] = {
		"Jugador 1",
		"Jugador 2",
		"Jugador 3",
		"Jugador 4",
		"Jugador 5",
		"Jugador 6",
		"Jugador 7",
		"Jugador 8",
		"Jugador 9",
		"Jugador 10",
		"Jugador 11",
		"Jugador 12",
	};
	SwkbdState swkbd;

	// Bucle de la aplicación
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START && menuActivo == MENU_PRINCIPAL) break;
        
		//Comprobar pantalla táctil
		if(pulsandoPantalla) pulsandoPantallaUltimoFrame = true;
		else pulsandoPantallaUltimoFrame = false;
		touchPosition touch;
		hidTouchRead(&touch);
		if(touch.px == 0 && touch.py == 0) pulsandoPantalla = false;
		else pulsandoPantalla = true;

		//Teclado
		if (menuActivo == MENU_GAMEPLAY && kDown & KEY_A && posicionAid(botonSeleccionado) < configPartida.numJugadores){
			char buffer[16];
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 15);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancelar", false);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Confirmar", true);
	        swkbdSetHintText(&swkbd, "Escribe tu nombre");
	        SwkbdButton button = swkbdInputText(&swkbd, buffer, sizeof(buffer));
			if(button == SWKBD_BUTTON_RIGHT){
				strncpy(nombreJug[posicionAid(botonSeleccionado)], buffer, 16);
			}
		}

		C2D_TextBufClear(g_dynamicBuf);
		for(int i = 0; i < configPartida.numJugadores; i++){
			C2D_TextParse(&g_dynamicText[i], g_dynamicBuf, nombreJug[i]);
			C2D_TextOptimize(&g_dynamicText[i]);
		}
		char numImpostoresMenu[3];
		sprintf(numImpostoresMenu, "%d", configPartida.numImpostores);
		C2D_TextParse(&g_dynamicText[12], g_dynamicBuf, numImpostoresMenu);
		C2D_TextOptimize(&g_dynamicText[12]);
		C2D_TextParse(&g_dynamicText[13], g_dynamicBuf, textoTarjeta);
		C2D_TextOptimize(&g_dynamicText[13]);
		char jugadorComienzaOutput[31];
		sprintf(jugadorComienzaOutput, "Empieza %s", nombreJug[jugadorComienza]);
		C2D_TextParse(&g_dynamicText[14], g_dynamicBuf, jugadorComienzaOutput);
		C2D_TextOptimize(&g_dynamicText[14]);

		char tiempoTranscurridoOutput[31];
		tiempoTranscurrido = (svcGetSystemTick() - tickInicial) / SYSCLOCK_ARM11;
        contadorTiempo.minutos = tiempoTranscurrido / 60;
		contadorTiempo.segundos = tiempoTranscurrido % 60;
		sprintf(tiempoTranscurridoOutput, "Tiempo: %d:%02d", contadorTiempo.minutos, contadorTiempo.segundos);
		C2D_TextParse(&g_dynamicText[15], g_dynamicBuf, tiempoTranscurridoOutput);
		C2D_TextOptimize(&g_dynamicText[15]);

		char palabraRevelada[60];
		sprintf(palabraRevelada, "La palabra era\n%s", palabrasRonda.palabraJugadores);
		palabraRevelada[strlen(palabraRevelada) - 2] = '\0';
		C2D_TextParse(&g_dynamicText[16], g_dynamicBuf, palabraRevelada);
		C2D_TextOptimize(&g_dynamicText[16]);
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, clrBg);
		C2D_TargetClear(top, clrBg);

		if (menuActivo == MENU_PRINCIPAL){

			C2D_SceneBegin(bottom);
			C2D_DrawRectSolid(22, 20, 0.0f, 276, 80, tocandoRectangulo(touch, 22, 20, 276, 80) ? clrLight : clrBlue);
		    C2D_DrawRectSolid(22, 115, 0.0f, 82, 80, tocandoRectangulo(touch, 22, 115, 82, 80) ? clrLight : clrBlue);
		    C2D_DrawRectSolid(119, 115, 0.0f, 82, 80, tocandoRectangulo(touch, 119, 115, 82, 80) ? clrLight : clrBlue);
		    C2D_DrawRectSolid(216, 115, 0.0f, 82, 80, tocandoRectangulo(touch, 216, 115, 82, 80) ? clrLight : clrBlue);
		    C2D_DrawText(&g_staticText[3], C2D_WithColor | C2D_AlignCenter, 160, 220, 1, 0.5f, 0.5f, clrWhite);
		    C2D_DrawText(&g_staticText[4], C2D_WithColor | C2D_AlignCenter, 160, 45, 0.0f, 1.0f, 1.0f, clrWhite);
		    C2D_DrawText(&g_staticText[5], C2D_WithColor | C2D_AlignCenter, 63, 135, 0.0f, 0.75f, 0.75f, clrWhite);
		    C2D_DrawText(&g_staticText[6], C2D_WithColor | C2D_AlignCenter, 160, 125, 0.0f, 0.75f, 0.75f, clrWhite);
		    C2D_DrawText(&g_staticText[7], C2D_WithColor | C2D_AlignCenter, 257, 135, 0.0f, 0.75f, 0.75f, clrWhite);

			C2D_SceneBegin(top);
			C2D_DrawText(&g_staticText[0], C2D_WithColor | C2D_AlignCenter, 200, 50, 1, 1.5f, 1.5f, clrWhite);
		    C2D_DrawText(&g_staticText[1], C2D_WithColor, 10, 220, 1, 0.5f, 0.5f, clrWhite);
		    C2D_DrawText(&g_staticText[2], C2D_WithColor | C2D_AlignRight, 390, 220, 1, 0.5f, 0.5f, clrWhite);

			if (tocandoRectangulo(touch, 22, 20, 276, 80)) menuActivo = MENU_GAMEPLAY;
			if (tocandoRectangulo(touch, 216, 115, 82, 80)) menuActivo = MENU_CREDITOS;
			if (tocandoRectangulo(touch, 22, 115, 82, 80)) menuActivo = MENU_OPCIONES;
		}

		else if(menuActivo == MENU_GAMEPLAY){
			
			C2D_SceneBegin(bottom);
			C2D_DrawRectSolid(15, 20, 0.0f, 139, 65, clrBlue);
			C2D_DrawRectSolid(166, 20, 0.0f, 139, 65, clrBlue);
			C2D_DrawRectSolid(15, 95, 0.0f, 290, 25, clrBlue);
			C2D_DrawRectSolid(15, 130, 0.0f, 290, 25, clrBlue);
			C2D_DrawRectSolid(15, 165, 0.0f, 290, 25, clrBlue);
			C2D_DrawRectSolid(100, 200, 0.0f, 120, 35, clrBlue);
			C2D_DrawText(&g_staticText[8], C2D_WithColor | C2D_AlignCenter, 84.5f, 30, 0.0f, 0.75f, 0.75f, clrWhite);
			C2D_DrawText(&g_staticText[9], C2D_WithColor | C2D_AlignCenter, 235.5f, 30, 0.0f, 0.75f, 0.75f, clrWhite);

			C2D_DrawText(&g_staticText[11], C2D_WithColor, 20, 100, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_dynamicText[12], C2D_WithColor | C2D_AlignCenter, 265, 100, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[16], C2D_WithColor | C2D_AlignCenter, 240, 100, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[17], C2D_WithColor | C2D_AlignCenter, 290, 100, 0.0f, 0.6f, 0.6f, clrWhite);

			C2D_DrawText(&g_staticText[12], C2D_WithColor, 20, 135, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawRectSolid(235, 132, 0.0f, 30, 21, configPartida.pistaImpostor ? clrLight : clrTrans);
			C2D_DrawRectSolid(270, 132, 0.0f, 30, 21, configPartida.pistaImpostor ? clrTrans : clrLight);
			C2D_DrawText(&g_staticText[30], C2D_WithColor | C2D_AlignCenter, 250, 135, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[31], C2D_WithColor | C2D_AlignCenter, 285, 135, 0.0f, 0.6f, 0.6f, clrWhite);
			
			C2D_DrawText(&g_staticText[13], C2D_WithColor, 20, 170, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawRectSolid(235, 167, 0.0f, 30, 21, configPartida.conocerImpostor ? clrLight : clrTrans);
			C2D_DrawRectSolid(270, 167, 0.0f, 30, 21, configPartida.conocerImpostor ? clrTrans : clrLight);
			C2D_DrawText(&g_staticText[30], C2D_WithColor | C2D_AlignCenter, 250, 170, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[31], C2D_WithColor | C2D_AlignCenter, 285, 170, 0.0f, 0.6f, 0.6f, clrWhite);

			C2D_DrawText(&g_staticText[15], C2D_WithColor | C2D_AlignCenter, 160, 205, 0.0f, 0.6f, 0.6f, clrWhite);

			C2D_SceneBegin(top);
			crearMenuGameplayArriba(configPartida, partidaTerminada);
			C2D_DrawText(&g_staticText[14], C2D_WithColor | C2D_AlignCenter, 200, 6, 0.0f, 1, 1, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
			C2D_DrawText(&g_staticText[10], C2D_WithColor | C2D_AlignCenter, 200, 220, 0.0f, 0.5f, 0.5f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));

	        C2D_DrawText(&g_dynamicText[0], C2D_WithColor | C2D_AlignCenter, 73, 50, 0.0f, 0.65f, 0.65f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
			C2D_DrawText(&g_dynamicText[1], C2D_WithColor | C2D_AlignCenter, 198, 50, 0.0f, 0.65f, 0.65f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
			C2D_DrawText(&g_dynamicText[2], C2D_WithColor | C2D_AlignCenter, 323, 50, 0.0f, 0.65f, 0.65f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));
			C2D_DrawText(&g_dynamicText[3], C2D_WithColor | C2D_AlignCenter, 73, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 4) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[4], C2D_WithColor | C2D_AlignCenter, 198, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 5) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[5], C2D_WithColor | C2D_AlignCenter, 323, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 6) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[6], C2D_WithColor | C2D_AlignCenter, 73, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 7) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[7], C2D_WithColor | C2D_AlignCenter, 198, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 8) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[8], C2D_WithColor | C2D_AlignCenter, 323, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 9) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[9], C2D_WithColor | C2D_AlignCenter, 73, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 10) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[10], C2D_WithColor | C2D_AlignCenter, 198, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 11) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[11], C2D_WithColor | C2D_AlignCenter, 323, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 12) ? clrTrans : clrWhite);

			if(kDown & KEY_DDOWN && botonSeleccionado.y < 3) botonSeleccionado.y++;
			if(kDown & KEY_DUP && botonSeleccionado.y > 0) botonSeleccionado.y--;
			if(kDown & KEY_DRIGHT && botonSeleccionado.x < 2) botonSeleccionado.x++;
			if(kDown & KEY_DLEFT && botonSeleccionado.x > 0) botonSeleccionado.x--;

			if (tocandoRectangulo(touch, 15, 20, 139, 65) && configPartida.numJugadores < 12 && !pulsandoPantallaUltimoFrame) configPartida.numJugadores++;
		    if (tocandoRectangulo(touch, 166, 20, 139, 65) && configPartida.numJugadores > 3 && !pulsandoPantallaUltimoFrame) {
				configPartida.numJugadores--;
				if(configPartida.numImpostores + 1 == configPartida.numJugadores) configPartida.numImpostores--;
			}
			if (tocandoRectangulo(touch, 235, 95, 15, 25) && configPartida.numImpostores > 1 && !pulsandoPantallaUltimoFrame) configPartida.numImpostores--;
			if (tocandoRectangulo(touch, 285, 95, 15, 25) && configPartida.numImpostores < configPartida.numJugadores - 2 && configPartida.numImpostores < 6 && !pulsandoPantallaUltimoFrame) configPartida.numImpostores++;

			if(tocandoRectangulo(touch, 235, 132, 30, 21)) configPartida.pistaImpostor = true;
			if(tocandoRectangulo(touch, 270, 132, 30, 21)) configPartida.pistaImpostor = false;
			if(tocandoRectangulo(touch, 235, 167, 30, 21)) configPartida.conocerImpostor = true;
			if(tocandoRectangulo(touch, 270, 167, 30, 21)) configPartida.conocerImpostor = false;

			if (tocandoRectangulo(touch, 100, 200, 120, 35)){
				menuActivo = MENU_PARTIDA;
				abrirPalabrasTXT();
	            numPalabrasArchivo = getNumPalabrasArchivo();
	            palabrasRonda = escogerPalabrasPartida(numPalabrasArchivo);
				crearImpostores(configPartida.numJugadores, configPartida.numImpostores);
			}
	        else if (kDown & KEY_B){
	            menuActivo = MENU_PRINCIPAL;
			}
		}

		else if (menuActivo == MENU_PARTIDA){
			C2D_SceneBegin(bottom);
			C2D_DrawRectSolid(40, 40, 0.0f, 240, 150, (viendoPalabra == -1) ? clrBg : clrBlue);
			C2D_DrawText(&g_dynamicText[13], C2D_WithColor | C2D_AlignCenter, 160, 80, 0.0f, 1, 1, clrTarjeta);
			C2D_DrawRectSolid(70, 200, 0.0f, 180, 35, (viendoPalabra == -1) ? clrBg : clrBlue);
			C2D_DrawText(&g_staticText[20], C2D_WithColor | C2D_AlignCenter, 160, 210, 0.0f, 0.6f, 0.6f, (viendoPalabra == -1) ? clrTrans : clrWhite);
			if(viendoPalabra != -1 && !palabraVistaJugador[viendoPalabra]){
				C2D_DrawText(&g_staticText[19], C2D_WithColor | C2D_AlignCenter, 160, 80, 0.0f, 1, 1, clrWhite);
			}

			C2D_SceneBegin(top);
			crearMenuGameplayArriba(configPartida, partidaTerminada);
			C2D_DrawText(&g_staticText[18], C2D_WithColor | C2D_AlignCenter, 200, 10, 0.0f, 0.75f, 0.75f, clrWhite);
			C2D_DrawText(&g_dynamicText[0], C2D_WithColor | C2D_AlignCenter, 73, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[1], C2D_WithColor | C2D_AlignCenter, 198, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[2], C2D_WithColor | C2D_AlignCenter, 323, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[3], C2D_WithColor | C2D_AlignCenter, 73, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 4) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[4], C2D_WithColor | C2D_AlignCenter, 198, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 5) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[5], C2D_WithColor | C2D_AlignCenter, 323, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 6) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[6], C2D_WithColor | C2D_AlignCenter, 73, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 7) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[7], C2D_WithColor | C2D_AlignCenter, 198, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 8) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[8], C2D_WithColor | C2D_AlignCenter, 323, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 9) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[9], C2D_WithColor | C2D_AlignCenter, 73, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 10) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[10], C2D_WithColor | C2D_AlignCenter, 198, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 11) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[11], C2D_WithColor | C2D_AlignCenter, 323, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 12) ? clrTrans : clrWhite);

			if(viendoPalabra == -1){
			    if(kDown & KEY_DDOWN && botonSeleccionado.y < 3) botonSeleccionado.y++;
			    if(kDown & KEY_DUP && botonSeleccionado.y > 0) botonSeleccionado.y--;
			    if(kDown & KEY_DRIGHT && botonSeleccionado.x < 2) botonSeleccionado.x++;
			    if(kDown & KEY_DLEFT && botonSeleccionado.x > 0) botonSeleccionado.x--;
				id = posicionAid(botonSeleccionado);

				if(kDown & KEY_A && id < configPartida.numJugadores && !palabraVistaJugador[id]){
					viendoPalabra = id;
				}
			} else if(!palabraVistaJugador[viendoPalabra]){
				if(tocandoRectangulo(touch, 40, 40, 240, 150)){
					palabraVistaJugador[viendoPalabra] = true;
					clrTarjeta = colorTextoTarjeta(viendoPalabra, configPartida.numImpostores);
					crearTextoTarjeta(viendoPalabra, configPartida, palabrasRonda);
					if(comprobarImpostor(viendoPalabra, configPartida.numImpostores) && configPartida.conocerImpostor) partidaTerminada = true;
				}
			} else{
				if(tocandoRectangulo(touch, 120, 200, 100, 35)){
					partidaTerminada = false;
					borrarTextoTarjeta();
					viendoPalabra = -1;
					numPalabrasVistas++;
				}
			}

			if(numPalabrasVistas == configPartida.numJugadores){
				jugadorComienza = primerJugador(configPartida.numJugadores);
				tickInicial = svcGetSystemTick();
				botonSeleccionado.x = -1;
				botonSeleccionado.y = -1;
				menuActivo = MENU_RESULTADOS;
			}
		}

		else if(menuActivo == MENU_RESULTADOS){
			C2D_SceneBegin(bottom);
			if(!partidaTerminada){
			    C2D_DrawText(&g_dynamicText[14], C2D_WithColor | C2D_AlignCenter, 160, 70, 0.0f, 0.8f, 0.8f, clrWhite);
			    C2D_DrawText(&g_dynamicText[15], C2D_WithColor | C2D_AlignCenter, 160, 95, 0.0f, 0.8f, 0.8f, clrWhite);
			    C2D_DrawRectSolid(70, 130, 0.0f, 180, 35, clrBlue);
			    C2D_DrawText(&g_staticText[21], C2D_WithColor | C2D_AlignCenter, 160, 140, 0.0f, 0.6f, 0.6f, clrWhite);
				if(tocandoRectangulo(touch, 70, 130, 180, 35) && !pulsandoPantallaUltimoFrame) partidaTerminada = true;
			} else{
				C2D_DrawText(&g_dynamicText[16], C2D_WithColor | C2D_AlignCenter, 160, 70, 0.0f, 0.8f, 0.8f, clrWhite);
				C2D_DrawRectSolid(70, 130, 0.0f, 180, 35, clrBlue);
			    C2D_DrawText(&g_staticText[29], C2D_WithColor | C2D_AlignCenter, 160, 140, 0.0f, 0.6f, 0.6f, clrWhite);
				C2D_DrawRectSolid(70, 175, 0.0f, 180, 35, clrBlue);
			    C2D_DrawText(&g_staticText[22], C2D_WithColor | C2D_AlignCenter, 160, 185, 0.0f, 0.6f, 0.6f, clrWhite);
				if(tocandoRectangulo(touch, 70, 175, 180, 35) && !pulsandoPantallaUltimoFrame){
					numPalabrasVistas = 0;
					viendoPalabra = -1;
					for(int i = 0; i < 6; i++){
						IDimpostores[i] = -1;
					}
					for(int i = 0; i < 12; i++){
						palabraVistaJugador[i] = false;
					}
					partidaTerminada = false;
					fclose(palabrasTXT);
					botonSeleccionado.x = 0;
					botonSeleccionado.y = 0;

					menuActivo = MENU_PRINCIPAL;
				}
				if(tocandoRectangulo(touch, 70, 130, 180, 35) && !pulsandoPantallaUltimoFrame){
					numPalabrasVistas = 0;
					viendoPalabra = -1;
					for(int i = 0; i < 6; i++){
						IDimpostores[i] = -1;
					}
					for(int i = 0; i < 12; i++){
						palabraVistaJugador[i] = false;
					}
					partidaTerminada = false;
					fclose(palabrasTXT);
					botonSeleccionado.x = 0;
					botonSeleccionado.y = 0;

					menuActivo = MENU_PARTIDA;
					abrirPalabrasTXT();
	                numPalabrasArchivo = getNumPalabrasArchivo();
	                palabrasRonda = escogerPalabrasPartida(numPalabrasArchivo);
				    crearImpostores(configPartida.numJugadores, configPartida.numImpostores);
				}
			}
			C2D_SceneBegin(top);
			crearMenuGameplayArriba(configPartida, partidaTerminada);
			C2D_DrawText(&g_staticText[14], C2D_WithColor | C2D_AlignCenter, 200, 6, 0.0f, 0.75f, 0.75f, partidaTerminada ? clrTrans : clrWhite);
			C2D_DrawText(&g_staticText[28], C2D_WithColor | C2D_AlignCenter, 200, 6, 0.0f, 0.75f, 0.75f, partidaTerminada ? clrWhite : clrTrans);
			C2D_DrawText(&g_dynamicText[0], C2D_WithColor | C2D_AlignCenter, 73, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[1], C2D_WithColor | C2D_AlignCenter, 198, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[2], C2D_WithColor | C2D_AlignCenter, 323, 50, 0.0f, 0.65f, 0.65f, clrWhite);
			C2D_DrawText(&g_dynamicText[3], C2D_WithColor | C2D_AlignCenter, 73, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 4) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[4], C2D_WithColor | C2D_AlignCenter, 198, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 5) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[5], C2D_WithColor | C2D_AlignCenter, 323, 94, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 6) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[6], C2D_WithColor | C2D_AlignCenter, 73, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 7) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[7], C2D_WithColor | C2D_AlignCenter, 198, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 8) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[8], C2D_WithColor | C2D_AlignCenter, 323, 138, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 9) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[9], C2D_WithColor | C2D_AlignCenter, 73, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 10) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[10], C2D_WithColor | C2D_AlignCenter, 198, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 11) ? clrTrans : clrWhite);
			C2D_DrawText(&g_dynamicText[11], C2D_WithColor | C2D_AlignCenter, 323, 182, 0.0f, 0.65f, 0.65f, (configPartida.numJugadores < 12) ? clrTrans : clrWhite);
		}

		else if(menuActivo == MENU_CREDITOS){

			C2D_SceneBegin(bottom);
			C2D_DrawSprite(&codigoQR);
			C2D_DrawText(&g_staticText[7], C2D_WithColor | C2D_AlignCenter, 160, 5, 0.0f, 0.8f, 0.8f, clrWhite);
			C2D_DrawText(&g_staticText[25], C2D_WithColor, 10, 40, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[23], C2D_WithColor, 10, 100, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[24], C2D_WithColor, 10, 137, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[26], C2D_WithColor, 10, 180, 0.0f, 0.6f, 0.6f, clrWhite);
			C2D_DrawText(&g_staticText[27], C2D_WithColor, 10, 215, 0.0f, 0.6f, 0.6f, clrWhite);

			if(kDown & KEY_B) menuActivo = MENU_PRINCIPAL;
		}

		else if(menuActivo == MENU_OPCIONES){

			C2D_SceneBegin(bottom);
			C2D_SceneBegin(top);

			if(kDown & KEY_B) menuActivo = MENU_PRINCIPAL;
		}
		
	    C3D_FrameEnd(0);
	}

	//Cerrar hojas de sprites
	C2D_SpriteSheetFree(spritesT3X);

	//Cerrar librerías
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
