#include <stdio.h>

#define MAX_NODES 100
#define INF 1000000000

int main() {
    FILE *file = fopen("input.txt", "r");

    if(file == NULL){
        printf("Error opening file\n");
        return 0;
    }

    int node, edge;

    fscanf(file, "%d%d", &node, &edge);

    printf("Nodes: %d\n", node);
    printf("Edges: %d\n", edge);

    int graph[MAX_NODES][MAX_NODES];

    for(int i = 0; i < node; i++){
        for(int j = 0; j < node; j++){
            graph[i][j] = 0;
        }
    }

    int src, dst, weight;
    for(int i = 0; i < edge; i++){
        fscanf(file, "%d %d %d", &src, &dst, &weight);
        graph[src][dst] = weight;
    }

    int source, destination;
    fscanf(file, "%d %d", &source, &destination );

    printf("\nAdjacency Matrix:\n");
    for(int i = 0; i < node; i++){
        for(int j = 0; j < node; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }


    fclose(file);

    if(source == destination){
        printf("%d\n%d\n", source, 0);
        return 0;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int parent[MAX_NODES];

    for(int i = 0; i< node; i++){
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;

    for(int i = 0; i < node; i++){
        int u = -1;
        for(int j = 0; j < node; j++){
            if(!visited[j]&& (u == -1 || dist[j] < dist[u])){
                u = j;
            }
        }

        if(u == -1){
            break;
        }

        for(int v = 0; v < node; v++){
            if(graph[u][v] != 0 && !visited[v]){
                if (dist[u] + graph[u][v] < dist[v]){
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    if(dist[destination] == INF){
        printf("No path found\n");
        return 0;
    }

    int path[MAX_NODES];
    int count = 0;

    int cur = destination;
    while (cur!= -1){
        path[count++] = cur;
        cur = parent [cur];
    }

    for(int i = count - 1; i >= 0; i--){
        printf("%d", path[i]);
        if(i != 0){
            printf(" -> ");
        }
    }
    printf("\n");
    printf("%d\n", dist[destination]);


    return 0;
}
