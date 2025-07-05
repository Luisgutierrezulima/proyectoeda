//VERIFIQUEN o cambien si lo ven necesario luises

#include <iostream>
#include <fstream>
#include "Persona.h"
#include "tablaHash.cpp"
#include "Heap.cpp"
#include "ArbolAVL.cpp"
using namespace std;

void cargarDesdeTxt(const string& nombreArchivo, TablaHash& tabla) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error en abrir el archivo: "<<endl;
        return;
    }

    string nombre, apellido, rol, zona, hora;
    int dni;
    int correctas = 0; 
    int total = 0;

    while (archivo >> nombre >> apellido >> dni >> rol >> zona) {
        string hora;
        archivo >> hora;
        // Si la hora es vacía o contiene comillas dobles, se considera como no ingresada
        if (hora == "\"\""  || hora.empty()) {
            hora = ""; // Si la hora es vacía, se asigna una cadena vacía
        }
        
        total++;
        if (tabla.buscar(dni) == nullptr) { // Validar que no exista el DNI
            Persona* p = new Persona{nombre, apellido, dni, rol, zona, hora};
            tabla.insertar(p);
            correctas++;
        } else {
            cout << "DNI repetido (" << dni << "), no insertado." << endl;
        }
    }

    cout << "Datos cargados: "<< correctas << "/" << total<< " registros"<<endl;
}

void insertarAlHeap(const string& nombreArchivo, TablaHash& tabla, MaxHeap& heap) {

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {   
        cout << "Error al abrir el archivo: " << endl;
        return;
    }
    
    int dni;
    int insertados = 0;
    int YaEstanEnCola = 0;
    int total = 0;
    while (archivo >> dni) {
        total++;
        Persona* p = tabla.buscar(dni);
        if (p) {
            if (!heap.estaEnCola(p->dni)) {
                heap.insertar(p);
                insertados++;
            } else {
                YaEstanEnCola++;
            }
        }
    }
    cout << "Insertados al heap: " << insertados << " de "<<total<<" personas." << endl;
    cout << "Ya estaban en la cola: " << YaEstanEnCola << endl;
}

void insertarDesdeHeapAlAVL(MaxHeap& heap, ArbolAVL& arbol) {
    while (!heap.estaVacio()) {
        Persona* p = heap.extraerMax(); 
        if (!p) {
            cout << "Cola vacia, no hay personas para ingresar al árbol." << endl;
            return;
        } else {
            p->setHora(); 
            arbol.insertar(p->dni); //Que se pueda mostrar las estadisticas en el AVL(opcion 6)
            cout << "DNI: " << p->dni << "  Hora: " << p->horaEntrada << "." << endl;
        }
    }
}

int main() {
    TablaHash tablaUsuarios(50000);
    MaxHeap heapPrioridades(50000);
    ArbolAVL registroAccesos;
    

    while (true) {
        cout << "=== MENU ACCESOS ===" <<endl 
             << "1. Registrar asistente" <<endl
             << "2. Agregar a cola"<<endl
             << "3. Procesar ingreso"<<endl
             << "4. Ver proximos en cola"<<endl
             << "5. Consultar por hora"<<endl
             << "6. Ver estadisticas"<<endl
             << "7. Cargar desde archivo"<<endl
             << "8. Iniciar Cola desde archivo"<<endl
             << "9. Salir"<<endl
             << "Opcion: ";

        int opc;
        if (!(cin >> opc)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada no valida."<<endl;
        continue;
        }
        cin.ignore();

        if (opc == 1) {
            auto* p = new Persona;
            cout << "Nombre: ";            cin >> p->nombre;
            cout << "Apellido: ";          cin >> p->apellido;
            cout << "DNI (Zona1, Zona2, Zona3, Zona4, Zona5): ";               cin >> p->dni;
            cout << "Rol (VIP,medico,seguridad,discapacitado): ";               cin >> p->rol;
            cout << "Zona: ";              cin >> p->zona;
            p->horaEntrada = ""; // Hora de ingreso se deja vacía inicialmente
            tablaUsuarios.insertar(p);
        }
        else if (opc == 2) {
            int id; 
            cout << "DNI: "; cin >> id;
            Persona* p = tablaUsuarios.buscar(id);
            if (!p) 
            cout << "No existe DNI"<<endl;
            else if (heapPrioridades.estaEnCola(p->dni)) {
                cout << "Ya esta en la cola de espera." << endl;
            } 
            else if (p->horaEntrada != "") {
                cout << "Ya ingreso a la zona." << endl;
            } 
            
            else    heapPrioridades.insertar(p);
        }
        else if (opc == 3) {
           /*
            Persona* p = heapPrioridades.extraerMax();
            if (!p) 
            cout << "Cola vacia" <<endl;
            else {
                cout << "Ingreso: " << p->nombre << " (" << p->rol << ")"<<endl;
                string h; 
                cout << "Hora ingreso (HH:MM): "; cin >> h;
                p->horaEntrada = h;
                registroAccesos.insertar(p);
            }
          */

           insertarDesdeHeapAlAVL(heapPrioridades, registroAccesos);
        }
        else if (opc == 4) {
            heapPrioridades.mostrarTop(5);
        }
        else if (opc == 5) {
            string h; 
            cout << "Hora (HH:MM): "; 
            cin >> h;
            registroAccesos.buscarPorFranjaHoraria(h);
        }
        else if (opc == 6) {
            registroAccesos.mostrarEstadisticas();
        }
        else if (opc == 7){
                    string archivo;
            cout << "--- Cargar datos desde TXT ---"<<endl;
            cout << "Nombre (con .txt): ";
            getline(cin, archivo);
            cargarDesdeTxt(archivo, tablaUsuarios);
        }
        else if (opc == 8) {
            string archivo;
            cout << "Insertando al heap..." << endl;
            cout << "Nombre del archivo (con .txt): ";
            getline(cin, archivo);
            insertarAlHeap(archivo, tablaUsuarios, heapPrioridades);
        }
        else if (opc == 9) break;
        else {
            cout << "Opcion no valida."<<endl;
        }

        
    }
    return 0;
}
    
