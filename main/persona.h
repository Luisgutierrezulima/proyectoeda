#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

struct Persona {
    string nombre;
    string apellido;
    int dni;
    string rol;
    string zona;
    string horaEntrada;

    void setHora() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        ss << setfill('0') << setw(2) << ltm->tm_hour << ":" << setw(2) << ltm->tm_min;
        horaEntrada = ss.str();
    }
};

#endif
