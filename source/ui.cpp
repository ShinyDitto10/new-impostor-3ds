#include "ui.h"
#include "main.h"

C2D_TextBuf buffer_txt_estatico;
C2D_TextBuf buffer_txt_dinamico;

vector<Elemento*> menu_abajo;
vector<Elemento*> menu_arriba;

void Boton::update(){
    //Comprueba si el botón se ha soltado este frame
    if(pos_tactil_anterior.px < x + w && pos_tactil_anterior.px > x && pos_tactil_anterior.py < y + h && pos_tactil_anterior.py > y
    && pos_tactil.px == 0 && pos_tactil.py == 0 && this->accion != nullptr){
        this->accion();
    }
}

void Boton::dibujar(){
    float w_texto;
    float h_texto;
    C2D_TextGetDimensions(&texto, tamano_texto, tamano_texto, &w_texto, &h_texto);

    if(pos_tactil.px < x + w && pos_tactil.px > x && pos_tactil.py < y + h && pos_tactil.py > y){
        C2D_DrawRectSolid(x, y, 0, w, h, (u32)Color::AZUL_CLARO);
        C2D_DrawText(&texto, C2D_WithColor | C2D_AlignCenter, x + w / 2, y + (h - h_texto) / 2, 0, tamano_texto, tamano_texto, col_texto);
    } else{
        C2D_DrawRectSolid(x, y, 0, w, h, (u32)Color::AZUL);
        C2D_DrawText(&texto, C2D_WithColor | C2D_AlignCenter, x + w / 2, y + (h - h_texto) / 2, 0, tamano_texto, tamano_texto, col_texto);
    }
}

Boton::Boton(int x, int y, int w, int h, const char* texto, Accion_Boton accion, float tamano_texto, u32 col_texto){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->tamano_texto = tamano_texto;
    C2D_TextParse(&this->texto, buffer_txt_estatico, texto);
    C2D_TextOptimize(&this->texto);
    this->accion = accion;
    this->col_texto = col_texto;
}

void Texto::dibujar(){
    C2D_DrawText(&texto, flags, x, y, 0.f, tamano, tamano, (u32)Color::BLANCO);
}

Texto::Texto(int x, int y, float tamano, const char* texto, u32 flags = 0){
    this->x = x;
    this->y = y;
    this->tamano = tamano;
    this->flags = flags | C2D_WithColor;
    C2D_TextParse(&this->texto, buffer_txt_estatico, texto);
    C2D_TextOptimize(&this->texto);
}

void TextoDinamico::update(){
    C2D_TextParse(&texto, buffer_txt_dinamico, str_texto.c_str());
    C2D_TextOptimize(&texto);
}

void TextoDinamico::dibujar(){
    C2D_DrawText(&texto, flags, x, y, 0, tamano, tamano, color);
}

TextoDinamico::TextoDinamico(int x, int y, float tamano, string& str_texto, u32 flags) : str_texto(str_texto){
    this->x = x;
    this->y = y;
    this->tamano = tamano;
    this->flags = flags | C2D_WithColor;
    C2D_TextParse(&texto, buffer_txt_dinamico, this->str_texto.c_str());
    C2D_TextOptimize(&texto);
}

void Sprite::dibujar(){
    C2D_SpriteSetPos(&sprite, x, y);
    C2D_SpriteSetScale(&sprite, tamano, tamano);
    C2D_DrawSprite(&sprite);
}

Sprite::Sprite(int x, int y, float tamano, C2D_Sprite sprite){
    this->x = x;
    this->y = y;
    this->tamano = tamano;
    this->sprite = sprite;
}

void Rect::dibujar(){
    C2D_DrawRectSolid(x, y, 0, w, h, color);
}

Rect::Rect(int x, int y, int w, int h, u32 color){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->color = color;
}

bool Checkbox::tocado(){
    return(pos_tactil_anterior.px < x + w && pos_tactil_anterior.px > x && pos_tactil_anterior.py < y + h && pos_tactil_anterior.py > y
    && pos_tactil.px == 0 && pos_tactil.py == 0);
}

void Checkbox::dibujar(bool activo){
    float w_texto;
    float h_texto;
    C2D_TextGetDimensions(&texto, tamano_texto, tamano_texto, &w_texto, &h_texto);

    C2D_DrawRectSolid(x, y, 0, w, h, activo ? (u32)Color::AZUL_CLARO : (u32)Color::AZUL);
    C2D_DrawText(&texto, C2D_WithColor | C2D_AlignCenter, x + w / 2, y + (h - h_texto) / 2, 0, tamano_texto, tamano_texto, (u32)Color::BLANCO);
}

Checkbox::Checkbox(int x, int y, int w, int h, const char* texto, Accion_Boton accion, float tamano_texto){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->accion = accion;
    this->tamano_texto = tamano_texto;
    C2D_TextParse(&this->texto, buffer_txt_estatico, texto);
    C2D_TextOptimize(&this->texto);
}

void renderizar_menu(vector<Elemento*>& menu_arriba, vector<Elemento*>& menu_abajo){
    Menu menu_al_iniciar = menu_activo;

    for(auto& i : menu_arriba){
        if(i != nullptr && i->activo) i->update();
        if(menu_al_iniciar != menu_activo) break;
    }
    if(menu_al_iniciar != menu_activo) return;
    for(auto& i : menu_abajo){
        if(i != nullptr && i->activo) i->update();
        if(menu_al_iniciar != menu_activo) break;
    }
    if(menu_al_iniciar != menu_activo) return;

    C2D_SceneBegin(top);
    C2D_TargetClear(top, (u32)Color::BG);
    for(auto& i : menu_arriba){
        if(i != nullptr && i->activo) i->dibujar();
    }
    C2D_SceneBegin(bottom);
    C2D_TargetClear(bottom, (u32)Color::BG);
    for(auto& i : menu_abajo){
        if(i != nullptr && i->activo) i->dibujar();
    }
}