#pragma once

enum class Menu{
    PRINCIPAL,
    CONFIGURACION,
    PALABRAS,
    PARTIDA,
    RESULTADOS,
    CREDITOS,
    OPCIONES,
    TUTORIAL
};

extern Menu menu_activo;

extern u32 kDown;
extern touchPosition pos_tactil;
extern touchPosition pos_tactil_anterior;

extern C3D_RenderTarget* bottom;
extern C3D_RenderTarget* top;

extern C2D_Sprite codigo_qr;