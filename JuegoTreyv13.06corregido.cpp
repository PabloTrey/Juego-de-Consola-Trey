/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string> // para usar un vector que guarde juegos
#include <vector>
#include <cctype> //Para isdigit


using namespace std;

//DECLARACION DE VECTORES GLOBALES PARA GUARDAR LOS DATOS DEL RANKING
const int MaxJugadores=100;
string rankingNombres[MaxJugadores];
int rankingPuntos [MaxJugadores];
int totalJugadores=0;

// DIBUJA LOS DADOS
void dibujarDados(int vDado[], int cantidad);

//FUNCION PRINCIPAL DEL JUEGO
void jugar();

// ORDENA EL RANKING DE JUGADORES CON MAYOR PUNTAJE
void ordenarRanking(string nombreJug [], int puntosTotales[], int cantidad);

// MUESTRA ESTADISTICAS (EN DESARROLLO)
void mostrarEstadisticas(string nombreJug[5],int puntosTotales[5], int cantidad);

// MUESTRA LOS CREDITOS DEL GRUPO
void mostrarCreditos();

// PREGUNTA SI REALMENTE QUIERE SALIR DEL JUEGO
bool confirmarSalida();

// ROBA UNA CARTA Y ASIGNA PUNTOS DE TIEMPO
int robarCarta(int &pt);

// CUENTA CUANTAS VECES APARECE UN VALOR EN EL VECTOR DE DADOS
int contarValor(int vDado[], int tam, int valor);

// ANALIZA LA JUGADA Y APLICA BUGS/FIXES Y PUNTOS DE RONDA (HAY QUE REVISARLO BIEN)
void analizarJugada(int vDado[], int tam,int &pt, int &bugs, int &puntosRonda);

// LIMPIA LA PANTALLA (ANDA EN WINDOWS Y MAC/LINUX)
void limpiarPantalla();

// PEDIR NUMERO 2 O 3 PARA JUGAR
int PedirNumeroValido();

// MOSTRAR RESULTADOS FINALES
void mostrarResultadoFinales (string nombreJug[2], int puntosTotales[2],int pt[2], int bugs[2], bool finalizado[2]);

// PAUSA HASTA QUE SE PRESIONE ENTER
void pausarPantalla();

// MUESTRA PANTALLA EN JUEGO Y ESTADISTICAS DE LOS JUGADORES
void mostrarPanelRonda(string nombreJug[2], int puntosTotales[2], int pt[2], int bugs[2], int turno, int ronda, int puntosRonda, int lanzamientos);


int main()
{
    srand(time(NULL));
// PONER LA SEMILLA ALEATORIA UNA SOLA VEZ
    int opcion;
    bool salir = false;

    while (!salir)
    {
        limpiarPantalla();
        cout << endl<<endl<<endl<<endl<<endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "                                           =======================" << endl;
        cout << "                                                     TREY           " << endl;
        cout << "                                           =======================" << endl;
        cout << "                                           1 - JUGAR" << endl;
        cout << "                                           2 - ESTADISTICAS" << endl;
        cout << "                                           3 - CREDITOS" << endl;
        cout << "                                           0 - SALIR" << endl;
        cout << "                                           Seleccione una opcion: ";



        cin >> opcion;
        limpiarPantalla();

        switch (opcion)
        {
        case 1:
            jugar();                  // JUGAR UNA PARTIDA
            break;
        case 2:
            ordenarRanking (rankingNombres, rankingPuntos, totalJugadores);
            mostrarEstadisticas(rankingNombres,rankingPuntos,totalJugadores);   // MOSTRAR ESTADISTICAS
            pausarPantalla();
            break;
        case 3:
            mostrarCreditos();        // MOSTRAR CREDITOS
            break;
        case 0:
            salir = confirmarSalida();// Preguntar si quiere salir
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente." << endl;
            pausarPantalla();
        }
    }

    return 0;
}

// === FUNCION PRINCIPAL DEL JUEGO ===
void jugar()
{
    // VARIABLES PRINCIPALES DEL JUEGO
    string nombreJug[2];
    int pt[2] = {0, 0};
    int bugs[2] = {0, 0};
    int objetivo[2] = {0, 0};
    int puntosTotales[2] = {0, 0};
    int rondaActual =1;
    bool finalizado[2] = {false, false};

    // REGISTRO DE NOMBRES
    cout <<endl;
    cout << "                              Antes de comenzar deben registrar sus nombres:" << endl;
    cout << "                              Jugador 1: ";
    cin.ignore();//limpia buffer para que funcione getline
    getline(cin,nombreJug[0]);//para poder ingresar por ejemplo "Juan Pablo"
    cout << endl << "                              Jugador 2: ";
    getline(cin,nombreJug[1]);
    limpiarPantalla();

    // ROBAR CARTA Y DEFINIR OBJETIVO INICIAL
    for (int i = 0; i < 2; i++)
    {
        cout <<endl<<endl<<endl<<endl<<endl<<endl;
        cout << "                                     "  <<nombreJug[i] <<" elegí una carta para empezar: " << endl;
        pausarPantalla();
        objetivo[i] = robarCarta(pt[i]);
        cout << "                                                  Recibe " << pt[i] << " PT" << endl << endl;
    }

    pausarPantalla();
    limpiarPantalla();

    // TURNOS DE LOS JUGADORES
    int turno = 0;
    bool juegoTerminado = false;

    while (!juegoTerminado)
    {
        if (finalizado[turno])
        {
            turno = (turno + 1) % 2;
            continue;
        }

        int lanzamientoActual=0;
        int puntosRonda = 0;
        bool seguirJugando = true;

       // int lanzamientos = 0;
        while (seguirJugando)
        {

            lanzamientoActual ++; // Incrementa cada vez que se lanza
            limpiarPantalla();
            mostrarPanelRonda(nombreJug, puntosTotales, pt, bugs, turno, rondaActual, puntosRonda, lanzamientoActual);

            // SI NO TIENE PT, TERMINA SU JUEGO
            if (pt[turno] < 1)
            {
                cout << nombreJug[turno] << " no tiene puntos de tiempo suficientes. Fin del juego para este jugador." << endl;
                finalizado[turno] = true;
                break;
            }

            // SELECCION DE DADOS

            int nDado;
            // VERIFICAR SI SOLO PUEDE TIRAR 2 DADOS
            if (pt[turno] == 1)
            {
                cout << "Solo tienes 1 PT. Solo puedes tirar 2 dados." << endl;
                nDado = 2;
            }
            else
            {
                cout << "Con cuantos dados queres lanzar? (2 = 1PT / 3 = 2PT): ";
                cin >> nDado;
                while ((nDado != 2 && nDado != 3) || (nDado == 2 && pt[turno] < 1) || (nDado == 3 && pt[turno] < 2))
                {
                    cout << "Opcion invalida o sin PT suficiente. Elegí otra cantidad: ";
                    cin >> nDado;
                }
            }

            pt[turno] -= (nDado == 2 ? 1 : 2);

            // TIRADA DE DADOS
            int vDado[3];
            for (int i = 0; i < nDado; i++)
            {
                vDado[i] = rand() % 6 + 1;
                cout << "Dado " << (i + 1) << ": " << vDado[i] << endl;
            }
            dibujarDados(vDado, nDado);
            //int puntosAntes = puntosRonda;
            analizarJugada(vDado, nDado, pt[turno], bugs[turno], puntosRonda); // Analiza resultado de los dados

            if (puntosRonda == 0 && contarValor(vDado, nDado, 1) > 0)
            {
                // SIHUBO BUG, SE TERMINA LA RONDA
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
                do
                {
                    cout << "¿Desea continuar lanzando? (S/N): ";
                    cin >> decision;
                    decision = toupper(decision);
                }
                while (decision != 'S' && decision != 'N');

                puntosTotales[turno] += puntosRonda;

                if (decision == 'N')
                {
                    puntosTotales[turno] += puntosRonda;
                    seguirJugando = false;
                }
                puntosTotales[turno] = puntosRonda;
            }

            // VERIFICA SI ALCANZARON EL OBJETIVO
            if (puntosTotales[turno] >= objetivo[turno])
            {
                cout << nombreJug[turno] << " Ganaste. fin del juego para este jugador." << endl;
                finalizado[turno] = true;
                seguirJugando = false;
            }

            pausarPantalla();
        }

        // SI AMBOS TERMINARON, FIN DEL JUEGO
        if (finalizado[0] && finalizado[1])
        {
            juegoTerminado = true;
        }
        else if (rondaActual>=3&&!(finalizado[0]||finalizado [1]))
        {
            cout<< "Se alcanzo la ronda 10 sin ganadores. FIn del juego por limite de rondas."<<endl;
            finalizado[0]=finalizado [1]=true;
            juegoTerminado=true;
        }
        turno = (turno + 1) % 2;
        if(turno ==0)rondaActual++;
    }

    cout << "Fin del juego. ┬íGracias por jugar TREY!" << endl;

    //GUARDAR RESULTADOS EN EL RANKING

    for(int i=0; i<2; i++)
    {
        if(totalJugadores<MaxJugadores)
        {
            rankingNombres[totalJugadores]=nombreJug[i];
            rankingPuntos[totalJugadores]=puntosTotales[i];
            totalJugadores++;
        }
    }

    pausarPantalla();



}

// ROBA CARTA Y DEFINE PUNTOS DE TIEMPO INICIALES
int robarCarta(int &pt)
{
    int carta = rand() % (65 - 50 + 1) + 50;
    cout << "                                             Tu carta robada es: " << carta << endl;

    if (carta <= 55)
    {
    pt = 7;
    }
    else if (carta <= 60)
    {
    pt = 8;
    }
    else pt = 9;

    return carta;
}


// CUENTA CUANTAS VECES APARECE CIERTO NUMERO EN LOS DADOS
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

// ANALIZA LA JUGADA Y ACTUALIZA BUGS, PT Y PUNTOS DE RONDA
void analizarJugada(int vDado[], int tam, int &pt, int &bugs, int &puntosRonda)
{
    int cantBugs = contarValor(vDado, tam, 1);
    int cantFixes = contarValor(vDado, tam, 3);

    if (cantBugs > 0)
    {
        puntosRonda = 0;

        if (cantBugs == 1) bugs += 1;
        else if (cantBugs == 2)
        {
            bugs += 2;
            pt -= 1;
        }
        else if (cantBugs == 3)
        {
            bugs += 3;
            pt -= 2;
        }

        cout << "Aparecio al menos un BUG! Fin de la ronda." << endl;
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

        if (cantFixes == 2) pt += 1; //chequear esto
        else if (cantFixes == 3) pt += 2;

        cout << "FIXES: -" << eliminados << " BUGS";
        if (cantFixes == 2) cout << " +1 PT";
        if (cantFixes == 3) cout << " +2 PT";
        cout << endl;
    }
}


//MUESTRA LA PANTALLA DE JUEGO ACTUAL

void mostrarPanelRonda(string nombreJug[2], int puntosTotales[2], int pt[2], int bugs[2], int turno, int ronda, int puntosRonda, int lanzamientos)
{
    cout << "                         TREY" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << nombreJug[0] << ":" << puntosTotales[0] << " de " << puntosTotales[0] + pt[0] << " puntos   ";
    cout << "                "<<nombreJug[1] << ":" << puntosTotales[1] << " de " << puntosTotales[1] + pt[1] << " puntos" << endl;
    cout << "PT: " << pt[0] << " / BUGS: " << bugs[0] << "   ";
    cout << "                    " "PT: " << pt[1] << " / BUGS: " << bugs[1] << endl;
    cout << "TURNO DE " << nombreJug[turno] << endl;
    cout << "+---------------------------------+" << endl;
    cout << "| RONDA #" <<ronda<<"                        |" << endl;
    cout << "| PUNTOS DE LA RONDA: " <<   puntosRonda << "           |" << endl;
    cout << "| LANZAMIENTOS: " << lanzamientos << "                 |" << endl;
    cout << "+---------------------------------+" << endl;

    cout <<endl;
}

// Muestra mensaje de estadísticas
void ordenarRanking (string nombreJug[], int puntosTotales [],int cantidad)
{
    int auxPuntosTotales;
    string auxNombreJug;
    for (int i=0; i<cantidad-1; i++)
    {
                                            //Intercambio de la variable auxiliar
        for (int j=0; j<cantidad-1; j++)
        {
            if(puntosTotales[j]<puntosTotales[j+1])
            {
                auxPuntosTotales= puntosTotales [j];
                puntosTotales[j]=puntosTotales [j+1];
                puntosTotales [j+1]= auxPuntosTotales;
                auxNombreJug= nombreJug[j];
                nombreJug [j] =nombreJug [j+1];
                nombreJug[j+1]=auxNombreJug;
            }
        }
    }
}


void mostrarEstadisticas(string nombreJug [5], int puntosTotales [5], int cantidad)
{
    int cantidadJugAMostrar=5;
    ordenarRanking(nombreJug,puntosTotales,cantidad);
    //Si hay menos de 5 jugadores que muestre los que haya
    if (cantidad <5)
    {
        cantidadJugAMostrar=cantidad;
    }

    cout<< "----Top "<<cantidadJugAMostrar<<" jugadores----" <<endl;

    for (int i=0; i<cantidadJugAMostrar; i++)
    {
        cout<< (i+1)<<". "<<nombreJug [i]<<" :"<<puntosTotales [i]<<" puntos totales"<<endl;

    }
    cout << "Jugador con mas puntos: "<<nombreJug [0]<<" ( "<<puntosTotales [0]<<" puntos)."<<endl;

}

// MUESTRA LOS CREDITOS DE LOS INTEGRANTES
void mostrarCreditos()
{
    cout << "                                       ------------INTEGRANTES-------------" << endl;
    cout << "                                       Ponce Alejandro - Legajo nro: 32583" << endl;
    cout << "                                       Troncoso Gabriela - Legajo nro: 32662" << endl;
    cout << "                                       Robledo Pablo - Legajo nro: 27909" << endl;
    cout <<endl;
    pausarPantalla();
}

// CONFIRMA SI EL JUGADOR REALMENTE QUIERE SALIR
bool confirmarSalida()
{
    char respuesta;
    do
    {
        cout << "Seguro que desea salir? (S/N): ";
        cin >> respuesta;
        respuesta = toupper(respuesta);
    }
    while (respuesta != 'S' && respuesta != 'N');


    return (respuesta == 'S');
}

// LIMPIA LA PANTALLA SEGUN EL SISTEMA OPERATIVO
void limpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// PAUSA HASTA QUE SE PRESIONE ENTER
void pausarPantalla()
{
    cout << "                                       Presione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

//MUESTRA LOS DADOS DIBUJADOS
void dibujarDados(int vDado[], int cantidad)
{


    for (int i = 0; i < cantidad; i++)
    {

        cout << "+-------+" << endl;
        switch (vDado[i])
        {
        case 1:
            cout << "|       |" << endl;
            cout << "|   *   |" << endl;
            cout << "|       |" << endl;
            break;
        case 2:
            cout << "| *     |" << endl;
            cout << "|       |" << endl;
            cout << "|     * |" << endl;
            break;
        case 3:
            cout << "| *     |" << endl;
            cout << "|   *   |" << endl;
            cout << "|     * |" << endl;
            break;
        case 4:
            cout << "| *   * |" << endl;
            cout << "|       |" << endl;
            cout << "| *   * |" << endl;
            break;
        case 5:
            cout << "| *   * |" << endl;
            cout << "|   *   |" << endl;
            cout << "| *   * |" << endl;
            break;
        case 6:
            cout << "| *   * |" << endl;
            cout << "| *   * |" << endl;
            cout << "| *   * |" << endl;
            break;
        }
        cout << "+-------+" << endl;
    }

}

//MOSTRAR RESULTADOS FINALES

void mostrarResultadoFinales (string nombreJug[], int puntosTotales[], int pt[],int bugs[])
{
    cout<< "-------RESULTADOS FINALES-------"<<endl;

    for (int i=0; i<2; i++)
    {
        cout<<nombreJug[i]<<":"<<endl;
        cout<<"Puntos totales: "<<puntosTotales[i]<<endl;
        cout<< "BUGS: "<<bugs[i]<<endl;
        cout<<"Puntos restantes: "<<pt[i]<<endl;
        cout<<"----------------------------------"<<endl;
    }

    if (puntosTotales[0]>puntosTotales[1])
        cout<< "Ganador: "<<nombreJug[0]<<"!"<<endl;
    else if (puntosTotales[1]>puntosTotales[0])
        cout<<"Ganador: "<<nombreJug[1]<<"!"<<endl;
    else
        cout<<"¡Empate!"<<endl;
}

// Para guardar juegos- pendiente
void guardarJuego(string jugador, int puntaje, int rondasGanadas, int empates, int rondasTotales, int puntuacionFinal)
{

}
/*Faltar├¡a:
*Mostrar una pantalla de resultado final con:

*Puntaje total

*BUGS

*PT restantes

*Hitos (opcional)

Ganador final*/