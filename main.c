#include <stdio.h>
#include "raylib.h"

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

    int graph[MAX_NODES][MAX_NODES];

    for(int i = 0; i < node; i++){
        for(int j = 0; j < node; j++){
            graph[i][j] = 0;
        }
    }

    int src, dst, weight;
    for(int i = 0; i < edge; i++){
        fscanf(file, "%d %d %d", &src, &dst, &weight);
        if (weight < 0) {
            printf("Invalid input: Negative weight detected\n");
            fclose(file);
            return 0;
        }
        graph[src][dst] = weight;
    }

    int source, destination;
    fscanf(file, "%d %d", &source, &destination );


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
        if(u == -1 || dist[u] == INF) {
            break;
        }
        visited[u] = 1;

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

    Vector2 pos[MAX_NODES];

    pos[0] = (100,100);
    pos[1] = (300,100);
    pos[2] = (200,250);
    pos[3] = (500,250);
    pos[4] = (300,400);
    pos[5] = (600,400);

    InitWindow(800, 600, "Graph");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

    return 0;
}
