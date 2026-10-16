#pragma once
#include <citro2d.h>

#define clrBlue (u32) 0xFFFF3030
#define clrWhite (u32) 0xFFFFFFFF
#define clrBg (u32) 0xFF800000
#define clrLight (u32) 0xFFFF8080
#define clrTrans (u32) 0x00000000
#define clrGrey (u32) 0xFF7C7265

extern u32 clrTarjeta;

enum MenuActivo {
	MENU_PRINCIPAL,
	MENU_OPCIONES,
	MENU_TUTORIAL,
	MENU_CREDITOS,
	MENU_GAMEPLAY,
	MENU_PARTIDA,
	MENU_RESULTADOS
};

extern C2D_Text g_staticText[32];

void prepararTextoEstatico();
bool tocandoRectangulo(touchPosition touch, float x, float y, float w, float h);