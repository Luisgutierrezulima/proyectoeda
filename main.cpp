
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

struct Acceso {
    int id;
    string rol;
    string zona;
    string hora;
};

int obtenerPrioridad(const string& rol) {
    if (rol == "VIP") return 5;
    if (rol == "medico") return 4;
    if (rol == "seguridad") return 3;
    if (rol == "discapacitado") return 2;
    return 1;
}

int codificarHoraZona(const string& hora, const string& zona) {
    return stoi(hora.substr(0, 2) + hora.substr(3, 2)) + zona.length() * 10000;
}

string decodificarHoraZona(int codificado) {
    int hora = codificado % 10000;
    int zonaLen = codificado / 10000;
    int hh = hora / 100;
    int mm = hora % 100;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hh, mm);
    return string(buffer) + " (zona: " + to_string(zonaLen) + " letras)";
}

void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void cargarDesdeArchivo(const string& nombreArchivo, TablaHash& tabla, MaxHeap& heap, ArbolAVL& avl) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    int numLineas = 0, cargadas = 0;
    while (getline(archivo, linea)) {
        numLineas++;
        if (linea.empty()) continue;

        stringstream ss(linea);
        string parte;
        vector<string> partes;

        while (getline(ss, parte, ',')) {
            partes.push_back(parte);
        }

        if (partes.size() != 4) {
            cout << "Línea ignorada (formato incorrecto): " << linea << endl;
            continue;
        }

        try {
            int id = stoi(partes[0]);
            string rol = partes[1];
            string zona = partes[2];
            string hora = partes[3];

            tabla.insertar(id);
            int prioridad = obtenerPrioridad(rol);
            heap.insertar(id, prioridad);

            int clave = codificarHoraZona(hora, zona);
            avl.insertarR(clave);
            cargadas++;
        } catch (...) {
            cout << "Línea ignorada (error de conversión): " << linea << endl;
            continue;
        }
    }

    archivo.close();
    cout << "Cargadas " << cargadas << " de " << numLineas << " líneas." << endl;
}

int main() {
    TablaHash tablaUsuarios(100);
    MaxHeap heapPrioridades(10000);
    ArbolAVL registroAccesos;

    while (true) {
        cout << "\n0. Cargar desde archivo\n1. Registrar usuario\n2. Agregar a cola de acceso\n3. Procesar ingreso\n4. Mostrar siguientes 5\n5. Consultar zona/horario\n6. Actualizar prioridad en cola\n7. Mostrar zona/horario con más accesos\n8. Salir\nOpcion: ";
        int opcion;
        if (!(cin >> opcion)) {
            cout << "Entrada inválida. Intente de nuevo.\n";
            limpiarEntrada();
            continue;
        }
        //soy un luis
        //amistad mas carajo
        //amistad mas especial es luis
        limpiarEntrada(); // Limpiar antes de cada caso

        if (opcion == 0) {
            string nombre;
            cout << "Nombre del archivo (ej. datos_simulados.txt): ";
            getline(cin, nombre);
            cargarDesdeArchivo(nombre, tablaUsuarios, heapPrioridades, registroAccesos);
        }
        else if (opcion == 1) {
            int id;
            cout << "Ingrese DNI/codigo QR (puede ser negativo): ";
            cin >> id;
            tablaUsuarios.insertar(id);
            cout << "Registrado.\n";
        }
        else if (opcion == 2) {
            Acceso acceso;
            cout << "ID: "; cin >> acceso.id;
            if (!tablaUsuarios.buscar(acceso.id)) {
                cout << "ID no registrado.\n";
                continue;
            }
            cout << "Rol (VIP/medico/seguridad/discapacitado/general): "; cin >> acceso.rol;
            cout << "Zona: "; cin >> acceso.zona;
            cout << "Hora (HH:MM): "; cin >> acceso.hora;

            int prioridad = obtenerPrioridad(acceso.rol);
            heapPrioridades.insertar(acceso.id, prioridad);

            cout << "Agregado a la cola de ingreso.\n";
        }
        else if (opcion == 3) {
            int siguienteID = heapPrioridades.extraerMax();
            if (siguienteID == -1) {
                cout << "No hay personas en espera.\n";
                continue;
            }
            cout << "Ingresando ID: " << siguienteID << endl;

            string zona, hora;
            cout << "Confirmar zona: "; cin >> zona;
            cout << "Hora de ingreso (HH:MM): "; cin >> hora;
            int codificado = codificarHoraZona(hora, zona);
            registroAccesos.insertarR(codificado);

            cout << "Ingreso registrado.\n";
        }
        else if (opcion == 4) {
            cout << "Siguientes 5 personas:\n";
            heapPrioridades.mostrarTop(5);
        }
        else if (opcion == 5) {
            int h1, h2;
            cout << "Consulta por franja horaria.\nHora inicio (HHMM): "; cin >> h1;
            cout << "Hora fin (HHMM): "; cin >> h2;
            cout << "Resultados:\n";
            for (int zonaLen = 1; zonaLen <= 20; zonaLen++) {
                for (int hora = h1; hora <= h2; hora++) {
                    int clave = zonaLen * 10000 + hora;
                    if (registroAccesos.buscarR(clave))
                        cout << "- Zona " << zonaLen << ", hora " << hora << " encontrada.\n";
                }
            }
        }
        else if (opcion == 6) {
            int id;
            string nuevoRol;
            cout << "ID a actualizar: "; cin >> id;
            cout << "Nuevo rol: "; cin >> nuevoRol;
            int nuevaPrioridad = obtenerPrioridad(nuevoRol);
            heapPrioridades.actualizarPrioridad(id, nuevaPrioridad);
        }
        else if (opcion == 7) {
            int clave = registroAccesos.zonaConMasEntradas();
            if (clave == -1) cout << "No hay accesos registrados.\n";
            else cout << "Zona/horario con más accesos: " << decodificarHoraZona(clave) << endl;
        }
        else if (opcion == 8) break;
        else cout << "Opción inválida.\n";
    }

    return 0;
}
