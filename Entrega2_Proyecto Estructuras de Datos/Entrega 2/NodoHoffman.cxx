#include "NodoHoffman.h"

// Constructor hoja
NodoHoffman::NodoHoffman(char s, int f) {
    simbolo = s;
    frecuencia = f;
    izq = nullptr;
    der = nullptr;
}

// Constructor interno (padre)
NodoHoffman::NodoHoffman(NodoHoffman *I, NodoHoffman *D) {
    simbolo = 0;
    frecuencia = I->frecuencia + D->frecuencia;
    izq = I;
    der = D;
}

// Destructor
NodoHoffman::~NodoHoffman() {}

bool NodoHoffman::esHoja() {
    return der == nullptr && izq == nullptr;
}

