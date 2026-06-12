#include <stdio.h>

#define MAX_NODES 100
#define INF 1000000000

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int node, edge;
    if (fscanf(file, "%d%d", &node, &edge) != 2)
    {
        printf("Error opening file\n");
        fclose(file);
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES];

    for (int i = 0; i < node; i++) {
        for (int j = 0; j < node; j++) {
            graph[i][j] = 0;
        }
    }

    int src, dst, weight;

    for (int i = 0; i < edge; i++) {
        if (fscanf(file, "%d %d %d", &src, &dst, &weight) != 3)
        {
            printf("Invalid input file\n");
            fclose(file);
            return 1;
        }
        graph[src][dst] = weight;
    }

    int source, destination;
    if (fscanf(file, "%d %d", &source, &destination) != 2)
    {
        printf("Invalid input file\n");
        fclose(file);
        return 1;
    }

    fclose(file);

    int dist[MAX_NODES];
    int visited[MAX_NODES];

    for (int i = 0; i < node; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[source] = 0;

    for (int i = 0; i < node; i++) {
        int u = -1;

        for (int j = 0; j < node; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < node; v++) {
            if (graph[u][v] != 0 && !visited[v]) {
                if (dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    printf("Shortest distance from %d to %d is %d\n",
           source, destination, dist[destination]);

    return 0;
}
