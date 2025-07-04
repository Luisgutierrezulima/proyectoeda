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


        void actualizarPrioridad(int dni, const string& nuevoRol) {
    for (int i = 1; i <= ultimo; i++) {
        if (heap[i]->dni == dni) {
            string rolAntiguo = heap[i]->rol;
            heap[i]->rol = nuevoRol;

            if (obtenerPrioridad(nuevoRol) > obtenerPrioridad(rolAntiguo)) {
                // Prioridad subió → subir en el heap
                int j = i;
                while (j > 1 && obtenerPrioridad(heap[j]->rol) > obtenerPrioridad(heap[j/2]->rol)) {
                    intercambiar(j, j/2);
                    j /= 2;
                }
            } else {
                // Prioridad bajó → bajar en el heap
                int j = i;
                while (2*j <= ultimo) {
                    int h = 2*j;
                    if (h+1 <= ultimo && obtenerPrioridad(heap[h+1]->rol) > obtenerPrioridad(heap[h]->rol))
                        ++h;
                    if (obtenerPrioridad(heap[j]->rol) > obtenerPrioridad(heap[h]->rol))
                        break;
                    intercambiar(j, h);
                    j = h;
                }
            }

            cout << "Prioridad actualizada para DNI: " << dni << endl;
            return;
        }
    }

    cout << "No se encontró a la persona con DNI: " << dni << endl;
}


   void verSiguientes(int cantidad) {
    if (ultimo == 0) {
        cout << "No hay asistentes en cola." << endl;
        return;
    }

    // 1. Copiar el heap original
    Persona** copia = new Persona*[tam + 1];
    for (int i = 1; i <= ultimo; i++)
        copia[i] = heap[i];
    int tempUltimo = ultimo;

    // 2. Extraer los siguientes `cantidad` elementos
    cout << "Próximos " << cantidad << " asistentes:\n";
    for (int i = 0; i < cantidad && tempUltimo > 0; i++) {
        Persona* top = copia[1];  // mayor prioridad
        cout << "- " << top->nombre << " (" << top->rol << ", DNI: " << top->dni << ")\n";

        // Simular extracción sin modificar el heap real
        copia[1] = copia[tempUltimo--];  // mover el último a la raíz
        int j = 1;

        // heapify-down en la copia
        while (2*j <= tempUltimo) {
            int h = 2*j;
            if (h+1 <= tempUltimo && obtenerPrioridad(copia[h+1]->rol) > obtenerPrioridad(copia[h]->rol))
                ++h;
            if (obtenerPrioridad(copia[j]->rol) > obtenerPrioridad(copia[h]->rol))
                break;
            swap(copia[j], copia[h]);
            j = h;
        }
    }

    delete[] copia;
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
