#include "Genoma.h"
#include "winRAR.h"
#include "ArbolHoffman.h"
#include <fstream>
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
    int anchoaux= conjunto.begin()->anchoLinea;
    std::list<Secuencia>:: iterator it;
    for ( it = conjunto.begin(); it != conjunto.end(); it++) {
    // se pasa de char a string para manejarlo mas facil..
        std::string secuencia_string(it->lineas.begin(), it->lineas.end());
        int contInt = 0 ;
        for (int i = 0; i + sub.size() <= secuencia_string.size(); i++) {
        	//se recorre de tal forma que toma una posicion evalua, y sigue a la siguiente posicion...
        	//se evalua si apartir de esa posicion existe un substring que cumpla el mismo tamaño y sea igual..

            contInt++;
            if (secuencia_string.substr(i, sub.size()) == sub && contInt!= anchoaux) {
                contador++;
                existe = true;
            }
            if (contInt == anchoaux) {
                contInt= 0;
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

void Genoma::codificar(std::string& salidaArchivo){
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria." << std::endl;
        return;
    }

    std::map<char, int> frecuencia;
    for (Secuencia c : conjunto) {
        for (char base : c.lineas) {
            frecuencia[base]++;
        }
    }
    ArbolHoffman arbol;
    arbol.construirHojas(frecuencia);
    
    std::map<char, std::string> ASCIIV2;
    arbol.codigosRegistrados(ASCIIV2);
    
    std::ofstream archivoSalida(salidaArchivo + ".fabin", std::ios::binary);
    if (!archivoSalida.is_open()) {
        std::cerr << "No se pueden guardar las secuencias cargadas en " << salidaArchivo << std::endl;
        return;
    }
    
    // Escribir cabecera (con tipos simplificados)
    short n_lineas = frecuencia.size();
    archivoSalida.write(reinterpret_cast<char*>(&n_lineas), sizeof(short));
    for (auto par : frecuencia) {
        archivoSalida.write(&par.first, sizeof(char));
        archivoSalida.write(reinterpret_cast<char*>(&par.second), sizeof(int));
    }
    
    int n_secuencias = conjunto.size();
    archivoSalida.write(reinterpret_cast<char*>(&n_secuencias), sizeof(int));

    winRAR codificador;
    for (Secuencia c : conjunto) {
        short longNombre = c.Nombresecuencia.length();
        archivoSalida.write(reinterpret_cast<char*>(&longNombre), sizeof(short));
        archivoSalida.write(c.Nombresecuencia.c_str(), longNombre);
        
        int long_linea = c.lineas.size();
        archivoSalida.write(reinterpret_cast<char*>(&long_linea), sizeof(int));
        
        short ancho = c.anchoLinea;
        archivoSalida.write(reinterpret_cast<char*>(&ancho), sizeof(short));
        
        std::string bases(c.lineas.begin(), c.lineas.end());
        std::vector<unsigned char> v_datos = codificador.codifi(bases, ASCIIV2);
        
        int tamano_codificado = v_datos.size();
        archivoSalida.write(reinterpret_cast<char*>(&tamano_codificado), sizeof(int));
        archivoSalida.write(reinterpret_cast<char*>(v_datos.data()), v_datos.size());
    }
    
    archivoSalida.close();
    std::cout << "Secuencias codificadas y almacenadas en " << salidaArchivo << std::endl;
}


void Genoma::decodificar(const std::string& nombreArchivo) {

    std::ifstream archivoEntrada(nombreArchivo+".fabin", std::ios::binary);
    if (!archivoEntrada.is_open()) {
        std::cerr << "No se pueden cargar las secuencias desde " << nombreArchivo << std::endl;
        return;
    }
    
    conjunto.clear();
    
    std::map<char, int> frecuencia;
    short n_lineas;
    archivoEntrada.read(reinterpret_cast<char*>(&n_lineas), sizeof(short));
    
    for (int i = 0; i < n_lineas; ++i) {
        char base;
        int freq;
        archivoEntrada.read(&base, sizeof(char));
        archivoEntrada.read(reinterpret_cast<char*>(&freq), sizeof(int));
        frecuencia[base] = freq;
    }
    
    ArbolHoffman arbol;
    arbol.construirHojas(frecuencia);
    if (arbol.raiz == nullptr) {
        std::cerr << "Error: árbol de Hoffman vacío tras construir hojas.\n";
        // no return for now, pero cuidado si decodifi espera arbol no nulo
    }

    int n_conjunto;
    archivoEntrada.read(reinterpret_cast<char*>(&n_conjunto), sizeof(int));
    
    winRAR decodificador;
    
    for (int i = 0; i < n_conjunto; ++i) {
        short nombre_len;
        archivoEntrada.read(reinterpret_cast<char*>(&nombre_len), sizeof(short));
        
        std::string nombre_secuencia(nombre_len, '\0');
        archivoEntrada.read(&nombre_secuencia[0], nombre_len);

        int secuencia_len_original;
        archivoEntrada.read(reinterpret_cast<char*>(&secuencia_len_original), sizeof(int));
        
        short ancho;
        archivoEntrada.read(reinterpret_cast<char*>(&ancho), sizeof(short));

        int tamano_codificado;
        archivoEntrada.read(reinterpret_cast<char*>(&tamano_codificado), sizeof(int));

        std::vector<unsigned char> v_datos(tamano_codificado);
        archivoEntrada.read(reinterpret_cast<char*>(v_datos.data()), tamano_codificado);
        
        std::string conjuntoDecodificado = decodificador.decodifi(v_datos, secuencia_len_original, arbol.raiz);
        
        std::list<char> lineas(conjuntoDecodificado.begin(), conjuntoDecodificado.end());
        
        bool completa = true;
        for(char base : lineas) { if (base == '-') { completa = false; break; } }
        
        Secuencia s(nombre_secuencia, lineas, ancho, completa);
        conjunto.push_back(s);
    }
    archivoEntrada.close();
    std::cout << "Secuencias decodificadas desde " << nombreArchivo << ".fabin y cargadas en memoria." << std::endl;
}

