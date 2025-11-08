#include "Genoma.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>

// Constructor
Genoma::Genoma(std::list<Secuencia> conjunto, int cantSecuencias) 
    : conjunto(conjunto), cantSecuencias(cantSecuencias) {}

// Operaciones
const std::list<Secuencia>& Genoma::getConjunto() const {
    return conjunto;
}
//getters y setters..
void Genoma::setConjunto(const std::list<Secuencia>& nuevoConjunto) {
    conjunto = nuevoConjunto;
}

int Genoma::getCantSecuencias() const {
    return cantSecuencias;
}

void Genoma::setCantSecuencias(int cantidad) {
    cantSecuencias = cantidad;
}
//Listar las secuencias que tiene el genoma
void Genoma::ListarSecuencias() {
	//evalua si hay secuencias cargadas 
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
    } else {
        std::cout << "Hay " << conjunto.size() << " secuencias cargadas en memoria:\n";
        //iterador para ir por cada secuencia del genoma
        std::list<Secuencia>::iterator it;
        for (it = conjunto.begin(); it != conjunto.end(); it++) {
        //dependiendo si esta completo o no, se imprime.. (nombre, tamaño)
            if (it->EsCompleta()) {
                std::cout << "Secuencia " << it->Obtenernombre()
                          << " contiene " << it->lineas.size() << " bases.\n";
            } else {
                std::cout << "Secuencia " << it->Obtenernombre()
                          << " contiene al menos " << it->lineas.size()<< " bases.\n";
            }
        }
    }
}
//Metodo para usar histograma, recibimos la secuencia 
void Genoma::ObtenerSecuencia(const std::string& nombre) {
    Histograma h;
    //iterador para ir por secuencias del genoma
    std::list<Secuencia>::iterator it;
    for (it = conjunto.begin(); it != conjunto.end(); it++) {
        if (it->Obtenernombre() == nombre) {
        //buscamos que la secuencia exista en genoma
            h.ordenSegunTabla(*it);
        }
    }
}
//Metodo para contar la cantidad de veces que sale una sub secuencia en las secuencias
void Genoma::ExisteSubsecuencia(std::string &sub) {
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    int contador = 0;
    bool existe = false;
     std::list<Secuencia>:: iterator it;
    for ( it = conjunto.begin(); it != conjunto.end(); it++) {
    // se pasa de char a string para manejarlo mas facil..
        std::string secuencia_string(it->lineas.begin(), it->lineas.end());

        for (int i = 0; i + sub.size() <= secuencia_string.size(); i++) {
        	//se recorre de tal forma que toma una posicion evalua, y sigue a la siguiente posicion...
        	//se evalua si apartir de esa posicion existe un substring que cumpla el mismo tamaño y sea igual..
            if (secuencia_string.substr(i, sub.size()) == sub) {
                contador++;
                existe = true;
            }
        }
    }
    // se imprime con base en si se encontro o no la sub secuencia
    if (existe) {
        std::cout << "La subsecuencia dada se repite " << contador 
                  << " veces dentro de las secuencias cargadas.\n";
    } else {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas.\n";
    }
}
//metodo para enmascarar una subsecuencia dada Funciona de forma general igual a "ExisteSubsecuencia"
void Genoma::EnmascararSubsecuencia(std::string &sub) {
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    bool encontrada = false;
	std::list<Secuencia>:: iterator it;
    for (it = conjunto.begin(); it != conjunto.end(); it++) {
    	// se pasa de char a string para manejarlo mas facil..
        std::string secuencia_string(it->lineas.begin(), it->lineas.end());
	
	//se recorre de tal forma que toma una posicion evalua, y sigue a la siguiente posicion...
        //se evalua si apartir de esa posicion existe un substring que cumpla el mismo tamaño y sea igual..
        for (int i = 0; i + sub.size() <= secuencia_string.size(); i++) {
            if (secuencia_string.substr(i, sub.size()) == sub) {
                encontrada = true;
                //desde la posicion que encontro la subsecuencua, pasar las bases a "x"
                for (int j = 0; j < sub.size(); j++) {
                    secuencia_string[i + j] = 'X';
                }
            }
        }
	//Codigo generado por IA
        // actualizar lineas en la secuencia
        //se limpia la secuencia anterior (sin enmascarar)
        it->lineas.clear();
        //se inserta la string que se divide en char.. con las sub secuencias ya enmascaradas
        it->lineas.insert(it->lineas.begin(), secuencia_string.begin(), secuencia_string.end());
    }
    //se imprime con base en si se pudo enmascarar o no 
    if (encontrada) {
        std::cout << "La subsecuencia se ha enmascarado correctamente.\n";
    } else {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas.\n";
    }
}
