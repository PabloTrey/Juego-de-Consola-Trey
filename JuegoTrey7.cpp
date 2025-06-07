#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


void jugar();
// Muestra estadísticas (todavia no esta hecho)
void mostrarEstadisticas();
// Muestra los créditos del grupo
void mostrarCreditos();
// Pregunta si realmente querés salir del juego
bool confirmarSalida();
// Roba una carta y asigna puntos de tiempo
int robarCarta(int &pt);
// Cuenta cuántas veces aparece un valor en el vector de dados
int contarValor(int vDado[], int tam, int valor);
// Analiza la jugada y aplica bugs/fixes y puntos de ronda(hay que revisarlo bien)
void analizarJugada(int vDado[], int tam, int &pt, int &bugs, int &puntosRonda);
// Limpia la pantalla (anda en Windows y Mac/Linux)
void limpiarPantalla();
// Pausa hasta que se presione ENTER
void pausarPantalla();

int main()
{
    srand(time(NULL)); // Pone la semilla aleatoria una sola vez
    int opcion;
    bool salir = false;

    while (!salir)
    {
        limpiarPantalla();
        cout << "=======================\n";
        cout << "        TREY           \n";
        cout << "=======================\n";
        cout << "1 - JUGAR\n";
        cout << "2 - ESTADISTICAS\n";
        cout << "3 - CREDITOS\n";
        cout << "0 - SALIR\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            jugar(); // Jugar una partida
            break;
        case 2:
            mostrarEstadisticas(); // Mostrar estadísticas
            break;
        case 3:
            mostrarCreditos(); // Mostrar créditos
            break;
        case 0:
            salir = confirmarSalida(); // Preguntar si quiere salir
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente.\n";
            pausarPantalla();
        }
    }

    return 0;
}

// === FUNCION PRINCIPAL DEL JUEGO ===
void jugar()
{
    // Variables principales del juego
    string nombreJug[2];
    int pt[2] = {0, 0};
    int bugs[2] = {0, 0};
    int objetivo[2] = {0, 0};
    int puntosTotales[2] = {0, 0};
    bool finalizado[2] = {false, false};

    // Registro de nombres
    cout << "Antes de comenzar deben registrar sus nombres:" << endl;
    cout << "Jugador 1: ";
    cin >> nombreJug[0];
    cout << "\nJugador 2: ";
    cin >> nombreJug[1];
    limpiarPantalla();

    // Robar carta y definir objetivo inicial
    for (int i = 0; i < 2; i++)
    {
        cout << nombreJug[i] << " elejí una carta para empezar: " << endl;
        pausarPantalla();
        objetivo[i] = robarCarta(pt[i]);
        cout << "Recibe " << pt[i] << " PT\n" << endl << endl;
    }

    pausarPantalla();
    limpiarPantalla();

    // Turnos de los jugadores
    int turno = 0;
    bool juegoTerminado = false;

    while (!juegoTerminado)
    {
        if (finalizado[turno]) {
            turno = (turno + 1) % 2;
            continue;
        }

        int puntosRonda = 0;
        bool seguirJugando = true;

        while (seguirJugando)
        {
            limpiarPantalla();
            // Muestra estado actual del jugador
            cout << "TURNO DE " << nombreJug[turno] << endl;
            cout << "Puntos acumulados: " << puntosTotales[turno] << " / Objetivo: " << objetivo[turno] << endl;
            cout << "Puntos de Tiempo: " << pt[turno] << " - BUGS: " << bugs[turno] << endl;
            cout << endl;

            // Si no tiene PT, termina su juego
            if (pt[turno] < 1)
            {
                cout << nombreJug[turno] << " no tiene puntos de tiempo suficientes. Fin del juego para este jugador." << endl;
                finalizado[turno] = true;
                break;
            }

            // Selección de dados
            int nDado;
            cout << "Con cuántos dados querés lanzar? (2 = 1PT / 3 = 2PT): ";
            cin >> nDado;

            while ((nDado != 2 && nDado != 3) || (nDado == 2 && pt[turno] < 1) || (nDado == 3 && pt[turno] < 2))
            {
                cout << "Opcion invalida o sin PT suficiente. Elejí otra cantidad: ";
                cin >> nDado;
            }

            pt[turno] -= (nDado == 2 ? 1 : 2);

            // Tirada de dados
            int vDado[3];
            for (int i = 0; i < nDado; i++)
            {
                vDado[i] = rand() % 6 + 1;
                cout << "Dado " << (i + 1) << ": " << vDado[i] << endl;
            }

            int puntosAntes = puntosRonda;
            analizarJugada(vDado, nDado, pt[turno], bugs[turno], puntosRonda); // Analiza resultado de los dados

            if (puntosRonda == 0 && contarValor(vDado, nDado, 1) > 0)
            {
                // Si hubo bug, se termina la ronda
                cout << "Puntos totales: " << puntosTotales[turno] << endl;
                cout << "PT: " << pt[turno] << " - BUGS: " << bugs[turno] << endl;
                pausarPantalla();
                break;
            }
            else
            {
                cout << "Puntos de ronda acumulados: " << puntosRonda << endl;
                cout << "PT: " << pt[turno] << " - BUGS: " << bugs[turno] << endl;

                char decision;
                do {
                    cout << "? Desea continuar lanzando? (S/N): ";
                    cin >> decision;
                    decision = toupper(decision);
                } while (decision != 'S' && decision != 'N');

                if (decision == 'N')
                {
                    puntosTotales[turno] += puntosRonda;
                    seguirJugando = false;
                }
            }

            // Verifica si alcanzó el objetivo
            if (puntosTotales[turno] >= objetivo[turno])
            {
                cout << nombreJug[turno] << " Ganaste. fin del juego para este jugador." << endl;
                finalizado[turno] = true;
                seguirJugando = false;
            }

            pausarPantalla();
        }

        // Si ambos terminaron, fin del juego
        if (finalizado[0] && finalizado[1]) {
            juegoTerminado = true;
        }

        turno = (turno + 1) % 2;
    }

    cout << "Fin del juego. ¡Gracias por jugar TREY!" << endl;
    pausarPantalla();
}

// Roba carta y define puntos de tiempo iniciales
int robarCarta(int &pt)
{
    int carta = rand() % (65 - 50 + 1) + 50;
    cout << "Tu carta robada es: " << carta << endl;

    if (carta <= 55) pt = 7;
    else if (carta <= 60) pt = 8;
    else pt = 9;

    return carta;
}

// Cuenta cuántas veces aparece cierto número en los dados
int contarValor(int vDado[], int tam, int valor)
{
    int contador = 0;
    for (int i = 0; i < tam; i++)
    {
        if (vDado[i] == valor)
        {
            contador++;
        }
    }
    return contador;
}

// Analiza la jugada y actualiza bugs, PT y puntos de ronda
void analizarJugada(int vDado[], int tam, int &pt, int &bugs, int &puntosRonda)
{
    int cantBugs = contarValor(vDado, tam, 1);
    int cantFixes = contarValor(vDado, tam, 3);

    if (cantBugs > 0)
    {
        puntosRonda = 0;

        if (cantBugs == 1) bugs += 1;
        else if (cantBugs == 2) { bugs += 2; pt -= 1; }
        else if (cantBugs == 3) { bugs += 3; pt -= 2; }

        cout << "¡Apareció al menos un BUG! Fin de la ronda.\n";
    }
    else
    {
        for (int i = 0; i < tam; i++)
        {
            puntosRonda += vDado[i];
        }
    }

    if (cantFixes > 0)
    {
        int eliminados = cantFixes;
        if (eliminados > bugs) eliminados = bugs;
        bugs -= eliminados;

        if (cantFixes == 2) pt += 1;
        else if (cantFixes == 3) pt += 2;

        cout << "FIXES: -" << eliminados << " BUGS";
        if (cantFixes == 2) cout << " +1 PT";
        if (cantFixes == 3) cout << " +2 PT";
        cout << endl;
    }
}

// Muestra mensaje de estadísticas
void mostrarEstadisticas()
{
    cout << "Estadísticas aún no implementadas." << endl;
    pausarPantalla();
}

// Muestra los créditos de los integrantes
void mostrarCreditos()
{
    cout << "------------INTREGRANTES-------------" << endl;
    cout << "Ponce Alejandro - Legajo nro: 32583" << endl;
    cout << "Troncoso Gabriela - Legano nro: 32662" << endl;
    cout << "Robledo Pablo - Legajo nro: 27909" << endl;
    pausarPantalla();
}

// Confirma si el jugador realmente quiere salir
bool confirmarSalida()
{
    char respuesta;
    do
    {
        cout << "Seguro que desea salir? (S/N): ";
        cin >> respuesta;
        respuesta = toupper(respuesta);
    } while (respuesta != 'S' && respuesta != 'N');

    return (respuesta == 'S');
}

// Limpia la pantalla segun el sistema operativo
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pausa hasta que se presione ENTER
void pausarPantalla() {
    cout << "Presione ENTER para continuar...";
    cin.ignore();
    cin.get();
}
/*Faltaría:
*Mostrar una pantalla de resultado final con:

*Puntaje total

*BUGS

*PT restantes

*Hitos (opcional)

*Ganador final*/