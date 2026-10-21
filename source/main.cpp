/*IMPOSTOR v0.1.0
creado por ShinyDitto10*/

#include <citro2d.h>
#include "main.h"
#include "gameplay.h"
#include "ui.h"
#include "menus/menus.h"
#include "guardado.h"

Menu menu_activo = Menu::PRINCIPAL;
vector<Elemento*> menu_nombres_gameplay;

u32 kDown;
touchPosition pos_tactil = {0, 0};
touchPosition pos_tactil_anterior = {0, 0};

C3D_RenderTarget* bottom;
C3D_RenderTarget* top;

C2D_Sprite codigo_qr;

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	romfsInit();

	C2D_SpriteSheet spritesT3X = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	C2D_SpriteFromSheet(&codigo_qr, spritesT3X, 0);
	C2D_SpriteSetPos(&codigo_qr, 185, 45);
	C2D_SpriteSetScale(&codigo_qr, 1, 1);

	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    std::srand(time(NULL));
    leer_palabras_archivo(RUTA_ARCHIVO_PALABRAS);

    buffer_txt_estatico = C2D_TextBufNew(4096);
	buffer_txt_dinamico = C2D_TextBufNew(4096);

	crear_menu_nombres();
	crear_carpeta_juego();

    while (aptMainLoop())
	{
        hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_START && menu_activo == Menu::PRINCIPAL) break;

		pos_tactil_anterior = pos_tactil;
        hidTouchRead(&pos_tactil);

		C2D_TextBufClear(buffer_txt_dinamico);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        switch(menu_activo){
            case Menu::PRINCIPAL:
                loop_menu_principal();
                break;
			case Menu::CONFIGURACION:
				loop_menu_config();
				break;
			case Menu::PALABRAS:
				loop_menu_palabras();
				break;
			case Menu::PARTIDA:
				loop_menu_partida();
				break;
			case Menu::RESULTADOS:
				loop_menu_resultados();
				break;
			case Menu::OPCIONES:
				loop_menu_opciones();
				break;
            case Menu::CREDITOS:
                loop_menu_creditos();
                break;
            default:
                C2D_SceneBegin(top);
                C2D_SceneBegin(bottom);
                break;
        }

        C3D_FrameEnd(0);
	}

	C2D_SpriteSheetFree(spritesT3X);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}