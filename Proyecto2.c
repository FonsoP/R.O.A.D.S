#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define LOCALIZACIONES_MAXIMA 10

typedef struct {
    float distancia;
    float horas; // Como tenemos la conversión realmente no es necesario guardarlo, solo es para cambios futuros que lo requieran.
} Rutas;

// Prototipo de funciones:
// Prototipo de función para validar rango ASCII
int ValidarRangoLetras(char caracter);
void InicializarGrafo(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA]);
void CargarRutas(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], const char* filename);
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int nodo_origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void MostrarRutaOptima(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], char origin, char destination, int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void MostrarRutasDadoOrigen(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origin, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);
void showMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]);

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

    // int sourceNode = 1;
    
    float distancias[LOCALIZACIONES_MAXIMA]; // Cambiado a float
    for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        distancias[i] = INT_MAX;
    }
	
    // dijkstra(grafo, sourceNode, distancias, nodos_anteriores);

    showMenu(grafo, distancias, nodos_anteriores);

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
        return;
    }

    char line[100];
    char origin, destination;
    float distancia, horas; // Cambiado a float.

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, " %c | %c | %f | %f hora", &origin, &destination, &distancia, &horas) == 4) {
            int originIndex = origin - 'A';
            int destinationIndex = destination - 'A';

            grafo[originIndex][destinationIndex].distancia = distancia;
            grafo[originIndex][destinationIndex].horas = horas;
        }
    }

    fclose(file);
    
    // Verificar que se esta guardando:
    
}

// Función dijkstra solicitada:
/* Función adaptada para arreglos vista en clase */
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int nodo_origen, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    bool visited[LOCALIZACIONES_MAXIMA] = {false};

    distancias[nodo_origen] = 0;
    nodos_anteriores[nodo_origen] = -1; // Para el nodo nodo de origen, no hay nodo anterior.

    for (int count = 0; count < LOCALIZACIONES_MAXIMA - 1; count++) {
        float minDistance = INT_MAX; 
		int  minIndex;
	
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visited[v] && distancias[v] < minDistance) {
                minDistance = distancias[v];
                minIndex = v;
            }
        }

        visited[minIndex] = true;
		
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visited[v] && grafo[minIndex][v].distancia != INT_MAX &&
                distancias[minIndex] != INT_MAX &&
                distancias[minIndex] + grafo[minIndex][v].distancia < distancias[v]) {
                distancias[v] = distancias[minIndex] + grafo[minIndex][v].distancia;
                nodos_anteriores[v] = minIndex; // Almacenar el nodo anterior para reconstruir el recorrido.
            }
        }
    }
}

void MostrarRutaOptima(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], char origin, char destination, int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    
    // Inicialización ASCII, se asume que el destino y el origen están en un rango A-Z
	int originIndex = origin - 'A';
    int destIndex = destination - 'A';
	
	/* Nota del programador: Se asume que en este caso el valor es el de inicialización que teóricamente es imposible como distancia lógica */
	
    if (distancias[destIndex] == INT_MAX) {
        printf("\nNo hay forma de llegar desde %c hasta %c\n", origin, destination);
        return;
    }

    printf("\nRuta mas optima desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\tRecorrido\n");

	// Se hace la conversión según las condiciones dadas
    printf("%.2f\t\t%.2f horas\t\t", distancias[destIndex], (float)distancias[destIndex] / 100);

    // Almacena el recorrido en un array para invertirlo.
    char route[LOCALIZACIONES_MAXIMA];
    int length = 0;
    int current = destIndex;
    while (current != -1) {
        route[length++] = current + 'A';
        current = nodos_anteriores[current];
    }

    // Imprime el recorrido invertido
    for (int i = length - 1; i >= 0; i--) {
        printf("%c", route[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }

    printf("\n");
}

void MostrarRutasDadoOrigen(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origin, float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    
	// Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int originIndex = origin - 'A';

    printf("\nTodas las rutas desde %c:\n", origin);

    for (int destIndex = 0; destIndex < LOCALIZACIONES_MAXIMA; destIndex++) { // Buscar hasta que se acaben las posibles opciones
        if (destIndex != originIndex) { // Validación para que un nodo no se busque consigo mismo
            MostrarRutaOptima(grafo, distancias, origin, destIndex + 'A', nodos_anteriores); // Ir alternando constantemente el nodo de destino
        }
    }
}

void showMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distancias[LOCALIZACIONES_MAXIMA], int nodos_anteriores[LOCALIZACIONES_MAXIMA]) {
    int choice;
    char origin, destination;
	int sourceNode;
	
    do {
        printf("\nMenu:\n");
        printf("1. Mostrar todos los destinos dado un origen\n");
        printf("2. Mostrar el recorrido mas optimo de un origen a un destino\n");
        printf("3. Salir\n");

        printf("Ingrese su eleccion: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origin);
                if (ValidarRangoLetras(origin) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
    			sourceNode= origin - 'A';
    			dijkstra(grafo, sourceNode, distancias, nodos_anteriores);
                printf("Destinos desde %c:\n", origin);
                MostrarRutasDadoOrigen(grafo, origin, distancias, nodos_anteriores);
                for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        			distancias[i] = INT_MAX;
    			}
                break;
            case 2:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origin);
                if (ValidarRangoLetras(origin) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
                printf("Ingrese el destino (A-J): ");
                scanf(" %c", &destination);
                if (ValidarRangoLetras(destination) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
    			sourceNode= origin - 'A';
    			dijkstra(grafo, sourceNode, distancias, nodos_anteriores);
                MostrarRutaOptima(grafo, distancias, origin, destination, nodos_anteriores);
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

    } while (choice != 3);
}
