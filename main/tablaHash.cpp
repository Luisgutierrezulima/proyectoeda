#include <iostream>
#include "Persona.h"
using namespace std;

class TablaHash {
private:
    Persona** tabla;
    int tam;

    int funcionHash(const string& clave) {
        int h = 0;
        for (char c : clave)
            h = (h * 31 + c) % tam;
        return h;
    }

public:
    TablaHash(int t) {
        tam = t;
        tabla = new Persona*[tam];
        for (int i = 0; i < tam; ++i)
            tabla[i] = nullptr;
    }

    void insertar(Persona* p) {
        int pos = funcionHash(p->dni);
        while (tabla[pos] != nullptr)
            pos = (pos + 1) % tam;
        tabla[pos] = p;
    }

    Persona* buscar(const string& dni) {
        int pos = funcionHash(dni);
        int intentos = 0;
        while (tabla[pos] != nullptr && intentos < tam) {
            if (tabla[pos]->dni == dni)
                return tabla[pos];
            pos = (pos + 1) % tam;
            ++intentos;
        }
        return nullptr;
    }
};
