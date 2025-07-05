#include <iostream>
#include "Persona.h"
#include "Heap.cpp"
using namespace std;

class NodoAVL {
    private:
        int dni;
        string zona;
        string hora;
        int altura;
        NodoAVL* izq;
        NodoAVL* der;

    public:
        NodoAVL(int dni, const string& zona, const string& hora) {
            this->dni = dni;
            this->zona = zona;
            this->hora = hora;
            this->altura = 1;
            this->izq = nullptr;
            this->der = nullptr;
        }

        int getDni() { return dni; }
        int getAltura() { return altura; }
        string getZona() { return zona; }
        string getHora() { return hora; }
        NodoAVL* getIzquierda() { return izq; }
        NodoAVL* getDerecha() { return der; }
        void setDato(int valor) { dni = valor; }
        void setAltura(int valor) { altura = valor; }
        void setZona(const string& zona) { this->zona = zona; }
        void setHora(const string& hora) { this->hora = hora; }
        void setIzquierda(NodoAVL* nodo) { izq= nodo; }
        void setDerecha(NodoAVL* nodo) { der = nodo; }
};

class ArbolAVL {
    private:
        NodoAVL* raiz;

        int altura(NodoAVL* nodo) {
            return nodo ? nodo->getAltura() : 0;
        }

        int balance(NodoAVL* nodo) {
            return nodo ? altura(nodo->getIzquierda()) - altura(nodo->getDerecha()) : 0;
        }

        void actualizarAltura(NodoAVL* nodo) {
            nodo->setAltura(1 + max(altura(nodo->getIzquierda()), altura(nodo->getDerecha())));
        }

        //x = Nueva raiz
        //y = actual raiz
        //t2 = subArbol
        NodoAVL* rotarIzq(NodoAVL* y) {
            NodoAVL* x = y->getDerecha();
            NodoAVL* t2 = x->getIzquierda();

            x->setIzquierda(y);
            y->setDerecha(t2);

            actualizarAltura(y);
            actualizarAltura(x);
            return x;
        }

        //x = actual raiz
        //y = Nueva raiz
        //t2 = subArbol
        NodoAVL* rotarDer(NodoAVL* x) {
            NodoAVL* y = x->getIzquierda();
            NodoAVL* t2 = y->getDerecha();

            y->setDerecha(x);
            x->setIzquierda(t2);

            actualizarAltura(x);
            actualizarAltura(y);
            return y;
        }
        
        NodoAVL* insertar(NodoAVL* nodo, Persona* p) {
            if (!nodo)
                return new NodoAVL(p->dni, p->zona, p->horaEntrada);

            if (p->dni == nodo->getDni() && p->zona == nodo->getZona() && p->horaEntrada == nodo->getHora()) {
                cout << "El ingreso con DNI " << p->dni << " ya está registrado en zona "
                    << p->zona << " a la hora " << p->horaEntrada << endl;
                return nodo;
            }

            if (p->dni < nodo->getDni())
                nodo->setIzquierda(insertar(nodo->getIzquierda(), p));
            else if (p->dni > nodo->getDni())
                nodo->setDerecha(insertar(nodo->getDerecha(), p));
            else
                return nodo;

            actualizarAltura(nodo);
            int fb = balance(nodo);

            if (fb > 1 && p->dni < nodo->getIzquierda()->getDni())
                return rotarDer(nodo);
            if (fb < -1 && p->dni > nodo->getDerecha()->getDni())
                return rotarIzq(nodo);
            if (fb > 1 && p->dni > nodo->getIzquierda()->getDni()) {
                nodo->setIzquierda(rotarIzq(nodo->getIzquierda()));
                return rotarDer(nodo);
            }
            if (fb < -1 && p->dni < nodo->getDerecha()->getDni()) {
                nodo->setDerecha(rotarDer(nodo->getDerecha()));
                return rotarIzq(nodo);
            }

            return nodo;
        }
    
        
        NodoAVL* insertar(NodoAVL* nodo, int dni, const string& zona, const string& hora) {
            if (!nodo)
                return new NodoAVL(dni, zona, hora);

            if (dni < nodo->getDni())
                nodo->setIzquierda(insertar(nodo->getIzquierda(), dni, zona, hora));
            else if (dni > nodo->getDni())
                nodo->setDerecha(insertar(nodo->getDerecha(), dni, zona, hora));
            else
                return nodo; // No se permiten repetidos

            actualizarAltura(nodo);
            int fb = balance(nodo);

            if (fb > 1 && dni < nodo->getIzquierda()->getDni())
                return rotarDer(nodo);
            if (fb < -1 && dni > nodo->getDerecha()->getDni())
                return rotarIzq(nodo);
            if (fb > 1 && dni > nodo->getIzquierda()->getDni()) {
                nodo->setIzquierda(rotarIzq(nodo->getIzquierda()));
                return rotarDer(nodo);
            }
            if (fb < -1 && dni < nodo->getDerecha()->getDni()) {
                nodo->setDerecha(rotarDer(nodo->getDerecha()));
                return rotarIzq(nodo);
            }

            return nodo;
        }


        void mostrarInorden(NodoAVL* nodo) {
            if (!nodo) return;
            mostrarInorden(nodo->getIzquierda());
            cout << "DNI: " << nodo->getDni() << ", Zona: " << nodo->getZona() << ", Hora: " << nodo->getHora() << endl;
            mostrarInorden(nodo->getDerecha());
        }

        void mostrarEnFranja(NodoAVL* nodo, const string& hInicio, const string& hFin) {
            if (!nodo) return;
            mostrarEnFranja(nodo->getIzquierda(), hInicio, hFin);
            if (nodo->getHora() >= hInicio && nodo->getHora() <= hFin)
                cout << "DNI: " << nodo->getDni() << ", Zona: " << nodo->getZona() << ", Hora: " << nodo->getHora() << endl;
            mostrarEnFranja(nodo->getDerecha(), hInicio, hFin);
        }

        void contarZonas(NodoAVL* nodo, string zonas[], int conteo[], int n) {
            if (!nodo) return;
            contarZonas(nodo->getIzquierda(), zonas, conteo, n);
            for (int i = 0; i < n; ++i) {
                if (nodo->getZona() == zonas[i]) {
                    conteo[i]++;
                    break;
                }
            }
            contarZonas(nodo->getDerecha(), zonas, conteo, n);
        }

    public:
        ArbolAVL() {
            raiz = nullptr;
        }
      
        void insertar(int dni) {
            // Puedes dejar zona y hora vacías o con valores por defecto
            raiz = insertar(raiz, dni, "", "");
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
            string zonas[N] = {"Zona1", "Zona2", "Zona3", "Zona4", "Zona5"};
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
