#include <iostream>
#include <vector>
using namespace std;


class Node {
private:
    int dato;
    int altura;
    int contador;
    Node* izquierda;
    Node* derecha;

public:
    Node(int n) : dato(n), altura(1), contador(1), izquierda(nullptr), derecha(nullptr) {}


    int getDato() { return dato; }
    int getAltura() { return altura; }
    int getContador() { return contador; }
    Node*& getIzquierda() { return izquierda; }
    Node*& getDerecha() { return derecha; }


    void setDato(int valor) { dato = valor; }
    void setAltura(int valor) { altura = valor; }
    void setContador(int valor) { contador = valor; }
    void setIzquierda(Node* nodo) { izquierda = nodo; }
    void setDerecha(Node* nodo) { derecha = nodo; }
};

class ArbolAVL {
private:
    Node* raiz;


    int altura(Node* nodo) {
        return nodo ? nodo->getAltura() : 0;
    }


    int max(int a, int b) {
        return (a > b) ? a : b;
    }


    int factorBalance(Node* nodo) {
        return nodo ? altura(nodo->getIzquierda()) - altura(nodo->getDerecha()) : 0;
    }


    void actualizarAltura(Node* nodo) {
        int alt = 1 + max(altura(nodo->getIzquierda()), altura(nodo->getDerecha()));
        nodo->setAltura(alt);
    }


    Node* rotacionIzquierda(Node* actualRaiz) {
        Node* nuevaRaiz = actualRaiz->getDerecha();
        Node* subArbol = nuevaRaiz->getIzquierda();

        nuevaRaiz->setIzquierda(actualRaiz);
        actualRaiz->setDerecha(subArbol);

        actualizarAltura(actualRaiz);
        actualizarAltura(nuevaRaiz);

        return nuevaRaiz;
    }


    Node* rotacionDerecha(Node* actualRaiz) {
        Node* nuevaRaiz = actualRaiz->getIzquierda();
        Node* subArbol = nuevaRaiz->getDerecha();

        nuevaRaiz->setDerecha(actualRaiz);
        actualRaiz->setIzquierda(subArbol);

        actualizarAltura(actualRaiz);
        actualizarAltura(nuevaRaiz);

        return nuevaRaiz;
    }


    void insertar(Node*& nodo, int n) {
        if (nodo == nullptr) {
            nodo = new Node(n);
            return;
        }

        if (n == nodo->getDato()) {
            nodo->setContador(nodo->getContador() + 1);
            return;
        }

        if (n < nodo->getDato()) {
            insertar(nodo->getIzquierda(), n);
        } else {
            insertar(nodo->getDerecha(), n);
        }

        actualizarAltura(nodo);
        int balance = factorBalance(nodo);


        if (balance > 1 && n < nodo->getIzquierda()->getDato()) {
            nodo = rotacionDerecha(nodo);
        }

        else if (balance < -1 && n > nodo->getDerecha()->getDato()) {
            nodo = rotacionIzquierda(nodo);
        }

        else if (balance > 1 && n > nodo->getIzquierda()->getDato()) {
            nodo->setIzquierda(rotacionIzquierda(nodo->getIzquierda()));
            nodo = rotacionDerecha(nodo);
        }

        else if (balance < -1 && n < nodo->getDerecha()->getDato()) {
            nodo->setDerecha(rotacionDerecha(nodo->getDerecha()));
            nodo = rotacionIzquierda(nodo);
        }
    }

    bool buscar(Node* nodo, int n) {
        if (nodo == nullptr)
            return false;
        if (n == nodo->getDato())
            return true;
        if (n < nodo->getDato())
            return buscar(nodo->getIzquierda(), n);
        else
            return buscar(nodo->getDerecha(), n);
    }


    void buscarPorFranja(Node* nodo, int h1, int h2, vector<int>& resultados) {
        if (nodo == nullptr) return;

        int horaActual = nodo->getDato() / 10000;

        if (horaActual >= h1) {
            buscarPorFranja(nodo->getIzquierda(), h1, h2, resultados);
        }

        if (horaActual >= h1 && horaActual <= h2) {
            resultados.push_back(nodo->getDato());
        }

        if (horaActual <= h2) {
            buscarPorFranja(nodo->getDerecha(), h1, h2, resultados);
        }
    }


    void obtenerMayor(Node* nodo, int& maxDato, int& maxContador) {
        if (nodo == nullptr) return;
        obtenerMayor(nodo->getIzquierda(), maxDato, maxContador);
        if (nodo->getContador() > maxContador) {
            maxDato = nodo->getDato();
            maxContador = nodo->getContador();
        }
        obtenerMayor(nodo->getDerecha(), maxDato, maxContador);
    }

public:
    ArbolAVL() : raiz(nullptr) {}


    void insertarR(int n) {
        insertar(raiz, n);
    }

    // Búsqueda por franja horaria
    void buscarPorFranjaHoraria(int h1, int h2) {
        vector<int> resultados;
        buscarPorFranja(raiz, h1, h2, resultados);

        if (resultados.empty()) {
            cout << "No se encontraron accesos en esa franja horaria.\n";
        } else {
            cout << "Accesos encontrados:\n";
            for (int clave : resultados) {
                cout << "- " << decodificarHoraZona(clave) << endl;
            }
        }
    }


    int zonaConMasEntradas() {
        int maxDato = -1;
        int maxContador = 0;
        obtenerMayor(raiz, maxDato, maxContador);
        return maxDato;
    }


    void mostrarEstadisticas() {
        int maxDato = zonaConMasEntradas();
        if (maxDato == -1) {
            cout << "No hay accesos registrados.\n";
        } else {
            cout << "Zona/horario con más accesos: " << decodificarHoraZona(maxDato) << endl;
        }
    }

    string decodificarHoraZona(int codificado) {
        int horaCodigo = codificado / 10000;
        int hh = horaCodigo / 100;
        int mm = horaCodigo % 100;
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", hh, mm);
        return string(buffer);
    }
};
