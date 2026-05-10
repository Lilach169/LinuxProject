#include <stdio.h>
#include "raylib.h"
#include <math.h>

#define MAX_NODES 100
#define INF 1000000000

int main() {
    // open and read the input file
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    int node, edge;
    // get number of nodes and edges
    fscanf(file, "%d%d", &node, &edge);

    int graph[MAX_NODES][MAX_NODES];

    // initialize the matrix with zero
    for (int i = 0; i < node; i++) {
        for (int j = 0; j < node; j++) {
            graph[i][j] = 0;
        }
    }

    int src, dst, weight;
    // load edges and check for negative weights
    for (int i = 0; i < edge; i++) {
        fscanf(file, "%d %d %d", &src, &dst, &weight);
        if (weight < 0) {
            printf("Invalid input: Negative weight detected\n");
            fclose(file);
            return 0;
        }
        graph[src][dst] = weight;
    }

    int source, destination;
    // get the start and end points
    fscanf(file, "%d %d", &source, &destination);
    fclose(file);

    // dijkstra logic to find shortest path
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int parent[MAX_NODES];

    for (int i = 0; i < node; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;

    for (int i = 0; i < node; i++) {
        int u = -1;
        for (int j = 0; j < node; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (u == -1 || dist[u] == INF) {
            break;
        }
        visited[u] = 1;

        for (int v = 0; v < node; v++) {
            if (graph[u][v] != 0 && !visited[v]) {
                if (dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    // set positions for the nodes on screen
    Vector2 pos[MAX_NODES];
    pos[0] = (Vector2){100, 100};
    pos[1] = (Vector2){300, 100};
    pos[2] = (Vector2){200, 250};
    pos[3] = (Vector2){500, 250};
    pos[4] = (Vector2){300, 400};
    pos[5] = (Vector2){600, 400};

    // create the window for the graph
    InitWindow(800, 600, "Graph Visualizer");
    SetTargetFPS(60);

    // main loop to draw everything
    while (!WindowShouldClose()) {
        BeginDrawing(); // capital B
        ClearBackground(RAYWHITE); // capital C and B

        // defining custom pink color
        Color CUSTOM_PINK = (Color){ 255, 105, 180, 255 };

        // draw edges and arrows
        for (int i = 0; i < node; i++) {
            for (int j = 0; j < node; j++) {
                if (graph[i][j] != 0) {
                    // draw connection line
                    DrawLineEx(pos[i], pos[j], 2.5f, DARKGRAY);

                    // calculate midpoint for weight label
                    int midX = (pos[i].x + pos[j].x) / 2;
                    int midY = (pos[i].y + pos[j].y) / 2;

                    // white background behind weight
                    DrawRectangle(midX - 12, midY - 12, 24, 24, RAYWHITE);

                    // draw weight text (only once!)
                    DrawText(TextFormat("%d", graph[i][j]), midX - 6, midY - 10, 20, MAROON);

                    // calculate arrow direction
                    float angle = atan2f(pos[j].y - pos[i].y, pos[j].x - pos[i].x);

                    // arrow tip position (offset from node center)
                    float tipOffset = 30.0f;
                    Vector2 tip = {
                            pos[j].x - tipOffset * cosf(angle),
                            pos[j].y - tipOffset * sinf(angle)
                    };

                    // V-shaped arrow (two lines from the tip)
                    float arrowLength = 15.0f;
                    float arrowAngle = 0.4f;  // angle spread for the V shape

                    Vector2 left = {
                            tip.x - arrowLength * cosf(angle - arrowAngle),
                            tip.y - arrowLength * sinf(angle - arrowAngle)
                    };

                    Vector2 right = {
                            tip.x - arrowLength * cosf(angle + arrowAngle),
                            tip.y - arrowLength * sinf(angle + arrowAngle)
                    };

                    // draw V-shaped arrowhead with two thick lines
                    DrawLineEx(tip, left, 3.0f, MAROON);
                    DrawLineEx(tip, right, 3.0f, MAROON);
                }
            }
        }

        EndDrawing();

    }

    CloseWindow();
    return 0;
}