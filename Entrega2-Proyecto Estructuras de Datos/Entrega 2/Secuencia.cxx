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
//Metdo para contar y que devuelve cuantas veces se repite una base
int Secuencia::ContarBase(char b) {
    int count = 0;
    for (char c : lineas) {
        if (c == b) {
            count++;
        }
    }
    return count;
}


