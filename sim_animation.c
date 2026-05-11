#include <stdio.h>
#include "raylib.h"
#include <math.h>

#define MAX_NODES 100
#define INF 1000000000
// Animation constants
#define TICK_TIME 0.3f       // Each small jump on a line takes 0.3 seconds
#define NODE_WAIT_TIME 1.0f  // Wait exactly 1 second at each station

// State variables
bool isRunning = false;      // Controls the Play/Stop button
float timer = 0.0f;          // Tracks how much time passed in the current step
int pathIndex = 0;           // Current station in the Dijkstra path -24
int currentStep = 0;         // How many jumps we finished on the current line
bool isWaitingAtNode = false; // True if the entity is currently resting at a station

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


    // the window for the graph
   //Create a list of nodes to visit
    int path[MAX_NODES];
    int pathCount = 0;
    int tempPath[MAX_NODES];
    int tempCount = 0;
    int curr = destination;

    while (curr != -1) {
        tempPath[tempCount++] = curr;
        curr = parent[curr];
    }

    for (int i = 0; i < tempCount; i++) {
        path[i] = tempPath[tempCount - 1 - i];
    }
    pathCount = tempCount;
    InitWindow(800, 600, "Graph Visualizer");
    SetTargetFPS(60);

    // main loop to draw everything
    while (!WindowShouldClose()) {
        // Movement Logic - Simple English: Handle 0.3s steps and 1.0s wait
        if (isRunning && pathCount > 1) {
            timer += GetFrameTime();

            if (isWaitingAtNode) {
                if (timer >= NODE_WAIT_TIME) {
                    isWaitingAtNode = false;
                    timer = 0;
                }
            } else {
                if (timer >= TICK_TIME) {
                    currentStep++;
                    timer = 0;
                    int W = graph[path[pathIndex]][path[pathIndex + 1]];
                    if (currentStep >= W) {
                        pathIndex++;
                        currentStep = 0;
                        if (pathIndex < pathCount - 1) isWaitingAtNode = true;
                        else isRunning = false;
                    }
                }
            }
        }
        BeginDrawing(); // capital B
        //background
        ClearBackground((Color){ 15, 15, 35, 255 }); // Midnight Blue
        // 1. Draw Button - Simple English: Toggle isRunning on click
        Rectangle btn = { 10, 10, 100, 40 };
        DrawRectangleRec(btn, LIGHTGRAY);
        DrawText(isRunning ? "STOP" : "PLAY", 25, 20, 20, BLACK);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
            if (!isRunning) {
                // Reset animation state
                pathIndex = 0;
                currentStep = 0;
                timer = 0.0f;
                isWaitingAtNode = false;
            }

            isRunning = !isRunning;
        }

        // 2. Draw Moving Entity - Simple English: Red circle moving on lines
        if (pathCount > 1 && pathIndex < pathCount - 1) {
            Vector2 p1 = pos[path[pathIndex]];
            Vector2 p2 = pos[path[pathIndex + 1]];
            int W = graph[path[pathIndex]][path[pathIndex + 1]];
            float t = (float)currentStep / W;
            Vector2 entityPos = { p1.x + (p2.x - p1.x) * t, p1.y + (p2.y - p1.y) * t };
            DrawCircleV(entityPos, 12, RED);
        }


        // defining custom pink color
        Color CUSTOM_PINK = (Color){ 255, 105, 180, 255 };

        // draw edges and arrows
        for (int i = 0; i < node; i++) {
            for (int j = 0; j < node; j++) {
                if (graph[i][j] != 0) {
                    // Draw a glow effect first
                    DrawLineEx(pos[i], pos[j], 4.0f, (Color){ 0, 150, 255, 50 });
                    DrawLineEx(pos[i], pos[j], 1.5f, SKYBLUE);
                    // calculate a position at 60% of the line (closer to the destination)
                    // this naturally separates the 2 and the 10
                    float ratio = 0.6f;
                    int labelX = (int)(pos[i].x + (pos[j].x - pos[i].x) * ratio);
                    int labelY = (int)(pos[i].y + (pos[j].y - pos[i].y) * ratio);

                    // adjust slightly so the number doesn't sit exactly ON the line
                   // labelY += 10;

                    // draw the weight

                    const char* weightText = TextFormat("%d", graph[i][j]);

                    DrawText(weightText, labelX - 9, labelY - 10, 22, BLACK);
                    DrawText(weightText, labelX - 7, labelY - 10, 22, WHITE);
                    DrawText(weightText, labelX - 8, labelY - 11, 22, WHITE);


                    DrawText(weightText, labelX - 8, labelY - 10, 22, WHITE);

                    // calculate arrow direction
                    float angle = atan2f(pos[j].y - pos[i].y, pos[j].x - pos[i].x);

                    // arrow tip position (offset from node center)
                    float tipOffset = 30.0f;
                    Vector2 tip = {
                            pos[j].x - tipOffset * cosf(angle),
                            pos[j].y - tipOffset * sinf(angle)
                    };

                    // V-shaped arrow
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
// draw nodes on top of the lines
        for (int i = 0; i < node; i++) {
            Color lavender = (Color){ 180, 120, 220, 255 };

            if (i == source) {
                DrawCircleV(pos[i], 24, lavender);
                DrawCircleV(pos[i], 22, WHITE);
            } else {
                DrawCircleV(pos[i], 24, WHITE);
                DrawCircleV(pos[i], 22, lavender);
            }

            const char* idText = TextFormat("%d", i);
            int fontSize = 25;
            int textWidth = MeasureText(idText, fontSize);
            DrawText(idText, pos[i].x - textWidth / 2, pos[i].y - 12, fontSize, BLACK);

            }
        if (pathIndex == pathCount - 1 && !isWaitingAtNode && !isRunning) {


            DrawRectangle(200, 520, 400, 60, (Color){ 40, 40, 80, 200 });
            DrawRectangleLines(200, 520, 400, 60, WHITE);


            const char* finishText = "DESTINATION REACHED!";
            int textWidth = MeasureText(finishText, 30);
            int startX = 400 - textWidth / 2;

            DrawText(finishText, startX + 2, 537, 30, BLACK);
            DrawText(finishText, startX, 535, 30, WHITE);

        }
        EndDrawing();

    }

    CloseWindow();
    return 0;
}