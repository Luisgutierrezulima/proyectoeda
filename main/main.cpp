#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
using namespace std;

#include "tablaHash.cpp"
#include "heap.cpp"
#include "arbolAVL.cpp"


int obtenerPrioridad(const string& rol) {
    if (rol == "VIP") return 5;        // Máxima prioridad
    if (rol == "medico") return 4;     // Alta prioridad
    if (rol == "seguridad") return 3;  // Prioridad media
    if (rol == "discapacitado") return 2; // Prioridad especial
    return 1; // Prioridad general (mínima)
}

int codificarHoraZona(const string& hora, const string& zona) {
    return stoi(hora.substr(0, 2) + hora.substr(3, 2)) + zona.length() * 10000;
}


string decodificarHoraZona(int codificado) {
    int horaCodigo = codificado / 10000;
    int hh = horaCodigo / 100;
    int mm = horaCodigo % 100;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hh, mm);
    return string(buffer);
}

void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void cargarDesdeArchivo(const string& nombreArchivo, TablaHash& tabla, MaxHeap& heap, ArbolAVL& avl) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    int correctas = 0, total = 0;

    // Procesar cada línea del archivo
    while (getline(archivo, linea)) {
        total++;
        stringstream ss(linea);
        vector<string> partes;
        string parte;

        // Dividir por comas
        while (getline(ss, parte, ',')) {
            partes.push_back(parte);
        }

        // Validar formato
        if (partes.size() != 4) continue;

        try {
            int id = stoi(partes[0]);
            string rol = partes[1];
            string zona = partes[2];
            string horaProgramada = partes[3];

            // Almacenar en todas las estructuras
            tabla.insertar(id, rol, zona, horaProgramada);
            heap.insertar(id, obtenerPrioridad(rol));
            correctas++;
        } catch (...) {
            continue;
        }
    }
    cout << "Datos cargados: " << correctas << "/" << total << " registros\n";
}

int main() {

    TablaHash tablaUsuarios(100);
    MaxHeap heapPrioridades(10000);
    ArbolAVL registroAccesos;

    while (true) {
        // Menú principal
        cout << "\n=== SISTEMA DE CONTROL DE ACCESOS ===" << endl;
        cout << "1. Registrar nuevo asistente" << endl;
        cout << "2. Agregar a cola de acceso" << endl;
        cout << "3. Procesar ingreso" << endl;
        cout << "4. Mostrar siguientes en cola" << endl;
        cout << "5. Consultar accesos por hora" << endl;
        cout << "6. Actualizar prioridad" << endl;
        cout << "7. Estadísticas de acceso" << endl;
        cout << "8. Cargar datos desde archivo" << endl;
        cout << "9. Salir" << endl;
        cout << "Seleccione opción: ";

        int opcion;
        if (!(cin >> opcion)) {
            limpiarEntrada();
            continue;
        }
        limpiarEntrada();


        if (opcion == 1) {
            int id;
            string rol, zona, horaProgramada;

            cout << "\n--- Registro de nuevo asistente ---" << endl;
            cout << "DNI/QR (puede ser negativo): ";
            cin >> id;
            cout << "Rol (VIP/medico/seguridad/discapacitado/general): ";
            cin >> rol;
            cout << "Zona asignada: ";
            cin >> zona;
            cout << "Hora de entrada programada (HH:MM): ";
            cin >> horaProgramada;


            tablaUsuarios.insertar(id, rol, zona, horaProgramada);
            cout << "Asistente registrado exitosamente.\n";
        }

        else if (opcion == 2) {
            int id;
            cout << "\n--- Agregar a cola de acceso ---" << endl;
            cout << "DNI/QR a agregar: ";
            cin >> id;

            // Buscar persona en tabla hash
            Persona* p = tablaUsuarios.buscar(id);
            if (!p) {
                cout << "Error: ID no registrado.\n";
                continue;
            }


            heapPrioridades.insertar(id, obtenerPrioridad(p->rol));
            cout << "Agregado a cola como " << p->rol << " (Prioridad: "
                 << obtenerPrioridad(p->rol) << ")\n";
        }

        else if (opcion == 3) {
            cout << "\n--- Procesar ingreso ---" << endl;


            int id = heapPrioridades.extraerMax();
            if (id == -1) {
                cout << "No hay personas en cola.\n";
                continue;
            }


            Persona* p = tablaUsuarios.buscar(id);
            if (!p) {
                cout << "Error: ID no encontrado en registros.\n";
                continue;
            }

            cout << "ID: " << id << " | Rol: " << p->rol << endl;
            cout << "Zona asignada: " << p->zona << endl;
            cout << "Hora programada: " << p->horaEntrada << endl;


            string horaAcceso;
            cout << "Ingrese hora de acceso (HH:MM): ";
            cin >> horaAcceso;


            if (horaAcceso < p->horaEntrada) {
                cout << "¡ADVERTENCIA! Intento de acceso ANTES de la hora programada.\n";
                cout << "El asistente se mantendrá en la cola.\n";
                heapPrioridades.insertar(id, obtenerPrioridad(p->rol));
                continue;
            }


    registroAccesos.insertarR(stoi(horaAcceso.substr(0, 2) + horaAcceso.substr(3, 2)) + p->zona.length() * 10000);
    cout << "Acceso registrado correctamente.\n";
}

        else if (opcion == 4) {
            cout << "\n--- Próximos en cola ---" << endl;
            heapPrioridades.mostrarTop(5);
        }


        else if (opcion == 5) {
            string hora;
            cout << "\n--- Consulta de accesos ---" << endl;
            cout << "Ingrese hora a consultar (HH:MM): ";
            cin >> hora;


            int codigoHora = stoi(hora.substr(0, 2)) * 100 + stoi(hora.substr(3, 2));
            cout << "Accesos registrados a las " << hora << ":\n";


            registroAccesos.buscarPorFranjaHoraria(codigoHora * 10000, (codigoHora + 1) * 10000);
        }

        else if (opcion == 6) {
            int id;
            string nuevoRol;
            cout << "\n--- Actualizar prioridad ---" << endl;
            cout << "ID a actualizar: ";
            cin >> id;
            cout << "Nuevo rol: ";
            cin >> nuevoRol;

            Persona* p = tablaUsuarios.buscar(id);
            if (p) {

                p->rol = nuevoRol;


                heapPrioridades.actualizarPrioridad(id, obtenerPrioridad(nuevoRol));
                cout << "Prioridad actualizada a " << obtenerPrioridad(nuevoRol) << endl;
            } else {
                cout << "ID no encontrado.\n";
            }
        }

        else if (opcion == 7) {
            cout << "\n--- Estadísticas de acceso ---" << endl;
            registroAccesos.mostrarEstadisticas();
        }

        else if (opcion == 8) {
            string archivo;
            cout << "\n--- Cargar datos ---" << endl;
            cout << "Nombre del archivo: ";
            cin >> archivo;
            cargarDesdeArchivo(archivo, tablaUsuarios, heapPrioridades, registroAccesos);
        }

        else if (opcion == 9) {
            cout << "Saliendo del sistema...\n";
            break;
        }
        else {
            cout << "Opción no válida.\n";
        }
    }

    return 0;
}
