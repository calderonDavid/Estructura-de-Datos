#include "ArbolHoffman.h"
#include "NodoHoffman.h"
ArbolHoffman::ArbolHoffman() {
    raiz = nullptr;
}

ArbolHoffman::~ArbolHoffman() {
    destruirArbol(raiz);
}

void ArbolHoffman::destruirArbol(NodoHoffman* nodoH) {
    if (nodoH != nullptr) {
        destruirArbol(nodoH->izq);
        destruirArbol(nodoH->der);
        delete nodoH;
    }
}

void ArbolHoffman::construirHojas(std::map<char, int> basesFrecuencias) {
    
    std::list<NodoHoffman*> nodosH;
    std::map<char, int>::iterator it;

    for (it =basesFrecuencias.begin(); it!=basesFrecuencias.end(); ++it) {
    nodosH.push_back(new NodoHoffman(it->first, it->second));}

    while (nodosH.size() > 1) {
        
        nodosH.sort([](NodoHoffman* a, NodoHoffman* b) { return a->frecuencia < b->frecuencia; });

        NodoHoffman* Izq = nodosH.front();
        nodosH.pop_front();
        
        NodoHoffman* Der = nodosH.front();
        nodosH.pop_front();


        NodoHoffman* x = new NodoHoffman(Izq, Der);
        

        nodosH.push_back(x);
    }

    raiz = nodosH.front();
}


void ArbolHoffman::codigosRegistrados(std::map<char, std::string>& ASCCIV2) {
    generarCodigosRec(raiz, "", ASCCIV2);
}

void ArbolHoffman::generarCodigosRec(NodoHoffman* nodoH, std::string c, std::map<char, std::string>& ASCCIV2) {
    if (nodoH == nullptr) {
        return;
    }

    if (nodoH->esHoja()) {
        ASCCIV2[nodoH->simbolo] = c;
        return;
    }

    generarCodigosRec(nodoH->izq, c+"0", ASCCIV2);
    generarCodigosRec(nodoH->der, c+"1", ASCCIV2);
}
