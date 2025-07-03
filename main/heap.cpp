#include <iostream>
#include "Persona.h"
using namespace std;

int obtenerPrioridad(const string& rol) {
    if (rol == "VIP")          return 5;
    if (rol == "medico")       return 4;
    if (rol == "seguridad")    return 3;
    if (rol == "discapacitado")return 2;
    return 1;
}

class MaxHeap {
private:
    Persona** heap;
    int tam;
    int ultimo;

    void intercambiar(int i, int j) {
        Persona* tmp = heap[i];
        heap[i] = heap[j];
        heap[j] = tmp;
    }

public:
    MaxHeap(int capacidad) {
        tam    = capacidad;
        ultimo = 0;
        heap   = new Persona*[tam + 1];
    }

    void insertar(Persona* p) {
        if (ultimo + 1 >= tam) {
            cout << "Cola llena" <<endl;
            return;
        }
        heap[++ultimo] = p;
        int i = ultimo;
        while (i > 1 && obtenerPrioridad(heap[i]->rol) > obtenerPrioridad(heap[i/2]->rol)) {
            intercambiar(i, i/2);
            i /= 2;
        }
    }

    Persona* extraerMax() {
        if (ultimo == 0) return nullptr;
        Persona* top = heap[1];
        heap[1] = heap[ultimo--];
        int i = 1;
        while (2*i <= ultimo) {
            int h = 2*i;
            if (h+1 <= ultimo && obtenerPrioridad(heap[h+1]->rol) > obtenerPrioridad(heap[h]->rol))
                ++h;
            if (obtenerPrioridad(heap[i]->rol) >= obtenerPrioridad(heap[h]->rol)) break;
            intercambiar(i, h);
            i = h;
        }
        return top;
    }

    void mostrarTop(int n) {
        cout << "Proximos " << n << " en cola: "<<endl;
        for (int i = 1; i <= min(n, ultimo); ++i)
            cout << heap[i]->dni << " - " << heap[i]->rol << endl;
    }

    bool estaEnCola(int dni) {
        for (int i = 1; i <= ultimo; ++i) {
            if (heap[i]->dni == dni) 
            return true;
        }
        return false;
    }
};
