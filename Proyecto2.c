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
int validateInputRange(char character);
void InicializarGrafo(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA]);
void CargarRutas(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], const char* filename);
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int source, float distances[LOCALIZACIONES_MAXIMA]);
void showOptimalRoute(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distances[LOCALIZACIONES_MAXIMA], char origin, char destination);
void showAllRoutesFromOrigin(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origin, float distances[LOCALIZACIONES_MAXIMA]);
void showMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distances[LOCALIZACIONES_MAXIMA]);

int main() {
    Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA];
    InicializarGrafo(grafo);
    
    /* Notas del programador:
    printf("Tenga en cuenta que el valor 2147483647 indica que no hay forma de realizar tal recorrido (distancias)\n");
    printf("Tenga en cuenta que el valor 21474836.00 indica que no hay forma de realizar tal recorrido (horas)\n"); */

    CargarRutas(grafo, "prueba.txt");

    int sourceNode = 0;
    float distances[LOCALIZACIONES_MAXIMA]; // Cambiado a float
    for (int i = 0; i < LOCALIZACIONES_MAXIMA; i++) {
        distances[i] = INT_MAX;
    }
	
    dijkstra(grafo, sourceNode, distances);

    showMenu(grafo, distances);

    return 0;
}

// Función para validar rango ASCII
int validateInputRange(char character) {
    return (character >= 'A' && character <= 'J');
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
}

// Función dijkstra solicitada:
/* Función adaptada para arreglos vista en clase */
void dijkstra(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], int source, float distances[LOCALIZACIONES_MAXIMA]) {
    bool visited[LOCALIZACIONES_MAXIMA] = {false};

    distances[source] = 0;

    for (int count = 0; count < LOCALIZACIONES_MAXIMA - 1; count++) {
        float minDistance = INT_MAX; 
		int  minIndex;
	
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        visited[minIndex] = true;
		
        for (int v = 0; v < LOCALIZACIONES_MAXIMA; v++) {
            if (!visited[v] && grafo[minIndex][v].distancia != INT_MAX &&
                distances[minIndex] != INT_MAX &&
                distances[minIndex] + grafo[minIndex][v].distancia < distances[v]) {
                distances[v] = distances[minIndex] + grafo[minIndex][v].distancia;
            }
        }
    }
}

void showOptimalRoute(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distances[LOCALIZACIONES_MAXIMA], char origin, char destination) {
    
    // Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int originIndex = origin - 'A';
    int destIndex = destination - 'A';
	
	/* Nota del programador: Se asume que en este caso el valor es el de inicialización que teóticamente es imposible como distancia lógica */
	
    if (distances[destIndex] == INT_MAX) {
        printf("\nNo hay forma de llegar desde %c hasta %c\n", origin, destination);
        return;
    }

    printf("\nRuta mas optima desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\n");

	// Se hace la conversión según las condiciones dadas
    printf("%.2f\t\t%.2f horas\n", distances[destIndex], (float)distances[destIndex] / 100);
}

void showAllRoutesFromOrigin(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], char origin, float distances[LOCALIZACIONES_MAXIMA]) {
    
	// Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int originIndex = origin - 'A';

    printf("\nTodas las rutas desde %c:\n", origin);

    for (int destIndex = 0; destIndex < LOCALIZACIONES_MAXIMA; destIndex++) { // Buscar hasta que se acaben las posibles opciones
        if (destIndex != originIndex) { // Validación para que un nodo no se busque consigo mismo
            showOptimalRoute(grafo, distances, origin, destIndex + 'A'); // Ir alternando constantemente el nodo de destino
        }
    }
}

void showMenu(Rutas grafo[LOCALIZACIONES_MAXIMA][LOCALIZACIONES_MAXIMA], float distances[LOCALIZACIONES_MAXIMA]) {
    int choice;
    char origin, destination;

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
                if (validateInputRange(origin) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
                printf("Destinos desde %c:\n", origin);
                showAllRoutesFromOrigin(grafo, origin, distances);
                break;
            case 2:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origin);
                if (validateInputRange(origin) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
                printf("Ingrese el destino (A-J): ");
                scanf(" %c", &destination);
                if (validateInputRange(destination) == 0) {
        			printf("Error: El origen debe estar en el rango ASCII de 'A' a 'J'\n");
        			break; // Retornar al menú principal
    			}
                showOptimalRoute(grafo, distances, origin, destination);
                break;
            case 3:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
        }

    } while (choice != 3);
}
