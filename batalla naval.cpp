#include <iostream>
#include <cstdlib>  
#include <ctime>     

using namespace std;

int main() {

    char jugar;
    cout << "¿Quieres jugar Batalla Naval? (s/n): ";
    cin >> jugar;
    if (jugar != 's' && jugar != 'S') {
        cout << "Programa terminado.\n";
        return 0;
    }

    const int FILAS = 10, COLS = 10;
    char mapa[FILAS][COLS];
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLS; j++)
            mapa[i][j] = '~';

  
    auto mostrarMapa = [&]() {
     
        cout << "\n    ";
        for (int c = 0; c < COLS; c++)
            cout << c << "  ";
        cout << "\n";

       
        for (int f = 0; f < FILAS; f++) {
            cout << f << " | ";
            for (int c = 0; c < COLS; c++)
                cout << mapa[f][c] << "  ";
            cout << "\n";
        }
    };

    mostrarMapa();

    const int NUM_BARCOS = 5;
    string nombres[NUM_BARCOS] = {
        "Portaaviones", "Acorazado", "Submarino",
        "Destructor",    "Lancha"
    };
    int tamanos[NUM_BARCOS]  = {5, 4, 3, 3, 2};
    bool colocado[NUM_BARCOS] = {false, false, false, false, false};

    int colocados = 0;
    srand(time(NULL));

  
    while (colocados < NUM_BARCOS) {
        cout << "\nBarcos disponibles:\n";
        for (int i = 0; i < NUM_BARCOS; i++) {
            if (!colocado[i])
                cout << " " << i+1 << ". "
                     << nombres[i]
                     << " (long " << tamanos[i] << ")\n";
        }

        int sel;
        cout << "\nSelecciona número de barco para colocar: ";
        cin >> sel;
        sel--;
        if (sel < 0 || sel >= NUM_BARCOS || colocado[sel]) {
            cout << "Selección inválida. Intenta de nuevo.\n";
            continue;
        }

        int fila, col;
        char ori;
        cout << "Introduce fila (0-9): ";
        cin >> fila;
        cout << "Introduce columna (0-9): ";
        cin >> col;
        cout << "Orientación Horizontal (H) o Vertical (V): ";
        cin >> ori;

        cout << "\n¿Seguro de colocar '"
             << nombres[sel] << "' long "
             << tamanos[sel] << " en ("
             << fila << "," << col << ") ori "
             << ori << "? (s/n): ";
        char conf;
        cin >> conf;
        if (conf != 's' && conf != 'S') {
            cout << "Colocación cancelada.\n";
            continue;
        }

        bool ok = true;
        if (ori == 'H' || ori == 'h') {
            if (col + tamanos[sel] > COLS) ok = false;
            else
                for (int k = 0; k < tamanos[sel]; k++)
                    if (mapa[fila][col + k] != '~')
                        ok = false;
        } else {
            if (fila + tamanos[sel] > FILAS) ok = false;
            else
                for (int k = 0; k < tamanos[sel]; k++)
                    if (mapa[fila + k][col] != '~')
                        ok = false;
        }
        if (!ok) {
            cout << "No cabe o hay choque con otro barco.\n";
            continue;
        }

        if (ori == 'H' || ori == 'h') {
            for (int k = 0; k < tamanos[sel]; k++) {
                if (k == 0) 
                    mapa[fila][col + k] = '<';
                else if (k == tamanos[sel] - 1)
                    mapa[fila][col + k] = '>';
                else 
                    mapa[fila][col + k] = '-';
            }
        } else {
            for (int k = 0; k < tamanos[sel]; k++) {
                if (k == 0) 
                    mapa[fila + k][col] = '^';
                else if (k == tamanos[sel] - 1)
                    mapa[fila + k][col] = 'v';
                else 
                    mapa[fila + k][col] = '|';
            }
        }

        colocado[sel] = true;
        colocados++;

        cout << "\n¡Barco colocado! Tablero actualizado:";
        mostrarMapa();
    }
    cout << "\n¿Estás seguro de estas posiciones? (s/n): ";
    char finalConf;
    cin >> finalConf;
    if (finalConf != 's' && finalConf != 'S')
        cout << "Aquí podrías reiniciar, pero esta parte 1 termina.\n";
    else
        cout << "¡Configuración de barcos completada!\n";

    return 0;
}

#Revision26septiembre2006

