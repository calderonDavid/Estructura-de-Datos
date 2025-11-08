#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <string>
#include <vector>
#include <list>
#include "Secuencia.h"

class Histograma {
public:

    //Datos minimos 
    std::vector<char> codigos;

    std::vector<int> resultado;
    
    //constructor 
    Histograma();
    // Operaciones
    void ordenSegunTabla(const Secuencia& s);
};

#endif
