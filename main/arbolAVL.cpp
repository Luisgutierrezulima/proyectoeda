#include <iostream>
#include "Persona.h"
using namespace std;

struct NodoAVL {
    string clave;
    int contador;
    int altura;
    NodoAVL *izq, *der;

    NodoAVL(const string& c) {
        clave    = c;
        contador = 1;
        altura   = 1;
        izq = der = nullptr;
    }
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* n) {
        return n ? n->altura : 0;
    }

    void actualizar(NodoAVL* n) {
        n->altura = 1 + max(altura(n->izq), altura(n->der));
    }

    int balance(NodoAVL* n) {
        return n ? altura(n->izq) - altura(n->der) : 0;
    }

    NodoAVL* rotIzq(NodoAVL* y) {
        NodoAVL* x = y->der;
        NodoAVL* t = x->izq;
        x->izq = y; y->der = t;
        actualizar(y); actualizar(x);
        return x;
    }

    NodoAVL* rotDer(NodoAVL* x) {
        NodoAVL* y = x->izq;
        NodoAVL* t = y->der;
        y->der = x; x->izq = t;
        actualizar(x); actualizar(y);
        return y;
    }

    NodoAVL* insertarRec(NodoAVL* nodo, const string& clave) {
        if (!nodo) return new NodoAVL(clave);
        if (clave == nodo->clave) {
            nodo->contador++;
            return nodo;
        }
        if (clave < nodo->clave)
            nodo->izq = insertarRec(nodo->izq, clave);
        else
            nodo->der = insertarRec(nodo->der, clave);

        actualizar(nodo);
        int b = balance(nodo);

        if (b > 1 && clave < nodo->izq->clave)        return rotDer(nodo);
        if (b < -1 && clave > nodo->der->clave)      return rotIzq(nodo);
        if (b > 1 && clave > nodo->izq->clave) {
            nodo->izq = rotIzq(nodo->izq);
            return rotDer(nodo);
        }
        if (b < -1 && clave < nodo->der->clave) {
            nodo->der = rotDer(nodo->der);
            return rotIzq(nodo);
        }
        return nodo;
    }

    void inorder(NodoAVL* nodo) {
        if (!nodo) return;
        inorder(nodo->izq);
        cout << nodo->clave << ": " << nodo->contador << "\n";
        inorder(nodo->der);
    }

public:
    ArbolAVL() { raiz = nullptr; }

    void insertar(const string& zona, const string& hora) {
        string clave = zona + "_" + hora;
        raiz = insertarRec(raiz, clave);
    }
    
    //Falta hacer los detalles de esto, tipo si una zona es mayor que otra y eso
    void mostrarEstadisticas() {
        inorder(raiz);
    }

    void buscarPorFranjaHoraria(const string& prefijo) {
        //Falta hacer esto reyes
    }

    //Falta hacer lo de buscar por zona y hora
    
    
};
