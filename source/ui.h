#pragma once
#include <string>
#include <citro2d.h>
#include <vector>
#include <array>
#include <functional>
using std::string, std::vector, std::array;

extern C2D_TextBuf buffer_txt_estatico;
extern C2D_TextBuf buffer_txt_dinamico;

enum class Color: u32{
    BG = 0xFF311910,
    OSCURO = 0xFF3A0906,
    AZUL = 0xFF632A16,
    BLANCO = 0xFFFFFFFF,
    AZUL_CLARO = 0xFFFF6505,
    TRANS = 0x00000000,
    GRIS = 0xFF7C7265,
    ROJO = 0xFF3934DF,
    VERDE = 0xFFBFD045
};

class Elemento{
    public:
    inline virtual void update(){return;};
    inline virtual void dibujar(){return;};
    virtual ~Elemento() = default;

    bool activo = true;
};

extern vector<Elemento*> menu_abajo;
extern vector<Elemento*> menu_arriba;

//typedef void(*Accion_Boton)(void);
using Accion_Boton = std::function<void(void)>;

class Boton : public Elemento{
    int x, y, w, h;
    C2D_Text texto;
    u32 col_texto = (u32)Color::BLANCO;
    float tamano_texto;
    Accion_Boton accion;

    public:
    void update() override;
    void dibujar() override;
    Boton(int x, int y, int w, int h, const char* texto, Accion_Boton accion, float tamano_texto, u32 col_texto = (u32)Color::BLANCO);
};

class Texto : public Elemento{
    int x, y;
    float tamano;
    u32 flags = C2D_WithColor;
    C2D_Text texto;

    public:
    void dibujar() override;
    Texto(int x, int y, float tamano, const char* texto, u32 flags);
};

class TextoDinamico : public Elemento{
    public:
    int x, y;
    float tamano;
    u32 flags = C2D_WithColor;
    u32 color = (u32)Color::BLANCO;
    C2D_Text texto;
    string& str_texto;

    void update() override;
    void dibujar() override;
    TextoDinamico(int x, int y, float tamano, string& str_texto, u32 flags);
};

class Sprite : public Elemento{
    int x, y;
    float tamano;
    C2D_Sprite sprite;

    public:
    void dibujar() override;
    Sprite(int x, int y, float tamano, C2D_Sprite sprite);
};

class Rect : public Elemento{
    public:
    int x, y, w, h;
    u32 color;

    void dibujar() override;
    Rect(int x, int y, int w, int h, u32 color);
};

struct Checkbox{
    int x, y, w, h;
    float tamano_texto;
    C2D_Text texto;
    Accion_Boton accion;
    bool tocado();
    void dibujar(bool activo);
    Checkbox(int x, int y, int w, int h, const char* texto, Accion_Boton accion, float tamano_texto);
};

template<size_t num_cajas>
class Checklist : public Elemento{
    array<Checkbox, num_cajas> cajas;
    public:
    size_t caja_activa = 0;

    void update() override{
        for(size_t i = 0; i < num_cajas; i++){
            if(cajas.at(i).tocado()){
                caja_activa = i;
                if(cajas.at(i).accion != nullptr) cajas.at(i).accion();
            }
        }
    };
    void dibujar() override{
        for(size_t i = 0; i < num_cajas; i++){
            cajas.at(i).dibujar(caja_activa == i);
        }
    };
    Checklist(array<Checkbox, num_cajas> cajas, size_t defecto = 0) : cajas(cajas){
        caja_activa = defecto;
    };
};

void renderizar_menu(vector<Elemento*>& menu_arriba, vector<Elemento*>& menu_abajo);
extern vector<Elemento*> menu_nombres_gameplay;
