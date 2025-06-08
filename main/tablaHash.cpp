
#include <iostream>
#include <cmath>
using namespace std;

class TablaHash {
private:
    int* tabla;
    int tam;
    int cantidad;

    int funcionHash(int clave) {
        return abs(clave) % tam;
    }

    void rehash() {
        int nuevoTam = tam * 2;
        int* nuevaTabla = new int[nuevoTam];
        for (int i = 0; i < nuevoTam; i++)
            nuevaTabla[i] = -1;

        for (int i = 0; i < tam; i++) {
            if (tabla[i] != -1) {
                int clave = tabla[i];
                int pos = abs(clave) % nuevoTam;
                while (nuevaTabla[pos] != -1)
                    pos = (pos + 1) % nuevoTam;
                nuevaTabla[pos] = clave;
            }
        }

        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
    }

public:
    TablaHash(int t) {
        tam = t;
        cantidad = 0;
        tabla = new int[tam];
        for (int i = 0; i < tam; i++)
            tabla[i] = -1;
    }

    void insertar(int clave) {
        if ((cantidad + 1) * 100 / tam > 70) // Umbral del 70%
            rehash();
        int pos = funcionHash(clave);
        while (tabla[pos] != -1)
            pos = (pos + 1) % tam;
        tabla[pos] = clave;
        cantidad++;
    }

    bool buscar(int clave) {
        int pos = funcionHash(clave);
        int intentos = 0;
        while (tabla[pos] != -1 && intentos < tam) {
            if (tabla[pos] == clave)
                return true;
            pos = (pos + 1) % tam;
            intentos++;
        }
        return false;
    }

    void imprimir() {
        for (int i = 0; i < tam; i++) {
            cout << i << ": ";
            if (tabla[i] == -1)
                cout << "Vacio" << endl;
            else
                cout << tabla[i] << endl;
        }
    }
};
