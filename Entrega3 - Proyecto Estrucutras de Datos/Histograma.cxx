#include "Histograma.h"
#include <iostream>
#include <vector>
#include <list>

Histograma::Histograma() : 
    codigos({'A', 'C', 'G', 'T', 'U', 'R', 'Y', 'K', 'M', 'S', 'W', 'B', 'D', 'H', 'V', 'N', 'X', '-'}){};
    
   
void Histograma::ordenSegunTabla(const Secuencia& s) {
    
    // En lugar de: resultado(codigos.size(), 0);
    //Generado con IA: 
    resultado.assign(codigos.size(), 0);
    /*Limpiar: Vacía el vector por completo, eliminando todos los elementos que tenía antes.

      Redimensionar: Le da al vector un nuevo tamaño, el cual tú especificas.

      Llenar: Rellena todas las nuevas posiciones con un valor predeterminado que también le indicas.*/
      
      //se evalua si hay lineas de secuencia
    if (s.lineas.empty()) {
        std::cout << "Secuencia inválida.\n";
        return;
    }
	// se itera sobre la lista de bases establecidad
    for (int i = 0; i < codigos.size(); i++) {
    	// se itera sobre los caracteres (bases) que tiene el genoma
    	std::list<char>::const_iterator it;
        for (it = s.lineas.begin(); it != s.lineas.end(); it++) {
        //se compara las bases establecidad y las que tiene la secuencia
            if (codigos[i] == *it) {
                resultado[i]++;
            }
        }
    }
	//se muestran los resultados 
    for (int j = 0; j < codigos.size(); j++) {
        std::cout << codigos[j] << " : " << resultado[j] << "\n";
    }
}
