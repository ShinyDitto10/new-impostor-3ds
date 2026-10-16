#include <citro2d.h>

#include "ui.h"

C2D_Text g_staticText[32];
u32 clrTarjeta = 0xFF4B33FF;

void prepararTextoEstatico(){
    C2D_TextBuf g_staticBuf;
    g_staticBuf = C2D_TextBufNew(4096);

    C2D_TextParse(&g_staticText[0], g_staticBuf, "IMPOSTOR");
	C2D_TextParse(&g_staticText[1], g_staticBuf, "ver 0.1.0");
	C2D_TextParse(&g_staticText[2], g_staticBuf, "Creado por ShinyDitto10");
	C2D_TextParse(&g_staticText[3], g_staticBuf, "Pulsa START para salir");
	C2D_TextParse(&g_staticText[4], g_staticBuf, "Jugar");
	C2D_TextParse(&g_staticText[5], g_staticBuf, "Opciones");
	C2D_TextParse(&g_staticText[6], g_staticBuf, "Cómo\njugar");
	C2D_TextParse(&g_staticText[7], g_staticBuf, "Créditos");
	C2D_TextParse(&g_staticText[8], g_staticBuf, "Añadir\nJugador");
	C2D_TextParse(&g_staticText[9], g_staticBuf, "Quitar\nJugador");
	C2D_TextParse(&g_staticText[10], g_staticBuf, "Cruceta: Navegar  A: Cambiar nombre  B: Volver");
	C2D_TextParse(&g_staticText[11], g_staticBuf, "Número de impostores");
	C2D_TextParse(&g_staticText[12], g_staticBuf, "Pista para los impostores");
	C2D_TextParse(&g_staticText[13], g_staticBuf, "Los impostores se conocen");
	C2D_TextParse(&g_staticText[14], g_staticBuf, "Jugadores");
	C2D_TextParse(&g_staticText[15], g_staticBuf, "Empezar partida");
	C2D_TextParse(&g_staticText[16], g_staticBuf, "-");
	C2D_TextParse(&g_staticText[17], g_staticBuf, "+");
	C2D_TextParse(&g_staticText[18], g_staticBuf, "Escoge tu nombre de la lista:");
	C2D_TextParse(&g_staticText[19], g_staticBuf, "Pulsa para ver\ntu palabra");
	C2D_TextParse(&g_staticText[20], g_staticBuf, "Siguiente jugador");
	C2D_TextParse(&g_staticText[21], g_staticBuf, "Terminar partida");
	C2D_TextParse(&g_staticText[22], g_staticBuf, "Volver al menú");
    C2D_TextParse(&g_staticText[23], g_staticBuf, "- ShinyDitto10:\n   Programador");
    C2D_TextParse(&g_staticText[24], g_staticBuf, "- mayi2323:\n   Lista de palabras");
    C2D_TextParse(&g_staticText[25], g_staticBuf, "Impostor 3DS\nCreado con libctru\n y citro2d");
    C2D_TextParse(&g_staticText[26], g_staticBuf, "Si encuentras algún bug\no tienes alguna sugerencia,");
    C2D_TextParse(&g_staticText[27], g_staticBuf, "escanea el QR y ve al proyecto en github");
    C2D_TextParse(&g_staticText[28], g_staticBuf, "Los impostores eran:");
    C2D_TextParse(&g_staticText[29], g_staticBuf, "Jugar de nuevo");
	C2D_TextParse(&g_staticText[30], g_staticBuf, "Sí");
	C2D_TextParse(&g_staticText[31], g_staticBuf, "No");
	for(int i = 0; i < 32; i++){
	    C2D_TextOptimize(&g_staticText[i]);
	}
}

bool tocandoRectangulo(touchPosition touch, float x, float y, float w, float h){
	if(touch.px < x + w && touch.px > x && touch.py < y + h && touch.py > y) return true;
	else return false;
}