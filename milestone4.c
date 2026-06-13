#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_NODES 100
#define INF 1000000000

#define TICK_TIME 0.3f
#define NODE_WAIT_TIME 1.0f

bool isRunning = false;
float timer = 0.0f;
int pathIndex = 0;
int currentStep = 0;
bool isWaitingAtNode = false;

int main(int argc, char *argv[])
{
    // =========================
    // ARGUMENT CHECK
    // =========================
    if (argc < 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }

    int node, edge;
    fscanf(file, "%d%d", &node, &edge);

    int graph[MAX_NODES][MAX_NODES];

    for (int i = 0; i < node; i++)
        for (int j = 0; j < node; j++)
            graph[i][j] = 0;

    int src, dst, weight;

    for (int i = 0; i < edge; i++)
    {
        fscanf(file, "%d %d %d", &src, &dst, &weight);

        if (weight < 0)
        {
            printf("Invalid input: Negative weight detected\n");
            fclose(file);
            return 0;
        }

        graph[src][dst] = weight;
    }

    int travelers;
    fscanf(file, "%d", &travelers);

    if (travelers <= 0)
    {
        printf("No travelers\n");
        fclose(file);
        return 1;
    }

    int sources[MAX_NODES];
    int destinations[MAX_NODES];

    for (int i = 0; i < travelers; i++)
    {
        fscanf(file, "%d %d", &sources[i], &destinations[i]);
    }

    fclose(file);

    // =========================
    // FORK PROCESS (MILESTONE 4)
    // =========================
    pid_t pids[MAX_NODES];

    for (int i = 0; i < travelers; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("Fork failed\n");
            return 1;
        }

        if (pid == 0)
        {
            // child process
            while (1)
            {
                pause();
            }
            exit(0);
        }
        else
        {
            pids[i] = pid;
        }
    }

    // =========================
    // DIJKSTRA
    // =========================
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int parent[MAX_NODES];

    for (int i = 0; i < node; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    // FIX: support multiple travelers (not only [0])
    for (int i = 0; i < travelers; i++)
        dist[sources[i]] = 0;

    for (int i = 0; i < node; i++)
    {
        int u = -1;

        for (int j = 0; j < node; j++)
        {
            if (!visited[j] && (u == -1 || dist[j] < dist[u]))
                u = j;
        }

        if (u == -1 || dist[u] == INF)
            break;

        visited[u] = 1;

        for (int v = 0; v < node; v++)
        {
            if (graph[u][v] && !visited[v])
            {
                if (dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    // =========================
    // GRAPH POSITIONS
    // =========================
    Vector2 pos[MAX_NODES];

    pos[0] = (Vector2){100, 100};
    pos[1] = (Vector2){300, 100};
    pos[2] = (Vector2){200, 250};
    pos[3] = (Vector2){500, 250};
    pos[4] = (Vector2){300, 400};
    pos[5] = (Vector2){600, 400};

    // =========================
    // PATH BUILD
    // =========================
    int path[MAX_NODES];
    int pathCount = 0;
    int tempPath[MAX_NODES];
    int tempCount = 0;

    int curr = destinations[0];

    while (curr != -1)
    {
        tempPath[tempCount++] = curr;
        curr = parent[curr];
    }

    for (int i = 0; i < tempCount; i++)
        path[i] = tempPath[tempCount - 1 - i];

    pathCount = tempCount;

    // =========================
    // RAYLIB
    // =========================
    InitWindow(800, 600, "Graph Visualizer");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (isRunning && pathCount > 1)
        {
            timer += GetFrameTime();

            if (isWaitingAtNode)
            {
                if (timer >= NODE_WAIT_TIME)
                {
                    isWaitingAtNode = false;
                    timer = 0;
                }
            }
            else
            {
                if (timer >= TICK_TIME)
                {
                    currentStep++;
                    timer = 0;

                    int W = graph[path[pathIndex]][path[pathIndex + 1]];

                    if (currentStep >= W)
                    {
                        pathIndex++;
                        currentStep = 0;

                        if (pathIndex < pathCount - 1)
                            isWaitingAtNode = true;
                        else
                            isRunning = false;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){15, 15, 35, 255});

        Rectangle btn = {10, 10, 100, 40};

        DrawRectangleRec(btn, LIGHTGRAY);
        DrawText(isRunning ? "STOP" : "PLAY", 25, 20, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), btn))
        {
            if (!isRunning)
            {
                pathIndex = 0;
                currentStep = 0;
                timer = 0;
                isWaitingAtNode = false;
            }
            isRunning = !isRunning;
        }

        if (pathCount > 1 && pathIndex < pathCount - 1)
        {
            Vector2 p1 = pos[path[pathIndex]];
            Vector2 p2 = pos[path[pathIndex + 1]];
            int W = graph[path[pathIndex]][path[pathIndex + 1]];

            float t = (float)currentStep / W;

            Vector2 entityPos =
                {p1.x + (p2.x - p1.x) * t,
                 p1.y + (p2.y - p1.y) * t};

            DrawCircleV(entityPos, 12, RED);
        }

        // DRAW NODES
        for (int i = 0; i < node; i++)
        {
            Color c = (i == sources[0]) ? SKYBLUE : WHITE;
            DrawCircleV(pos[i], 24, c);
        }

        EndDrawing();
    }

    // =========================
    // WAIT CHILDREN CLEAN EXIT
    // =========================
    for (int i = 0; i < travelers; i++)
        waitpid(pids[i], NULL, 0);

    CloseWindow();
    return 0;
}
