#ifndef ARBOLHOFFMAN_H
#define ARBOLHOFFMAN_H

#include "NodoHoffman.h"
#include <string>
#include <map>
#include <list>

class ArbolHoffman {
public:
    NodoHoffman* raiz;

    ArbolHoffman();
    ~ArbolHoffman(); // Destructor para liberar la memoria

    // La función principal para construir las bases y sus frecuencias "T - 5"
    void construirHojas(std::map<char, int> basesFrecuencias);


    void codigosRegistrados(std::map<char, std::string>& ASCIIV2);

private:
 
 
    void generarCodigosRec(NodoHoffman* nodo, std::string codigoActual, std::map<char, std::string>& ASCIIV2);
    
    
    void destruirArbol(NodoHoffman* nodoH);
};

#endif
