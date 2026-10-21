#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <cJSON.h>
#include "gameplay.h"

void crear_carpeta_juego(){
    mkdir("sdmc:/3ds/Impostor3DS", 0777);
    mkdir("sdmc:/3ds/Impostor3DS/save", 0777);
    mkdir("sdmc:/3ds/Impostor3DS/palabras", 0777);
}

void guardar_config_partida(const Partida& partida, int slot){
    string ruta = "sdmc:/3ds/Impostor3DS/save/config_partida_" + std::to_string(slot) + ".json";
    std::ofstream savefile(ruta);
    cJSON* datos = cJSON_CreateObject();
    cJSON_AddNumberToObject(datos, "num_jugadores", partida.num_jugadores);
    cJSON_AddNumberToObject(datos, "num_impostores", partida.num_impostores);
    cJSON_AddBoolToObject(datos, "pista", partida.hay_pista);
    cJSON_AddBoolToObject(datos, "imp_se_conocen", partida.impostores_se_conocen);

    const char* nombres[12] = {
        partida.nombre_jugadores[0].c_str(),
        partida.nombre_jugadores[1].c_str(),
        partida.nombre_jugadores[2].c_str(),
        partida.nombre_jugadores[3].c_str(),
        partida.nombre_jugadores[4].c_str(),
        partida.nombre_jugadores[5].c_str(),
        partida.nombre_jugadores[6].c_str(),
        partida.nombre_jugadores[7].c_str(),
        partida.nombre_jugadores[8].c_str(),
        partida.nombre_jugadores[9].c_str(),
        partida.nombre_jugadores[10].c_str(),
        partida.nombre_jugadores[11].c_str(),
    };
    cJSON* array_nombres = cJSON_CreateStringArray(nombres, 12);
    cJSON_AddItemToObject(datos, "jugadores", array_nombres);
    char* output = cJSON_Print(datos);
    string output_str(output);
    savefile.write(output_str.c_str(), output_str.size());
    savefile.close();
    cJSON_free(output);
    cJSON_Delete(datos);
}

bool cargar_config_partida(Partida& partida, int slot){
    string ruta = "sdmc:/3ds/Impostor3DS/save/config_partida_" + std::to_string(slot) + ".json";
    std::ifstream savefile(ruta);
    if(!savefile.is_open()) return false;

    savefile.seekg(0, std::ios::end);
    size_t tamano = savefile.tellg();
    char* buffer = new char[tamano];
    savefile.seekg(0);
    savefile.read(buffer, tamano);

    cJSON* datos = cJSON_ParseWithLength(buffer, tamano);
    if(!datos){delete[] buffer; return false;}
    cJSON* json = nullptr;
    json = cJSON_GetObjectItemCaseSensitive(datos, "num_jugadores");
    if(!json || !cJSON_IsNumber(json)){cJSON_Delete(datos); delete[] buffer; return false;}
    partida.num_jugadores = cJSON_GetNumberValue(json);

    json = cJSON_GetObjectItemCaseSensitive(datos, "num_impostores");
    if(!json || !cJSON_IsNumber(json)){cJSON_Delete(datos); delete[] buffer; return false;}
    partida.num_impostores = cJSON_GetNumberValue(json);

    json = cJSON_GetObjectItemCaseSensitive(datos, "pista");
    if(!json || !cJSON_IsBool(json)){cJSON_Delete(datos); delete[] buffer; return false;}
    partida.hay_pista = cJSON_IsTrue(json);

    json = cJSON_GetObjectItemCaseSensitive(datos, "imp_se_conocen");
    if(!json || !cJSON_IsBool(json)){cJSON_Delete(datos); delete[] buffer; return false;}
    partida.impostores_se_conocen = cJSON_IsTrue(json);

    json = cJSON_GetObjectItemCaseSensitive(datos, "jugadores");
    if(!json || !cJSON_IsArray(json)){cJSON_Delete(datos); delete[] buffer; return false;}
    cJSON* it = nullptr;
    int i = 0;
    cJSON_ArrayForEach(it, json){
        if(!it || !cJSON_IsString(it)){cJSON_Delete(datos); delete[] buffer; return false;}
        partida.nombre_jugadores[i] = cJSON_GetStringValue(it);
        i++;
    }

    cJSON_Delete(datos);
    delete[] buffer;
    return true;
}

vector<string> leer_carpeta(const char* carpeta){
    vector<string> archivos;
    DIR* dir;
    dirent* ent;
    if((dir = opendir(carpeta)) == nullptr) return archivos;

    while((ent = readdir(dir)) != nullptr){
        if(ent->d_type != DT_REG) continue;
        archivos.push_back(string(ent->d_name));
    }
    closedir(dir);
    return archivos;
}