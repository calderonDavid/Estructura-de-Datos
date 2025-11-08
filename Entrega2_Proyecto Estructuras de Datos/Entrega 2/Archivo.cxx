#include "Archivo.h"
#include "Genoma.h"
#include "Secuencia.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>

//Constructor vacio
Archivo::Archivo() {}

//Metodo para leer el archivo 
Genoma Archivo::leer(const std::string &ruta) {
    //Genoma que va a guardar toda la informacion 
    Genoma genoma;
    //La lista de bases {A,P,O,R,...}
    std::list<char> secuencias;
   
   //lECTURA del archvio
    std::ifstream in(ruta+".txt");
	
   //Evalua si se puede abrir el archivo
    if (!in.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << ruta << std::endl;
        return genoma;
    }
    //VAriables para el llenado de Genoma, lectura y conteo de secuencias
    int cantSec = 1;
    std::string linea;
    std::string name;
    int ancho = 0;
    bool completo = true;
    int primeraLinea=0;
	
	//Mientras que pueda leer...
    while (std::getline(in, linea)) {
        if (linea.empty())
            continue;
	//Al tomar la linea se evalua si es el nombre de una secuencia
        if (linea[0] == '>') {
            if (!name.empty()) {
            	//Se guarda la informacion de la iteracion anterior (ya leyo un nombre "name" )
                cantSec++;
                Secuencia secu(name, secuencias, ancho, completo);
         	//se añade a genoma
                genoma.conjunto.push_back(secu);
           
            }
            //Se reinicia la informacion, para la siguiente secuencia
            secuencias.clear();
            ancho = 0;
            completo = true;
            name = linea.substr(1); 
            primeraLinea=0;
            //Codigo generado por IA:
            // sirve para limpiar una cadena de caracteres eliminando espacios en blanco y tabulaciones del inicio y el 		final.
	    name.erase(0, name.find_first_not_of(" \t\r\n"));
	    name.erase(name.find_last_not_of(" \t\r\n") + 1);

        } else {
        //agrupa todos los caracteres no deseados (los espacios) al final de la cadena y erase los borra de forma definitiva.
            linea.erase(std::remove_if(linea.begin(), linea.end(), ::isspace), linea.end());

            for (char c : linea) {
            //evalua si esta en la primera linea y en caso de ser asi, toma el tamaño que tiene cada linea de secuencia
                if(primeraLinea==0) ancho=linea.size();
                //evalua si esta completa o no
                if(c=='-')completo=false;
                //en caso de que si, lo añade a secuencias
                secuencias.push_back(c);
            }
            primeraLinea++;
        }
        
    }

    if (!name.empty()) { 
        Secuencia secu(name, secuencias, ancho, completo);
        genoma.conjunto.push_back(secu);
    }

    if (cantSec > 1) {
        std::cout << cantSec << " secuencias cargadas correctamente desde " << ruta << "\n";
    } else if (cantSec == 1) {
        std::cout << cantSec << " secuencia cargada correctamente desde " << ruta << "\n";
    } else {
        std::cout << ruta << " no contiene ninguna secuencia.\n";
    }

    return genoma;
}
//Metodo para escribir archivos 
//recibe el objeto y el nombre del archivo 
void Archivo::guardar(const Genoma& genoma, std::string nombreArchivoGuardado) { 
    //se saca la lista de secuencias que tiene todo el genoma
    const std::list<Secuencia>& conjunto = genoma.getConjunto();

    if (conjunto.empty()) {
        std::cout << "No hay secuencias en memoria para guardar.\n";
        return;
    }
    //establece la variable out para escritura...
    std::ofstream out(nombreArchivoGuardado + ".fa");
    if (!out.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << nombreArchivoGuardado << ".fa" << std::endl;
        return;
    }
    std::string nombresecuencia;
    //recorre todas las secuencias que hay 
    for (Secuencia sec : conjunto) {
 	nombresecuencia= sec.Obtenernombre();
 	out << ">"+ nombresecuencia+"\n";
 	int n=0;
 	// Se itera sobre las bases 
        for (char c : sec.lineas) {
        // Sirve para evaluar el largo de las lineas...
       	    if(n==sec.anchoLinea){
       	    	out << "\n";
       	    	n=0;// se reinicia
       	    }
       	    n++;
            out << c;
        }
        out << '\n';
    }
	//se cierra el "enlace"
    out.close();
    std::cout << "Archivo guardado correctamente como " << nombreArchivoGuardado << ".fa\n";
}
