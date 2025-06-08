
#include <iostream>
#include <map>
using namespace std;

class Node {
private:
    int dato;
    int altura;
    int contador;
    Node* izquierda;
    Node* derecha;
public:
    Node(int n) {
        dato = n;
        altura = 1;
        contador = 1;
        izquierda = nullptr;
        derecha = nullptr;
    }
    int getDato() { return dato; }
    int getAltura() { return altura; }
    int getContador() { return contador; }
    Node* getIzquierda() { return izquierda; }
    Node* getDerecha() { return derecha; }

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

    void insertar(Node*&nodo, int n) {
        if (nodo == nullptr) {
            nodo = new Node(n);
            return;  // Agregamos return para evitar acceder a un nodo nulo
        }
        
        if (n == nodo->getDato()) {
            nodo->setContador(nodo->getContador() + 1);
            return;  // No necesitamos rebalancear si solo incrementamos el contador
        }
        
        if (n < nodo->getDato()) {
            Node* izq = nodo->getIzquierda();
            insertar(izq, n);
            nodo->setIzquierda(izq);
        } else {
            Node* der = nodo->getDerecha();
            insertar(der, n);
            nodo->setDerecha(der);
        }
 

        actualizarAltura(nodo);
        int facBalance = factorBalance(nodo);

        // Verificamos que los nodos no sean nulos antes de acceder a ellos
        if (facBalance < -1 && nodo->getDerecha() != nullptr && n > nodo->getDerecha()->getDato())
            nodo = rotacionIzquierda(nodo);
        if (facBalance > 1 && nodo->getIzquierda() != nullptr && n < nodo->getIzquierda()->getDato())
            nodo = rotacionDerecha(nodo);
        if (facBalance > 1 && nodo->getIzquierda() != nullptr && n > nodo->getIzquierda()->getDato()) {
            nodo->setIzquierda(rotacionIzquierda(nodo->getIzquierda()));
            nodo = rotacionDerecha(nodo);
        }
        if (facBalance < -1 && nodo->getDerecha() != nullptr && n < nodo->getDerecha()->getDato()) {
            nodo->setDerecha(rotacionDerecha(nodo->getDerecha()));
            nodo = rotacionIzquierda(nodo);
        }
    }

    void inorder(Node* nodo) {
        if (nodo != nullptr) {
            inorder(nodo->getIzquierda());
            cout << nodo->getDato() << " (x" << nodo->getContador() << ") ";
            inorder(nodo->getDerecha());
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
    ArbolAVL() {
        raiz = nullptr;
    }

    void insertarR(int n) {
        insertar(raiz, n);
    }

    void inorderR() {
        inorder(raiz);
        cout << endl;
    }

    bool buscarR(int n) {
        return buscar(raiz, n);
    }

    int zonaConMasEntradas() {
        int maxDato = -1;
        int maxContador = 0;
        obtenerMayor(raiz, maxDato, maxContador);
        return maxDato;
    }
};
