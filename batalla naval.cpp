#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

struct Barco {
    string nombre;
    int tam;
    vector<pair<int, int>> posiciones;
    int toques = 0;
    bool hundido = false;
};

void inicializarMapa(char mapa[][10], int FILAS, int COLS) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            mapa[i][j] = '~';
        }
    }
}

void inicializarDisparos(char disparos[][10], int FILAS, int COLS) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            disparos[i][j] = '~';  // '~' indica no disparado
        }
    }
}

void mostrarMapa(const char mapa[][10], int FILAS, int COLS, bool esPropio) {
    cout << "\n ";
    for (int c = 0; c < COLS; c++) {
        cout << " " << c;
    }
    cout << "\n";
    for (int f = 0; f < FILAS; f++) {
        cout << f << " |";
        for (int c = 0; c < COLS; c++) {
            if (esPropio && mapa[f][c] == '~') {
                cout << " ~";  // Muestra '~' durante colocación para ver el espacio vacío
            } else {
                cout << " " << mapa[f][c];
            }
        }
        cout << "\n";
    }
}

void mostrarDisparos(const char disparos[][10], int FILAS, int COLS) {
    cout << "\nTu tablero de disparos:\n";
    mostrarMapa(disparos, FILAS, COLS, false);  // No es propio, muestra todo
}

bool colocarBarco(char mapa[][10], Barco& barco, int fila, int col, char ori, int FILAS, int COLS) {
    bool ok = true;
    vector<pair<int, int>> pos;
    if (ori == 'H' || ori == 'h') {
        if (col + barco.tam > COLS) ok = false;
        else {
            for (int k = 0; k < barco.tam; k++) {
                if (mapa[fila][col + k] != '~') {
                    ok = false;
                    break;
                }
                pos.push_back({fila, col + k});
                // Colocar símbolo temporalmente para visualización
                if (k == 0) mapa[fila][col + k] = '<';
                else if (k == barco.tam - 1) mapa[fila][col + k] = '>';
                else mapa[fila][col + k] = '-';
            }
        }
    } else {
        if (fila + barco.tam > FILAS) ok = false;
        else {
            for (int k = 0; k < barco.tam; k++) {
                if (mapa[fila + k][col] != '~') {
                    ok = false;
                    break;
                }
                pos.push_back({fila + k, col});
                // Colocar símbolo
                if (k == 0) mapa[fila + k][col] = '^';
                else if (k == barco.tam - 1) mapa[fila + k][col] = 'v';
                else mapa[fila + k][col] = '|';
            }
        }
    }
    if (ok) {
        barco.posiciones = pos;
    } else {
        // Revertir si no ok
        for (auto& p : pos) {
            mapa[p.first][p.second] = '~';
        }
    }
    return ok;
}

bool todosHundidos(const Barco barcos[], int NUM_BARCOS) {
    for (int i = 0; i < NUM_BARCOS; i++) {
        if (!barcos[i].hundido) return false;
    }
    return true;
}

void procesarDisparo(char mapaEnemigo[][10], char disparosPropio[][10], Barco barcosEnemigo[], int fila, int col, int NUM_BARCOS, bool& acierto, string& mensaje) {
    acierto = false;
    mensaje = "";
    if (mapaEnemigo[fila][col] != '~' && mapaEnemigo[fila][col] != 'X' && mapaEnemigo[fila][col] != 'O') {
        // Es un barco (símbolo)
        mapaEnemigo[fila][col] = 'X';
        disparosPropio[fila][col] = 'X';
        acierto = true;
        mensaje = "¡Tocado! ";
        
        // Encontrar y tocar el barco
        for (int i = 0; i < NUM_BARCOS; i++) {
            for (auto& p : barcosEnemigo[i].posiciones) {
                if (p.first == fila && p.second == col) {
                    barcosEnemigo[i].toques++;
                    if (barcosEnemigo[i].toques == barcosEnemigo[i].tam) {
                        barcosEnemigo[i].hundido = true;
                        mensaje += "¡Hundido: " + barcosEnemigo[i].nombre + "! ";
                    }
                    break;
                }
            }
        }
    } else if (mapaEnemigo[fila][col] == '~') {
        // Agua
        mapaEnemigo[fila][col] = 'O';  // Opcional, para tracking interno
        disparosPropio[fila][col] = 'O';
        mensaje = "¡Agua! ";
    } else {
        mensaje = "Ya disparaste ahí. ";
    }
}

int main() {
    char jugar;
    cout << "¿Quieres jugar Batalla Naval? (s/n): ";
    cin >> jugar;
    if (jugar != 's' && jugar != 'S') {
        cout << "Programa terminado.\n";
        return 0;
    }

//Revision26septiembre2006


    const int FILAS = 10, COLS = 10;
    const int NUM_BARCOS = 5;
    string nombres[NUM_BARCOS] = {"Portaaviones", "Acorazado", "Submarino", "Destructor", "Lancha"};
    int tamanos[NUM_BARCOS] = {5, 4, 3, 3, 2};

    // Jugador 1
    cout << "\n=== JUGADOR 1: Coloca tus barcos ===" << endl;
    char mapa1[FILAS][COLS];
    Barco barcos1[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++) {
        barcos1[i].nombre = nombres[i];
        barcos1[i].tam = tamanos[i];
    }
    inicializarMapa(mapa1, FILAS, COLS);
    
    // Mostrar mapa vacío inicial para Jugador 1
    auto mostrarMapa1 = [&]() {
        cout << "\nTu mapa (barcos visibles para ti):" << endl;
        mostrarMapa(mapa1, FILAS, COLS, true);
    };
    mostrarMapa1();  // <-- AGREGADO: Mostrar mapa inicial vacío
    
    bool colocado1[NUM_BARCOS] = {false};
    int colocados1 = 0;

    while (colocados1 < NUM_BARCOS) {
        cout << "\nBarcos disponibles:" << endl;
        for (int i = 0; i < NUM_BARCOS; i++) {
            if (!colocado1[i]) {
                cout << " " << i + 1 << ". " << nombres[i] << " (long " << tamanos[i] << ")" << endl;
            }
        }
        int sel;
        cout << "\nSelecciona número de barco para colocar: ";
        cin >> sel;
        sel--;
        if (sel < 0 || sel >= NUM_BARCOS || colocado1[sel]) {
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
        cout << "\n¿Seguro de colocar '" << nombres[sel] << "' long " << tamanos[sel]
             << " en (" << fila << "," << col << ") ori " << ori << "? (s/n): ";
        char conf;
        cin >> conf;
        if (conf != 's' && conf != 'S') {
            cout << "Colocación cancelada.\n";
            continue;
        }
        if (colocarBarco(mapa1, barcos1[sel], fila, col, ori, FILAS, COLS)) {
            colocado1[sel] = true;
            colocados1++;
            cout << "\n¡Barco colocado! Tablero actualizado:" << endl;
            mostrarMapa1();
        } else {
            cout << "No cabe o hay choque con otro barco.\n";
        }
    }

    cout << "\n¿Estás seguro de estas posiciones? (s/n): ";
    char finalConf1;
    cin >> finalConf1;
    if (finalConf1 != 's' && finalConf1 != 'S') {
        cout << "Reiniciando colocación para Jugador 1... (Implementa reinicio si deseas)\n";
        // Por simplicidad, continuamos
    } else {
        cout << "¡Configuración de barcos de Jugador 1 completada!\n";
    }

    // Jugador 2
    cout << "\n=== JUGADOR 2: Coloca tus barcos ===" << endl;
    // Limpiar pantalla conceptualmente (en consola real, usa system("clear") o similar, pero omitimos)
    char mapa2[FILAS][COLS];
    Barco barcos2[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++) {
        barcos2[i].nombre = nombres[i];
        barcos2[i].tam = tamanos[i];
    }
    inicializarMapa(mapa2, FILAS, COLS);
    
    // Mostrar mapa vacío inicial para Jugador 2
    auto mostrarMapa2 = [&]() {
        cout << "\nTu mapa (barcos visibles para ti):" << endl;
        mostrarMapa(mapa2, FILAS, COLS, true);
    };
    mostrarMapa2();  // <-- AGREGADO: Mostrar mapa inicial vacío
    
    bool colocado2[NUM_BARCOS] = {false};
    int colocados2 = 0;

    while (colocados2 < NUM_BARCOS) {
        cout << "\nBarcos disponibles:" << endl;
        for (int i = 0; i < NUM_BARCOS; i++) {
            if (!colocado2[i]) {
                cout << " " << i + 1 << ". " << nombres[i] << " (long " << tamanos[i] << ")" << endl;
            }
        }
        int sel;
        cout << "\nSelecciona número de barco para colocar: ";
        cin >> sel;
        sel--;
        if (sel < 0 || sel >= NUM_BARCOS || colocado2[sel]) {
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
        cout << "\n¿Seguro de colocar '" << nombres[sel] << "' long " << tamanos[sel]
             << " en (" << fila << "," << col << ") ori " << ori << "? (s/n): ";
        char conf;
        cin >> conf;
        if (conf != 's' && conf != 'S') {
            cout << "Colocación cancelada.\n";
            continue;
        }
        if (colocarBarco(mapa2, barcos2[sel], fila, col, ori, FILAS, COLS)) {
            colocado2[sel] = true;
            colocados2++;
            cout << "\n¡Barco colocado! Tablero actualizado:" << endl;
            mostrarMapa2();
        } else {
            cout << "No cabe o hay choque con otro barco.\n";
        }
    }

    cout << "\n¿Estás seguro de estas posiciones? (s/n): ";
    char finalConf2;
    cin >> finalConf2;
    if (finalConf2 != 's' && finalConf2 != 'S') {
        cout << "Reiniciando colocación para Jugador 2... (Implementa reinicio si deseas)\n";
        // Por simplicidad, continuamos
    } else {
        cout << "¡Configuración de barcos de Jugador 2 completada!\n";
    }

    // Inicializar tableros de disparos
    char disparos1[FILAS][COLS];  // Disparos de Jug1 en mapa2
    char disparos2[FILAS][COLS];  // Disparos de Jug2 en mapa1
    inicializarDisparos(disparos1, FILAS, COLS);
    inicializarDisparos(disparos2, FILAS, COLS);

    // Juego
    cout << "\n=== INICIO DEL JUEGO ===" << endl;
    cout << "Reglas: Alternan turnos. Di coordenadas para disparar al oponente." << endl;
    cout << "Si aciertas, disparas de nuevo. Gana quien hunda todos los barcos enemigos." << endl;

    bool turnoJug1 = true;  // Empieza Jug1
    bool juegoTerminado = false;
    string ganador;

    while (!juegoTerminado) {
        if (turnoJug1) {
            cout << "\n--- TURNO DEL JUGADOR 1 ---" << endl;
            mostrarDisparos(disparos1, FILAS, COLS);
            mostrarMapa(mapa1, FILAS, COLS, true);  // Mostrar propio para referencia (sin revelar agua)
            int fila, col;
            cout << "Dispara: Introduce fila (0-9): ";
            cin >> fila;
            cout << "Introduce columna (0-9): ";
            cin >> col;
            if (fila < 0 || fila >= FILAS || col < 0 || col >= COLS) {
                cout << "Coordenadas inválidas. Pierdes turno.\n";
                turnoJug1 = false;
                continue;
            }
            bool acierto;
            string mensaje;
            procesarDisparo(mapa2, disparos1, barcos2, fila, col, NUM_BARCOS, acierto, mensaje);
            cout << mensaje << endl;
            if (todosHundidos(barcos2, NUM_BARCOS)) {
                juegoTerminado = true;
                ganador = "Jugador 1";
                break;
            }
            if (!acierto) {
                turnoJug1 = false;
            }
            // Mostrar actualizado
            mostrarDisparos(disparos1, FILAS, COLS);
        } else {
            cout << "\n--- TURNO DEL JUGADOR 2 ---" << endl;
            mostrarDisparos(disparos2, FILAS, COLS);
            mostrarMapa(mapa2, FILAS, COLS, true);  // Mostrar propio
            int fila, col;
            cout << "Dispara: Introduce fila (0-9): ";
            cin >> fila;
            cout << "Introduce columna (0-9): ";
            cin >> col;
            if (fila < 0 || fila >= FILAS || col < 0 || col >= COLS) {
                cout << "Coordenadas inválidas. Pierdes turno.\n";
                turnoJug1 = true;
                continue;
            }
            bool acierto;
            string mensaje;
            procesarDisparo(mapa1, disparos2, barcos1, fila, col, NUM_BARCOS, acierto, mensaje);
            cout << mensaje << endl;
            if (todosHundidos(barcos1, NUM_BARCOS)) {
                juegoTerminado = true;
                ganador = "Jugador 2";
                break;
            }
            if (!acierto) {
                turnoJug1 = true;
            }
            // Mostrar actualizado
            mostrarDisparos(disparos2, FILAS, COLS);
        }
    }

    cout << "\n¡Juego terminado! Ganador: " << ganador << endl;
    cout << "¡Felicidades!" << endl;

    return 0;
}
