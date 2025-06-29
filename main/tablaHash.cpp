#include <iostream>
#include <string>
#include <cmath>
using namespace std;


struct Persona {
    int dni;
    string rol;
    string zona;
    string horaEntrada;


    Persona(int d = -1, string r = "", string z = "", string h = "")
        : dni(d), rol(r), zona(z), horaEntrada(h) {}
};

class TablaHash {
private:
    Persona* tabla;  // Array de Personas
    int tam;         // Tamaño actual de la tabla
    int cantidad;    // Número de elementos almacenados


    int funcionHash(int clave) {
        return abs(clave) % tam;
    }


    void rehash() {
        int nuevoTam = tam * 2;
        Persona* nuevaTabla = new Persona[nuevoTam];

        // Inicializar nueva tabla
        for (int i = 0; i < nuevoTam; i++) {
            nuevaTabla[i] = Persona();
        }


        for (int i = 0; i < tam; i++) {
            if (tabla[i].dni != -1) {
                int pos = funcionHash(tabla[i].dni);
                while (nuevaTabla[pos].dni != -1) {
                    pos = (pos + 1) % nuevoTam;
                }
                nuevaTabla[pos] = tabla[i];
            }
        }

        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
    }

public:

    TablaHash(int t) : tam(t), cantidad(0) {
        tabla = new Persona[tam];
        for (int i = 0; i < tam; i++) {
            tabla[i] = Persona();
        }
    }


    void insertar(int dni, string rol, string zona, string hora) {
        if ((cantidad + 1) * 100 / tam > 70) {
            rehash();
        }

        int pos = funcionHash(dni);
        while (tabla[pos].dni != -1 && tabla[pos].dni != dni) {
            pos = (pos + 1) % tam;
        }

        if (tabla[pos].dni == -1) cantidad++;
        tabla[pos] = Persona(dni, rol, zona, hora);
    }


    Persona* buscar(int dni) {
        int pos = funcionHash(dni);
        int intentos = 0;
        while (tabla[pos].dni != -1 && intentos < tam) {
            if (tabla[pos].dni == dni) {
                return &tabla[pos];
            }
            pos = (pos + 1) % tam;
            intentos++;
        }
        return nullptr;
    }


    bool actualizarHora(int dni, string nuevaHora) {
        Persona* p = buscar(dni);
        if (p) {
            p->horaEntrada = nuevaHora;
            return true;
        }
        return false;
    }
};
