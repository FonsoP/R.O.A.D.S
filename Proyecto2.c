#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_LOCATIONS 10

typedef struct {
    int distance;
    int hours;
    // Otros campos como el costo si es necesario
} Route;

void initializeGraph(Route graph[MAX_LOCATIONS][MAX_LOCATIONS]) {
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph[i][j].distance = INT_MAX;
            graph[i][j].hours = INT_MAX;
        }
    }
}

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
    int originIndex = origin - 'A';
    int destIndex = destination - 'A';

    if (graph[originIndex][destIndex].distance == INT_MAX) {
        printf("No hay forma de llegar desde %c hasta %c\n", origin, destination);
        return;
    }

    printf("Rutas desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\n"); 

    if (graph[originIndex][destIndex].distance == 0) {
        printf("0\t\t%.2f horas\n", (float)graph[originIndex][destIndex].distance / 100); // Convertir metros a horas
    } else {
        // Mostrar ruta directa
        printf("Directo: %d mts\t%.2f horas\n", graph[originIndex][destIndex].distance, (float)graph[originIndex][destIndex].distance / 100);

        // Mostrar rutas intermedias
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
    int originIndex = origin - 'A';

    printf("Todas las rutas desde %c:\n", origin);
    printf("Destino\t\tDistancia\tTiempo (en horas)\n");

    for (int destIndex = 0; destIndex < MAX_LOCATIONS; destIndex++) {
        if (destIndex != originIndex) {
            showAllRoutesToDestination(graph, origin, destIndex + 'A');
        }
    }
}

void showOptimalRoute(Route graph[MAX_LOCATIONS][MAX_LOCATIONS], int distances[MAX_LOCATIONS], char origin, char destination) {
    int originIndex = origin - 'A';
    int destIndex = destination - 'A';

    if (distances[destIndex] == INT_MAX) {
        printf("No hay forma de llegar desde %c hasta %c\n", origin, destination);
        return;
    }

    printf("Ruta más óptima desde %c hasta %c:\n", origin, destination);
    printf("Distancia\tTiempo (en horas)\n");

    printf("%d\t\t%.2f horas\n", distances[destIndex], (float)graph[originIndex][destIndex].distance / 100);
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

// main de prueba

/* int main() {
    Route graph[MAX_LOCATIONS][MAX_LOCATIONS];
    initializeGraph(graph);
    printf("Tenga en cuenta que el valor 2147483647, indica que no hay forma de realizar tal recorrido\n");

    loadRoutes(graph, "prueba.txt");

    int sourceNode = 0;
    int distances[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        distances[i] = INT_MAX;
    }

    dijkstra(graph, sourceNode, distances);

	
    printf("Distancias mínimas desde el nodo %c:\n", sourceNode + 'A');
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        printf("Hasta el nodo %c: %d horas\n", i + 'A', graph[sourceNode][i].hours);
    }
    

    char originNode = 'A';
    char destinationNode = 'E';

    showAllRoutesFromOrigin(graph, originNode);
    showOptimalRoute(graph, distances, originNode, destinationNode);

    return 0;
} */

/* NOTA: El Programa solo puede calcular todas las rutas indirectas entre un destino y un origen si en algún momento ese origen 
estuvo relacionado directamente con el destino.

En tal caso (si tuvo la relación directa) solo es capaz de calcular de manera correcta la distancia (en el caso de que sea 
indirecta sin tener relación directa), de ese Nodo a su Nodo destino (Ha de ser una falla relativamente fácil). */

int main() {
    Route graph[MAX_LOCATIONS][MAX_LOCATIONS];
    initializeGraph(graph);
    printf("Tenga en cuenta que el valor 2147483647 indica que no hay forma de realizar tal recorrido\n");

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
