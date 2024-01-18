#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_LOCATIONS 10

typedef struct {
    int distance;
    int hours; // Como tenemos la conversión realmente no es necesario guardarlo, solo es para cambios futuros.
} Route;


// Inicializar matriz de grafos
void initializeGraph(Route graph[MAX_LOCATIONS][MAX_LOCATIONS]) {
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph[i][j].distance = INT_MAX;
            graph[i][j].hours = INT_MAX;
        }
    }
}

// Cargar rutas del archivo
void loadRoutes(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char line[100];
    char origin, destination;
    int distance, hours;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, " %c | %c | %d | %d hora", &origin, &destination, &distance, &hours) == 4) {
            int originIndex = origin - 'A';
            int destinationIndex = destination - 'A';

            graph[originIndex][destinationIndex].distance = distance;
            graph[originIndex][destinationIndex].hours = hours;
        }
    }

    fclose(file);
}

// Función dijkstra solicitada:
void dijkstra(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], int source, int distances[MAX_LOCATIONS]) {
    bool visited[MAX_LOCATIONS] = {false};

    distances[source] = 0;

    for (int count = 0; count < MAX_LOCATIONS - 1; count++) {
        int minDistance = INT_MAX, minIndex;

        for (int v = 0; v < MAX_LOCATIONS; v++) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        visited[minIndex] = true;

        for (int v = 0; v < MAX_LOCATIONS; v++) {
            if (!visited[v] && graph[minIndex][v].distance != INT_MAX &&
                distances[minIndex] != INT_MAX &&
                distances[minIndex] + graph[minIndex][v].distance < distances[v]) {
                distances[v] = distances[minIndex] + graph[minIndex][v].distance;
            }
        }
    }
}

void showAllRoutesToDestination(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], char origin, char destination) {
 
 	// Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
    int originIndex = origin - 'A';
    int destIndex = destination - 'A';
	
    if (graph[originIndex][destIndex].distance == INT_MAX) {
        printf("\nNo hay forma de llegar al menos de manera directa desde %c hasta %c\n", origin, destination);
        // el return acá hacía que no estrase en el bucle for para el caso de rutas indirectas.
    }

    printf("\nRutas desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\n"); 

    if (graph[originIndex][destIndex].distance != 0) {
        // Mostrar ruta directa
        
        if (graph[originIndex][destIndex].distance != 2147483647) {	
        	printf("Directo: %d mts\t%.2f horas\n", graph[originIndex][destIndex].distance, (float)graph[originIndex][destIndex].distance / 100);
		}
		
		else {
			printf("No es posible hacer tal recorrido de manera directa\n");
		}
		
        // Mostrar rutas intermedias con lógica del recorrido de la matriz
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            if (i != originIndex && i != destIndex &&
                graph[originIndex][i].distance != INT_MAX &&
                graph[i][destIndex].distance != INT_MAX) {
                printf("Via %c: %d mts\t%.2f horas\n", i + 'A',
                       (graph[originIndex][i].distance + graph[i][destIndex].distance),
                       (float)(graph[originIndex][i].distance + graph[i][destIndex].distance) / 100);
            }
        }
    }
}

void showAllRoutesFromOrigin(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], char origin) {
    
	// Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int originIndex = origin - 'A';

    printf("\nTodas las rutas desde %c:\n", origin);

    for (int destIndex = 0; destIndex < MAX_LOCATIONS; destIndex++) { // Buscar hasta que se acaben las posibles opciones
        if (destIndex != originIndex) { // Validación para que un nodo no se busque consigo mismo
            showAllRoutesToDestination(graph, origin, destIndex + 'A'); // Ir alternando constantemente el nodo de destino
        }
    }
}

void showOptimalRoute(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], int distances[MAX_LOCATIONS], char origin, char destination) {
    
    // Inicialización ASCII, se asume que el destino y el origen esta en un rango A-Z
	int originIndex = origin - 'A';
    int destIndex = destination - 'A';
	
	/* Nota del programador: Se asume que en este caso el valor se desproporciono por lo tanto es INT_MAX, si eso sucede no hay 
	manera posible de llegar al destino */
	
    if (distances[destIndex] == INT_MAX) {
        printf("No hay forma de llegar desde %c hasta %c\n", origin, destination);
        return;
    }

    printf("\nRuta más óptima desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\n");

	// Se hace la conversión según las condiciones dadas
    printf("%d\t\t%.2f horas\n", distances[destIndex], (float)distances[destIndex] / 100);
}

void showMenu(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], int distances[MAX_LOCATIONS]) {
    int choice;
    char origin, destination;

    do {
        printf("\nMenu:\n");
        printf("1. Mostrar todos los destinos dado un origen\n");
        printf("2. Mostrar el recorrido más óptimo de un origen a un destino\n");
        printf("3. Salir\n");

        printf("Ingrese su elección: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origin);
                printf("Destinos desde %c:\n", origin);
                showAllRoutesFromOrigin(graph, origin);
                break;
            case 2:
                printf("Ingrese el origen (A-J): ");
                scanf(" %c", &origin);
                printf("Ingrese el destino (A-J): ");
                scanf(" %c", &destination);
                showOptimalRoute(graph, distances, origin, destination);
                break;
            case 3:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }

    } while (choice != 3);
}

/* NOTA: El Programa solo puede calcular todas las rutas indirectas entre un destino y un origen si en algún momento ese origen 
estuvo relacionado directamente con el destino. Un ejemplo: Si A->D luego D->E, La ruta indirectacon D solo se calcula si 
previamente A->E

En tal caso (si tuvo la relación directa) solo es capaz de calcular de manera correcta la distancia (en el caso de que sea 
indirecta sin tener relación directa), de ese Nodo a su Nodo destino (Ha de ser una falla relativamente fácil). */

/* Ya el bug fue parcheado */

int main() {
    Route graph[MAX_LOCATIONS][MAX_LOCATIONS];
    initializeGraph(graph);
    
    /* Notas del programador:
    printf("Tenga en cuenta que el valor 2147483647 indica que no hay forma de realizar tal recorrido (distancias)\n");
    printf("Tenga en cuenta que el valor 21474836.00 indica que no hay forma de realizar tal recorrido (horas)\n"); */

    loadRoutes(graph, "prueba.txt");

    int sourceNode = 0;
    int distances[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        distances[i] = INT_MAX;
    }

    dijkstra(graph, sourceNode, distances);

    showMenu(graph, distances);

    return 0;
}
