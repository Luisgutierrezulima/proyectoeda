#include <iostream>
#include "Persona.h"
using namespace std;

class TablaHash {
private:
    Persona** tabla;
    int tam;

public:
    TablaHash(int t) {
        tam = t;
        tabla = new Persona*[tam];
        for (int i = 0; i < tam; i++)
            tabla[i] = nullptr;
    }

    int funcionHash(int clave) {
        return abs(clave % tam);
    }

    void insertar(Persona* p) {
        int pos = funcionHash(p->dni);
        int intentos = 0;
        while (tabla[pos] != nullptr && intentos < tam)
            pos = (pos + 1) % tam;
            intentos++;
        if (intentos < tam) {
            tabla[pos] = p;
        } else {
            cout << "Tabla hash llena, no se puede insertar." << endl;
        }
    }

    Persona* buscar(int dni) {
        int pos = funcionHash(dni);
        int intentos = 0;
        while (tabla[pos] != nullptr && intentos < tam) {
            if (tabla[pos]->dni == dni)
                return tabla[pos];
            pos = (pos + 1) % tam;
            intentos++;
        }
        return nullptr;
    }

    Persona* obtenerPos(int i){
        if (i < 0 || i >= tam) {
            cout << "Indice fuera de rango." << endl;
            return nullptr;
        }
        return tabla[i];
    }

};
