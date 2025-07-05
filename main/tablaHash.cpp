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

    double factorCarga() {
        int ocupados = 0;
        for (int i = 0; i < tam; i++) {
            if (tabla[i] != nullptr)
                ocupados++;
        }
        return (double)ocupados / tam;
    }
    void rehashing() {
        int nuevoTam = tam * 2;
        Persona** nuevaTabla = new Persona*[nuevoTam];
        for (int i = 0; i < nuevoTam; i++)
            nuevaTabla[i] = nullptr;

        for (int i = 0; i < tam; i++) {
            if (tabla[i] != nullptr) {
                int pos = abs(tabla[i]->dni % nuevoTam);
                int intentos = 0;
                while (nuevaTabla[pos] != nullptr && intentos < nuevoTam) {
                    pos = (pos + 1) % nuevoTam;
                    intentos++;
                }
                if (intentos < nuevoTam) {
                    nuevaTabla[pos] = tabla[i];
                }
            }
        }
        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
        cout << "Rehashing realizado. Nuevo tamaño: " << tam << endl;
    }

    int funcionHash(int clave) {
        return abs(clave % tam);
    }

    void insertar(Persona* p) {
        if (factorCarga() > 0.7) {
            rehashing();
        }
        int pos = funcionHash(p->dni);
        int intentos = 0;
        while (tabla[pos] != nullptr && intentos < tam) {
            pos = (pos + 1) % tam;
            intentos++;
        }
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
