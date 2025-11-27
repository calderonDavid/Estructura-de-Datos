#include "Secuencia.h"

// Constructor
Secuencia::Secuencia(std::string nombre, std::list<char> linea, int ancho, bool complet)
    : Nombresecuencia(nombre), lineas(linea), anchoLinea(ancho), completo(complet) {}
    
//Getters..

std::string Secuencia::Obtenernombre() const {
    return Nombresecuencia;
}

int Secuencia::ObtenerLongitud() {
    return anchoLinea;
}

bool Secuencia::EsCompleta() const {
    return completo;
}

//Metodo para contar y que devuelve cuantas veces se repite una base
int Secuencia::ContarBase(char b) {
    int count = 0;
    for (char c : lineas) {
        if (c == b) {
            count++;
        }
    }
    return count;
}

// NUEVO METODO: Implementación para convertir lista a matriz
std::vector<std::vector<char>> Secuencia::obtenerMatriz() const {
    std::vector<std::vector<char>> matriz;
    if (lineas.empty() || anchoLinea <= 0) return matriz;

    std::vector<char> filaActual;
    int contador = 0;

    for (char base : lineas) {
        filaActual.push_back(base);
        contador++;
        
        // Si alcanzamos el ancho de linea, guardamos la fila y empezamos una nueva
        if (contador == anchoLinea) {
            matriz.push_back(filaActual);
            filaActual.clear();
            contador = 0;
        }
    }
    // Si sobran elementos (última fila incompleta), los agregamos
    if (!filaActual.empty()) {
        matriz.push_back(filaActual);
    }

    return matriz;
}
