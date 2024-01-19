#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#define LOCALIZACIONES_MAXIMA 10

typedef struct {
    float distancia;
    float horas; // Como tenemos la conversión realmente no es necesario guardarlo, solo es para cambios futuros que lo requieran.
} Rutas;

// Prototipo de funciones:
int ValidarRangoLetras(char caracter);
void InicializarGrafo(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA]);
void CargarRutas(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], const char* filename);
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int nodo_origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void MostrarRutaOptima(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], char origen, char destino, int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void MostrarRutasDadoOrigen(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void MostrarMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);

int main() {
    Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA];
    InicializarGrafo(grafo);
    
    /* Notas del programador:
    printf("Tenga en cuenta que el valor 2147483647 indica que no hay forma de realizar tal recorrido (distancias)\n");
    printf("Tenga en cuenta que el valor 21474836.00 indica que no hay forma de realizar tal recorrido (horas)\n"); */

    CargarRutas(grafo, "prueba.txt");

	int nodos_anteriores[LOCALIZACIONES_MAXIMA]; // Array para almacenar los nodos anteriores durante el recorrido.
    for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        nodos_anteriores[i] = -1; // Inicializar todos los nodos anteriores a -1.
    }

    // int nodo_origen_busqueda = 1;
    
    float distancias[LOCALIZACIONES_MAXIMA]; // Cambiado a float
    for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        distancias[i] = INT_MAX;
    }
	
    // dijkstra(grafo, nodo_origen_busqueda, distancias, nodos_anteriores);

    MostrarMenu(grafo, distancias, nodos_anteriores);

    return 0;
}

// Función para validar rango ASCII
int ValidarRangoLetras(char caracter) {
    return (caracter >= 'A' && caracter <= 'J');
}

// Inicializar matriz de grafos
void InicializarGrafo(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA]) {
    for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        for (int j = 0; j < LOCALIZACIONES_MAXIMA; j++) {
            grafo[i][j].distancia = INT_MAX;
            grafo[i][j].horas = INT_MAX;
        }
    }
}

// Cargar rutas del archivo
void CargarRutas(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        exit(-1); // Salir del programa
    }

    char linea[100];
    char origen, destino;
    float distancia, horas; // Cambiado a float.

    while (fgets(linea, sizeof(linea), file) != NULL) {
        if (sscanf(linea, " %c | %c | %f | %f hora", &origen, &destino, &distancia, &horas) == 4) {
            int indice_origen = origen - 'A';
            int indice_relativo = destino - 'A'; // Indice de destino o relativo (es aquel relacionado con el origen)

			// Validar si la relación entre distancia y horas es lógica
            if (distancia / 100 != horas) {
                printf("Error: Los valores de distancia y horas no muestran correspondencia logica.\n");
                exit(-1);  // Salir del programa
            }

            grafo[indice_origen][indice_relativo].distancia = distancia;
            grafo[indice_origen][indice_relativo].horas = horas;
        }
    }

    fclose(file);
    
}

// Función dijkstra solicitada:
/* Función adaptada para arreglos vista en clase */
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int nodo_origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    bool visitado[LOCALIZACIONES_MAXIMA] = {false};

    distancias[nodo_origen] = 0;
    nodos_anteriores[nodo_origen] = -1; // Para el nodo nodo de origen, no hay nodo anterior.

    for (int i = 0; i < LOCALIZACIONES_MAXIMA - 1; i++) {
        float distancia_minima = INT_MAX; // Inicialización de la única distancia númerica la cual no sería lógica (a parte de valores negativos)
		int  indice_distancia_minima;
	
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visitado[v] && distancias[v] < distancia_minima) {
                distancia_minima = distancias[v];
                indice_distancia_minima = v;
            }
        }

        visitado[indice_distancia_minima] = true;
		
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visitado[v] && grafo[indice_distancia_minima][v].distancia != INT_MAX &&
                distancias[indice_distancia_minima] != INT_MAX &&
                distancias[indice_distancia_minima] + grafo[indice_distancia_minima][v].distancia < distancias[v]) {
                distancias[v] = distancias[indice_distancia_minima] + grafo[indice_distancia_minima][v].distancia;
                nodos_anteriores[v] = indice_distancia_minima; // Almacenar el nodo anterior para reconstruir el recorrido.
            }
        }
    }
}

void MostrarRutaOptima(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], char origen, char destino, int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    
    // Inicialización ASCII, se asume que el destino y el origen están en un rango A-Z
	int indice_origen = origen - 'A';
    int indice_destino = destino - 'A';
	
	/* Nota del programador: Se asume que en este caso el valor es el de inicialización que teóricamente es imposible como distancia lógica */
	
    if (distancias[indice_destino] == INT_MAX) {
        printf("\nNo hay forma de llegar desde %c hasta %c\n", origen, destino);
        return;
    }

    printf("\nRuta mas optima desde %c hasta %c:\n", origen, destino);
    printf("Distancia\tTiempo (en horas)\tRecorrido\n");

	// Se hace la conversión según las condiciones dadas
    printf("%.2f\t\t%.2f horas\t\t", distancias[indice_destino], (float)distancias[indice_destino] / 100);

    // Almacena el recorrido en un array para invertirlo.
    char ruta[LOCALIZACIONES_MAXIMA];
    int longitud_ruta = 0;
    int actual = indice_destino;
    while (actual != -1) {
        ruta[longitud_ruta++] = actual + 'A';
        actual = nodos_anteriores[actual];
    }

    // Imprime el recorrido invertido
    for (int i = longitud_ruta - 1; i >= 0; i--) {
        printf("%c", ruta[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }

    printf("\n");
}

void MostrarRutasDadoOrigen(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    
	// Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int indice_origen = origen - 'A';

    printf("\nTodas las rutas desde %c:\n", origen);

    for (int indice_destino = 0; indice_destino < LOCALIZACIONES_MAXIMA; indice_destino++) { // Buscar hasta que se acaben las posibles opciones
        if (indice_destino != indice_origen) { // Validación para que un nodo no se busque consigo mismo
            MostrarRutaOptima(grafo, distancias, origen, indice_destino + 'A', nodos_anteriores); // Ir alternando constantemente el nodo de destino
        }
    }
}

void MostrarMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    int opcion;
    char origen, destino;
	int nodo_origen_busqueda;
	
    do {
        printf("\nMenu:\n");
        printf("1. Mostrar todos los destinos dado un origen\n");
        printf("2. Mostrar el recorrido mas optimo de un origen a un destino\n");
        printf("3. Salir\n");

        printf("Ingrese su eleccion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origen);
                if (ValidarRangoLetras(origen) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
    			nodo_origen_busqueda= origen - 'A';
    			dijkstra(grafo, nodo_origen_busqueda, distancias, nodos_anteriores);
                printf("Destinos desde %c:\n", origen);
                MostrarRutasDadoOrigen(grafo, origen, distancias, nodos_anteriores);
                for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        			distancias[i] = INT_MAX;
    			}
                break;
            case 2:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origen);
                if (ValidarRangoLetras(origen) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
                printf("Ingrese el destino (A-J): ");
                scanf(" %c", &destino);
                if (ValidarRangoLetras(destino) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
    			nodo_origen_busqueda= origen - 'A';
    			dijkstra(grafo, nodo_origen_busqueda, distancias, nodos_anteriores);
                MostrarRutaOptima(grafo, distancias, origen, destino, nodos_anteriores);
                for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        			distancias[i] = INT_MAX;
    			}
                break;
            case 3:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
        }

    } while (opcion != 3);
}
