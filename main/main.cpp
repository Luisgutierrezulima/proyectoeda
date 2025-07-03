//VERIFIQUEN o cambien si lo ven necesario luises

#include <iostream>
#include <fstream>
#include "Persona.h"
#include "tablaHash.cpp"
#include "Heap.cpp"
#include "ArbolAVL.cpp"
using namespace std;

void cargarDesdeTxt(const string& nombreArchivo, TablaHash& tabla, MaxHeap& heap, ArbolAVL& avl) {
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
        if (hora == "\"\""  || hora.empty()) // Si la hora es vacía o contiene comillas dobles, se considera como no ingresada{
            hora = ""; // Si la hora es vacía, se asigna una cadena vacía
        
        total++;
        Persona* p = new Persona{nombre, apellido, dni, rol, zona, hora};
        tabla.insertar(p);
        correctas++;
    }

    cout << "Datos cargados: "<< correctas << "/" << total<< " registros"<<endl;
}

void insertarTodosRegistradosAlHeap(TablaHash& tabla, MaxHeap& heap, int tam) {
    int insertados = 0;
    for (int i = 0; i < tam; ++i) {
        Persona* p = tabla.obtenerPos(i);
        if (p != nullptr && p->horaEntrada == "" && !heap.estaEnCola(p->dni)) {
            heap.insertar(p);
            insertados++;
        }
    }
    cout << "Insertados al heap: " << insertados << " personas." << endl;
}


int main() {
    TablaHash tablaUsuarios(1000);
    MaxHeap heapPrioridades(500);
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
             << "8. Salir"<<endl
             << "9. Insertar todos al heap"<<endl
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
            cout << "DNI: ";               cin >> p->dni;
            cout << "Rol (VIP,medico,seguridad,discapacitado): ";               cin >> p->rol;
            //pongale zonas para que "seleccione como en el ROL"
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
            cargarDesdeTxt(archivo, tablaUsuarios, heapPrioridades, registroAccesos);
        }
        else if (opc == 8) break;
        else if (opc == 9) {
            cout << "Insertando todos al heap..." << endl;
            insertarTodosRegistradosAlHeap(tablaUsuarios, heapPrioridades, 1000);
        }
        else {
            cout << "Opcion no valida."<<endl;
        }

    }
    return 0;
}
    
