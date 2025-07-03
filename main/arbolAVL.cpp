#include <iostream>
#include "Persona.h"
using namespace std;

class NodoAVL {
public:
    int dni;
    string zona;
    string hora;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;

    NodoAVL(int dni, const string& zona, const string& hora) {
        this->dni = dni;
        this->zona = zona;
        this->hora = hora;
        this->altura = 1;
        this->izq = nullptr;
        this->der = nullptr;
    }
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int balance(NodoAVL* nodo) {
        return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
    }

    void actualizarAltura(NodoAVL* nodo) {
        nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    }

    NodoAVL* rotarIzq(NodoAVL* y) {
        NodoAVL* x = y->der;
        NodoAVL* t2 = x->izq;

        x->izq = y;
        y->der = t2;

        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    NodoAVL* rotarDer(NodoAVL* x) {
        NodoAVL* y = x->izq;
        NodoAVL* t2 = y->der;

        y->der = x;
        x->izq = t2;

        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    NodoAVL* insertar(NodoAVL* nodo, Persona* p) {
        if (!nodo)
            return new NodoAVL(p->dni, p->zona, p->horaEntrada);

        if (p->dni == nodo->dni && p->zona == nodo->zona && p->horaEntrada == nodo->hora) {
            cout << "El ingreso con DNI " << p->dni << " ya está registrado en zona "
                 << p->zona << " a la hora " << p->horaEntrada << endl;
            return nodo;
        }

        if (p->dni < nodo->dni)
            nodo->izq = insertar(nodo->izq, p);
        else if (p->dni > nodo->dni)
            nodo->der = insertar(nodo->der, p);
        else
            return nodo;

        actualizarAltura(nodo);
        int fb = balance(nodo);

        if (fb > 1 && p->dni < nodo->izq->dni)
            return rotarDer(nodo);
        if (fb < -1 && p->dni > nodo->der->dni)
            return rotarIzq(nodo);
        if (fb > 1 && p->dni > nodo->izq->dni) {
            nodo->izq = rotarIzq(nodo->izq);
            return rotarDer(nodo);
        }
        if (fb < -1 && p->dni < nodo->der->dni) {
            nodo->der = rotarDer(nodo->der);
            return rotarIzq(nodo);
        }

        return nodo;
    }

    void mostrarInorden(NodoAVL* nodo) {
        if (!nodo) return;
        mostrarInorden(nodo->izq);
        cout << "DNI: " << nodo->dni << ", Zona: " << nodo->zona << ", Hora: " << nodo->hora << endl;
        mostrarInorden(nodo->der);
    }

    void mostrarEnFranja(NodoAVL* nodo, const string& hInicio, const string& hFin) {
        if (!nodo) return;
        mostrarEnFranja(nodo->izq, hInicio, hFin);
        if (nodo->hora >= hInicio && nodo->hora <= hFin)
            cout << "DNI: " << nodo->dni << ", Zona: " << nodo->zona << ", Hora: " << nodo->hora << endl;
        mostrarEnFranja(nodo->der, hInicio, hFin);
    }

    void contarZonas(NodoAVL* nodo, string zonas[], int conteo[], int n) {
        if (!nodo) return;
        contarZonas(nodo->izq, zonas, conteo, n);
        for (int i = 0; i < n; ++i) {
            if (nodo->zona == zonas[i]) {
                conteo[i]++;
                break;
            }
        }
        contarZonas(nodo->der, zonas, conteo, n);
    }

public:
    ArbolAVL() {
        raiz = nullptr;
    }

    void insertar(Persona* p) {
        if(p->horaEntrada == "") {
            cout << "Hora de entrada no puede estar vacía." << endl;
            return;
        }
        raiz = insertar(raiz,p);
    }

    void mostrarEstadisticas() {
        const int N = 5;
        string zonas[N] = {"Zona Médica", "Zona Preferencial", "Zona A", "Zona B", "Zona C"};
        int conteo[N] = {0};

        contarZonas(raiz, zonas, conteo, N);

        int maxIdx = 0;
        for (int i = 1; i < N; i++)
            if (conteo[i] > conteo[maxIdx]) maxIdx = i;

        if (conteo[maxIdx] > 0)
            cout << "Zona con más ingresos: " << zonas[maxIdx]
                 << " (" << conteo[maxIdx] << " personas"<< endl;
        else
            cout << "No hay ingresos registrados"<<endl;
    }

    void buscarPorFranjaHoraria(const string& hInicio) {
        string hFin = hInicio;
        if (hFin.length() == 5) {
            if (hFin[3] == '0') hFin[3] = '3';
            else if (hFin[3] == '3') hFin[3] = '5';
            else hFin[3] = '9';
        }

        cout << "--- Ingresos entre " << hInicio << " y " << hFin << " ---\n";
        mostrarEnFranja(raiz, hInicio, hFin);
    }

    void mostrarIngresosOrdenados() {
        cout << "--- Ingresos registrados (ordenados por DNI) ---" << endl;
        mostrarInorden(raiz);
    }
};
