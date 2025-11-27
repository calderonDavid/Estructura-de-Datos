#include "menu.h"
#include "Genoma.h"
#include "Archivo.h"
#include "Secuencia.h"
#include <sstream>
#include <iostream>
#include <vector>

// Definición de los comandos permitidos
std::vector<std::string> comandospermitidos = {
    "1- cargar <nombre_archivo>",
    "2- listar_secuencias",
    "3- histograma <descripcion_secuencia>",
    "4- es_subsecuencia <secuencia>",
    "5- enmascarar <secuencia>",
    "6- guardar <nombre_archivo>",
    "7- codificar <nombre_archivo>",
    "8- decodificar <nombre_archivo>",
    "9- ruta_mas_corta <desc_secuencia> <i> <j> <x> <y>",
    "10- base_remota <desc_secuencia> <i> <j>",
    "11- ayuda <comando>",
    "12- salir"
};

void procesarComando(const std::string& linea, Genoma &genoma) {
    std::istringstream iss(linea);
    std::string comando;
    std::string parametros;
    
    iss >> comando;
    // Leemos el resto de la linea para parsear manualmente segun el comando
    std::getline(iss, parametros);

    // Limpieza de espacios iniciales en 'parametros'
    if (!parametros.empty()) {
        size_t first = parametros.find_first_not_of(' ');
        if (std::string::npos != first)
        {
            parametros = parametros.substr(first);
        }
    }

    if (comando == "cargar") {
        if (parametros.empty()) {
            std::cout << "Debe indicar un archivo .txt (sin extension en el parametro)\n";
        } else {
            Archivo a;
            genoma = a.leer(parametros);
        }
    } else if (comando == "listar_secuencias") {
        genoma.ListarSecuencias();
    } else if (comando == "histograma") {
        genoma.ObtenerSecuencia(parametros);
    } else if (comando == "es_subsecuencia") {
        genoma.ExisteSubsecuencia(parametros);
    } else if (comando == "enmascarar") {
        genoma.EnmascararSubsecuencia(parametros);
    } else if (comando == "guardar") {
        Archivo a;
        a.guardar(genoma, parametros);
    } else if(comando == "codificar"){
        if (parametros.empty()) {
            std::cout << "Debe indicar un nombre para el archivo de salida\n";
        } else {
            genoma.codificar(parametros);
        }
    } else if (comando == "decodificar"){
        if (parametros.empty()) {
            std::cout << "Debe indicar un archivo a decodificar\n";
        } else { 
            genoma.decodificar(parametros);
        }
    } 
    // === NUEVOS COMANDOS COMPONENTE 3 ===
    else if (comando == "ruta_mas_corta") {
        std::istringstream streamParams(parametros);
        std::string desc;
        int i, j, x, y;
        
        // El nombre de la secuencia es el primer token
        streamParams >> desc;
        
        // Intentamos leer las 4 coordenadas
        if (streamParams >> i >> j >> x >> y) {
            genoma.ruta_mas_corta(desc, i, j, x, y);
        } else {
            std::cout << "Uso incorrecto. Formato: ruta_mas_corta <secuencia> <i> <j> <x> <y>\n";
        }
    } 
    else if (comando == "base_remota") {
        std::istringstream streamParams(parametros);
        std::string desc;
        int i, j;
        
        streamParams >> desc;
        
        if (streamParams >> i >> j) {
            genoma.base_remota(desc, i, j);
        } else {
            std::cout << "Uso incorrecto. Formato: base_remota <secuencia> <i> <j>\n";
        }
    }
    // ====================================
    else if (comando == "ayuda") {
        procesarAyuda(parametros);
    } else {
        std::cout << "Comando no reconocido o opcion incorrecta.\n";
    }
}

void ayudaGeneral() {
    std::cout << "Comandos disponibles:\n";
    for (const std::string& s : comandospermitidos) {
        std::cout << "  " << s << "\n";
    }
}

void ayudaComando(const std::string& comando) {
    if (comando == "cargar") {
        std::cout << "Uso: cargar nombre_archivo (sin extension .txt)\nCarga las secuencias en memoria.\n";
    } else if (comando == "listar_secuencias") {
        std::cout << "Lista todas las secuencias cargadas en memoria.\n";
    } else if (comando == "histograma") {
        std::cout << "Genera el histograma de frecuencias de la secuencia indicada.\n";
    } else if (comando == "es_subsecuencia") {
        std::cout << "Verifica si una subsecuencia está contenida en las secuencias cargadas.\n";
    } else if (comando == "enmascarar") {
        std::cout << "Enmascara (reemplaza con X) una subsecuencia dada.\n";
    } else if (comando == "guardar") {
        std::cout << "Guarda las secuencias actuales en un archivo nuevo.\n";
    } else if (comando == "codificar"){
        std::cout << "Comprime las secuencias usando Huffman en un archivo binario.\n";
    } else if (comando == "decodificar"){
        std::cout << "Descomprime un archivo binario y carga las secuencias en memoria.\n";
    } else if (comando == "ruta_mas_corta") {
        std::cout << "Calcula la ruta de costo mínimo entre dos bases [i,j] y [x,y].\nUso: ruta_mas_corta <nombre> <i> <j> <x> <y>\n";
    } else if (comando == "base_remota") {
        std::cout << "Busca la base idéntica más lejana a la posición [i,j].\nUso: base_remota <nombre> <i> <j>\n";
    } else if (comando == "ayuda") {
        std::cout << "Muestra la lista de comandos o ayuda específica.\n";
    } else {
        std::cout << "Comando no reconocido.\n";
    }
}

void procesarAyuda(const std::string& parametro) {
    if (!parametro.empty()) {
        ayudaComando(parametro);
    } else {
        ayudaGeneral();
    }
}

int main() {
    std::string linea;
    Genoma genoma;

    std::cout << "=== Bienvenido al sistema de gestión de genomas ===\n";
    std::cout << "Escriba 'ayuda' para ver los comandos disponibles.\n";

    while (true) {
        std::cout << "\n$ ";
        if (!std::getline(std::cin, linea)) break; // Manejo de EOF (Ctrl+D)

        if (linea == "salir") {
            std::cout << "Saliendo del programa.\n";
            break;
        }
        if (linea.empty()) continue;

        procesarComando(linea, genoma);
    }

    return 0;
}
