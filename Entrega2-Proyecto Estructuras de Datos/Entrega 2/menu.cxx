	#include "menu.h"
	#include "Genoma.h"
	#include "Archivo.h"
	#include "Secuencia.h"
	#include <sstream>

	// Definición de los comandos permitidos
	std::vector<std::string> comandospermitidos = {
	    "1- cargar <nombre_archivo>",
	    "2- listar_secuencias  			//Muestra en Pantalla las secuencias del archivo",
	    "3- histograma <Nombresecuencia> 		//Ejemplo: histograma FuLL_SEQUENCE",
	    "4- es_subsecuencia <bases> 			//Ejemplo: AGT",
	    "5- enmascarar <bases> 			//Ejemplo: TGA",
	    "6- guardar <nombrearchivo guardado>",
	    "7- codificar <nombrearchivo salida>",
	    "8- decodificar <nombrearchivo codificado>",
	    "9- ayuda <comando> 				//*opcional",
	    "10- salir"
	};
	// Sirve para verificar que operacion se va a realizar, recibiendo la linea (comando) y el geneoma por referencia
	//Ya que es el que esta activo durante toda el funcionamiento
	void procesarComando(const std::string& linea,Genoma &genoma) {
	    std::istringstream iss(linea);
	    std::string comando;
	    std::string parametros;
	    
	    iss >> comando;
	    std::getline(iss, parametros);
	    if (!parametros.empty() && parametros[0] == ' ') {
		parametros = parametros.substr(1); // quitar espacio inicial
	    }


	    // Ejecutar comando
	    if (comando == "cargar") {
		if (parametros.empty()) {
		    std::cout << "Debe indicar un archivo .fa\n";
		} else {
		    Archivo a;
		    genoma = a.leer(parametros);
		    
		}
	    } else if (comando == "listar_secuencias") {
		genoma.ListarSecuencias();
	    } else if (comando == "histograma") {
		genoma.ObtenerSecuencia(parametros);
	    } else if (comando == "es_subsecuencia") {
		genoma.ExisteSubsecuencia(parametros);
	    } else if (comando == "enmascarar") {
		genoma.EnmascararSubsecuencia(parametros);
	    } else if (comando == "guardar") {
		Archivo a;
		a.guardar(genoma, parametros);
	    } else if(comando== "codificar"){
	    	if (parametros.empty()) {
		    std::cout << "Debe indicar un archivo .fa\n";
		} else {
		    genoma.codificar(parametros);
		}
	    } else if (comando== "decodificar"){
	    	if (parametros.empty()) {
		    std::cout << "Debe indicar un archivo .fa\n";
	    	} else { 
		    genoma.decodificar(parametros);
		}
	    } else if (comando == "ayuda") {
		procesarAyuda(parametros);
	    } else {
	    	std::cout<<"opcion incorrecta vuelva a empezar.\n";
	    }
	}
	//metodo para mostrar lo comando generales (cargar,hisotgrama,guardar...)
	void ayudaGeneral() {
	    std::cout << "Comandos disponibles:\n";
	    for (std::string s : comandospermitidos) {
		std::cout << "  " << s << "\n";
	    }
	}
	//Metodo pora especificar la ayuda a un comando asociado...
	void ayudaComando(const std::string& comando) {
	    if (comando == "cargar") {
		std::cout << "Uso: $cargar archivo.fa\n";
	    } else if (comando == "listar_secuencias") {
		std::cout << "Lista todas las secuencias cargadas en memoria.\n";
	    } else if (comando == "histograma") {
		std::cout << "Genera el histograma de la secuencia indicada.\n";
	    } else if (comando == "$es_subsecuencia") {
		std::cout << "Verifica si una subsecuencia está contenida.\n";
	    } else if (comando == "enmascarar") {
		std::cout << "Enmascara una subsecuencia dentro de las secuencias.\n";
	    } else if (comando == "guardar") {
		std::cout << "Guarda las secuencias en un archivo FASTA.\n";
	    } else if (comando == "ayuda") {
		std::cout << "Muestra esta ayuda.\n";
	    } else if (comando =="codificar"){
	    	std::cout << "Comprime el archivo FASTA en un archivo FASBIN.\n";
	    } else if (comando == "decodificar"){
	    	std::cout << "Descomprime un archivo con extension FASBIN a FASTA.\n";
	    }else{
		std::cout << "Comando no reconocido\n";
	    }
	}
	//Clasificar el tipo de ayuda, siendo la general o la especifica
	void procesarAyuda(const std::string& parametro) {
	    if (!parametro.empty()) {
		ayudaComando(parametro);
	    } else {
		ayudaGeneral();
	    }
	}

	int main() {
		//variables necesarias para el funcionamiento
	    std::string linea;
	    Genoma genoma;

	    std::cout << "=== Bienvenido al sistema de gestión de genomas ===\n";
	    std::cout << "Escriba '$ayuda' para ver los comandos disponibles.\n";

	    while (true) {
		std::cout << "\n$ ";
		//leer de consola...
		std::getline(std::cin, linea);

		if (linea == "salir") {
		    std::cout << "Saliendo del programa.\n";
		    break;
		}
		//Clasificar el comando...
		procesarComando(linea,genoma);
	    }

	    return 0;
	}
