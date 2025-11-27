#ifndef WINRAR_H
#define WINRAR_H

#include <string>
#include <vector>
#include <map>
#include "NodoHoffman.h"

class winRAR {
public:
    std::vector<unsigned char> codifi(std::string bases, std::map<char, std::string>& ASCCIV2);
    
    std::string decodifi(std::vector<unsigned char>& v_datos, int tamano, NodoHoffman* raiz);
};

#endif
