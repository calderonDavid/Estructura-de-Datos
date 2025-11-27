#include "winRAR.h"

std::vector<unsigned char> winRAR::codifi(std::string bases, std::map<char, std::string>& ASCCIV2) {
    unsigned char temp = 0;
    int contB = 0;
    std::string bits;
    for (char c : bases) {bits += ASCCIV2[c];}
    
    std::vector<unsigned char> rta;

    for (char bit : bits) {
    
   	//Generado por IA...
	//Mover los bits a la derecha para completar un Byte
        temp = temp << 1;
        
        // Si el bit es '1', enciéndelo en la última posición
        if (bit == '1') {
            temp = temp | 1;
        }
        //HASTA ACÁ
        contB++;
        
        if (contB == 8) {
            rta.push_back(temp);
            contB = 0;
            temp = 0;
        }
    }
    
    if (contB > 0) {
        temp = temp << (8 - contB); // Mueve los bits al principio del byte
        rta.push_back(temp);
    }
    
    return rta;
}


std::string winRAR::decodifi(std::vector<unsigned char>& v_datos, int tamano, NodoHoffman* raiz) {
    std::string rta;
    NodoHoffman* nodoActual = raiz;
    
    for (unsigned char byte : v_datos) {
        // Dentro de cada byte, lee cada uno de sus 8 bits
        for (int i = 7; i >= 0; i--) {
	     //Generado por IA
            char bit = (byte >> i) & 1;
            
	    
            if (bit == 0) {
                nodoActual = nodoActual->izq;
            } else {
                nodoActual = nodoActual->der;
            }

            if (nodoActual->esHoja()) {
                rta += nodoActual->simbolo;
                nodoActual = raiz; 
            }
            
            if (rta.length() == tamano) {
                return rta;
            }
        }
    }
    
    return rta;
}
