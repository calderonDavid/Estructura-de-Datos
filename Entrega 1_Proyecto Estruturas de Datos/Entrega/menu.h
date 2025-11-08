#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <iostream>
#include "Genoma.h"

// Lista de comandos permitidos
//el extern funciona parecido a una variable global
extern std::vector<std::string> comandospermitidos;

// Declaraciones de funciones
void procesarComando(const std::string& linea, Genoma &genoma);
void ayudaGeneral();
void ayudaComando(const std::string& comando);
void procesarAyuda(const std::string& parametro);

#endif
