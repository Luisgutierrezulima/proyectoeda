
#include <iostream>
using namespace std;

class MaxHeap {
private:
    pair<int, int>* heap;
    int tam;
    int ultimaPosicion;

    void swap(pair<int, int>& a, pair<int, int>& b) {
        pair<int, int> temp = a;
        a = b;
        b = temp;
    }

    int buscarIndice(int id) {
        for (int i = 1; i <= ultimaPosicion; i++) {
            if (heap[i].second == id)
                return i;
        }
        return -1;
    }

public:
    MaxHeap(int n) {
        heap = new pair<int, int>[n + 1]; // +1 para evitar acceso fuera de índice 0
        tam = n + 1;
        ultimaPosicion = 0;
    }

    void insertar(int id, int prioridad) {
        if (ultimaPosicion + 1 >= tam) {
            cout << "Heap lleno. No se puede insertar más elementos." << endl;
            return;
        }
        ultimaPosicion++;
        heap[ultimaPosicion] = {prioridad, id};
        int i = ultimaPosicion;
        while (i > 1 && heap[i].first > heap[i / 2].first) {
            swap(heap[i], heap[i / 2]);
            i = i / 2;
        }
    }

    int extraerMax() {
        if (ultimaPosicion == 0)
            return -1;
        int idMax = heap[1].second;
        heap[1] = heap[ultimaPosicion--];
        int i = 1;
        while (true) {
            int left = i * 2, right = i * 2 + 1, largest = i;
            if (left <= ultimaPosicion && heap[left].first > heap[largest].first)
                largest = left;
            if (right <= ultimaPosicion && heap[right].first > heap[largest].first)
                largest = right;
            if (largest != i) {
                swap(heap[i], heap[largest]);
                i = largest;
            } else break;
        }
        return idMax;
    }

    void actualizarPrioridad(int id, int nuevaPrioridad) {
        int idx = buscarIndice(id);
        if (idx == -1) {
            cout << "ID no encontrado en el heap." << endl;
            return;
        }
        int viejaPrioridad = heap[idx].first;
        heap[idx].first = nuevaPrioridad;

        if (nuevaPrioridad > viejaPrioridad) {
            while (idx > 1 && heap[idx].first > heap[idx / 2].first) {
                swap(heap[idx], heap[idx / 2]);
                idx /= 2;
            }
        } else {
            while (true) {
                int left = idx * 2, right = idx * 2 + 1, largest = idx;
                if (left <= ultimaPosicion && heap[left].first > heap[largest].first)
                    largest = left;
                if (right <= ultimaPosicion && heap[right].first > heap[largest].first)
                    largest = right;
                if (largest != idx) {
                    swap(heap[idx], heap[largest]);
                    idx = largest;
                } else break;
            }
        }
    }

    void mostrarTop(int n) {
        for (int i = 1; i <= ultimaPosicion && i <= n; i++) {
            cout << i << ". ID: " << heap[i].second << " (prioridad " << heap[i].first << ")" << endl;
        }
    }
};
