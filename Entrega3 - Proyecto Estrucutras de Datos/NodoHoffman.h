#ifndef NODO_HOFFMAN_H
#define NODO_HOFFMAN_H

#include <cstdint>

class NodoHoffman {
public:
    char simbolo;   
    int frecuencia;     
    NodoHoffman *izq;        // hijo izquierdo
    NodoHoffman *der;        // hijo derecho
    
    
    // Constructores
    NodoHoffman( char s, int f);
    NodoHoffman(NodoHoffman *I, NodoHoffman *D);

    // Destructor
    ~NodoHoffman();
    
    bool esHoja();               
};

#endif
