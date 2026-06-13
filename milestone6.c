#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "raylib.h"

#define MAX_NODES 100
#define MAX_TRAVELERS 20
#define INF 1000000000
#define TICK_TIME 0.3f
#define NODE_WAIT_TIME 1.0f

typedef struct {
    int source;
    int destination;
    int path[MAX_NODES];
    int pathCount;
    int pathIndex;
    int currentStep;
    float timer;
    int waiting;
    
    int waitingForNode;
    int insideNode;
    int finished;
    
    pid_t pid;
    Color color;
} Traveler;

typedef struct {
    pid_t pid;
    int currentNode;
    int nextNode;
    int finished;
} Message;

int readInt(FILE *file, int *value) {
    char token[256];

    while (fscanf(file, "%255s", token) == 1) {
        if (token[0] == '#') {
            fgets(token, sizeof(token), file);
            continue;
        }

        *value = atoi(token);
        return 1;
    }

    return 0;
}

void dijkstra(int node, int graph[MAX_NODES][MAX_NODES],
              int source, int destination,
              int path[MAX_NODES], int *pathCount) {
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

    *pathCount = 0;

    if (dist[destination] == INF) {
        return;
    }

    int temp[MAX_NODES];
    int count = 0;
    int curr = destination;

    while (curr != -1) {
        temp[count++] = curr;
        curr = parent[curr];
    }

    for (int i = count - 1; i >= 0; i--) {
        path[*pathCount] = temp[i];
        (*pathCount)++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int node, edge;

    if (!readInt(file, &node) || !readInt(file, &edge)) {
        printf("Invalid input file\n");
        fclose(file);
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES] = {0};

    for (int i = 0; i < edge; i++) {
        int src, dst, weight;

        if (!readInt(file, &src) || !readInt(file, &dst) || !readInt(file, &weight)) {
            printf("Invalid input file\n");
            fclose(file);
            return 1;
        }

        if (weight < 0) {
            printf("Invalid input: Negative weight detected\n");
            fclose(file);
            return 1;
        }

        graph[src][dst] = weight;
    }

    int travelersCount;

    if (!readInt(file, &travelersCount)) {
        printf("Invalid input file\n");
        fclose(file);
        return 1;
    }

    if (travelersCount <= 0 || travelersCount > MAX_TRAVELERS) {
        printf("Invalid number of travelers\n");
        fclose(file);
        return 1;
    }

    Traveler travelers[MAX_TRAVELERS];
    
    int pipes[MAX_TRAVELERS][2];

    Color colors[MAX_TRAVELERS] = {
        RED, BLUE, GREEN, PINK, PURPLE,
        YELLOW, PINK, SKYBLUE, LIME, MAROON,
        GOLD, VIOLET, BROWN, DARKBLUE, DARKGREEN,
        MAGENTA, BEIGE, DARKPURPLE, DARKGRAY, RAYWHITE
    };

    for (int i = 0; i < travelersCount; i++) {
        if (!readInt(file, &travelers[i].source) ||
            !readInt(file, &travelers[i].destination)) {
            printf("Invalid traveler input\n");
            fclose(file);
            return 1;
        }

        travelers[i].pathCount = 0;
        travelers[i].pathIndex = 0;
        travelers[i].currentStep = 0;
        travelers[i].timer = 0.0f;
        travelers[i].waiting = 0;
        travelers[i].waitingForNode = -1;
        travelers[i].insideNode = 0;
        travelers[i].finished = 0;
        travelers[i].pid = 0;
        travelers[i].color = colors[i];
    }

    fclose(file);

    for (int i = 0; i < travelersCount; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return 1;
        }
    }

    for (int i = 0; i < travelersCount; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork failed\n");
            return 1;
        }

        if (pid == 0) {
            close(pipes[i][0]);

            int childPath[MAX_NODES];
            int childPathCount = 0;

            dijkstra(node, graph,
                     travelers[i].source,
                     travelers[i].destination,
                     childPath,
                     &childPathCount);

            for (int j = 0; j < childPathCount; j++) {
                Message msg;
                msg.pid = getpid();
                msg.currentNode = childPath[j];

                if (j < childPathCount - 1) {
                    msg.nextNode = childPath[j + 1];
                    msg.finished = 0;
                } else {
                    msg.nextNode = -1;
                    msg.finished = 1;
                }
		
		
                write(pipes[i][1], &msg, sizeof(Message));
                sleep(1);
                
             
            }

            close(pipes[i][1]);
            exit(0);
        } else {
            close(pipes[i][1]);
            travelers[i].pid = pid;
            fcntl(pipes[i][0], F_SETFL, O_NONBLOCK);
        }
    }

    Vector2 pos[MAX_NODES];

    for (int i = 0; i < node; i++) {
        float angle = 2.0f * PI * i / node;
        pos[i] = (Vector2){
            400 + 250 * cosf(angle),
            300 + 200 * sinf(angle)
        };
    }

    InitWindow(800, 600, "Milestone 5 - IPC Travelers");
    SetTargetFPS(60);

    int isRunning = 0;
    int nodeOwner[MAX_NODES];
    sem_t nodeLocks[MAX_NODES];

for (int i = 0; i < MAX_NODES; i++) {
    nodeOwner[i] = -1;
    sem_init(&nodeLocks[i], 0, 1);
}

    while (!WindowShouldClose()) {
        Rectangle btn = {10, 10, 100, 40};

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), btn)) {
            isRunning = !isRunning;
        }

        if (isRunning) {
            for (int i = 0; i < travelersCount; i++) {
                Traveler *t = &travelers[i];

                int canReadNextMessage = 1;

                if (t->pathCount >= 2 && !t->finished) {
                    int from = t->path[0];
                    int to = t->path[1];
                    int weight = graph[from][to];

                   int entryStep = weight - 1;
		if (entryStep < 0) entryStep = 0;

		if (weight > 0 && t->currentStep < entryStep) {
                        t->timer += GetFrameTime();

                        if (t->timer >= TICK_TIME) {
                            t->timer = 0.0f;
                            t->currentStep++;
                        }

                        canReadNextMessage = 0;
                    } else {
    int targetNode = to;

   if (nodeOwner[targetNode] == i ||
    (nodeOwner[targetNode] == -1 && sem_trywait(&nodeLocks[targetNode]) == 0)) {
        nodeOwner[targetNode] = i;
        t->waitingForNode = -1;

        if (!t->waiting) {
            t->waiting = 1;
            t->timer = 0.0f;
        }

        t->timer += GetFrameTime();

        if (t->timer < NODE_WAIT_TIME) {
    canReadNextMessage = 0;
} else {
    nodeOwner[targetNode] = -1;
    sem_post(&nodeLocks[targetNode]);

    t->waiting = 0;
    t->timer = 0.0f;
    t->waitingForNode = -1;
}
    } else {
        t->waitingForNode = targetNode;
        canReadNextMessage = 0;
    }
}
                }

                if (canReadNextMessage) {
                    Message msg;
                    ssize_t bytes = read(pipes[i][0], &msg, sizeof(Message));

                    if (bytes == sizeof(Message)) {
                        t->path[0] = msg.currentNode;

                        if (msg.finished) {
                            t->path[1] = msg.currentNode;
                        } else {
                            t->path[1] = msg.nextNode;
                        }

                        t->pathCount = 2;
                        t->pathIndex = 0;
                        t->currentStep = 0;
                        t->timer = 0.0f;
                        t->waiting = 0;
                        t->finished = msg.finished;

                        if (msg.finished) {
                            printf("[PID=%d] arrived at node %d | DESTINATION\n",
                                   msg.pid, msg.currentNode);
                            printf("[PID=%d] finished\n", msg.pid);
                        } else {
                            printf("[PID=%d] arrived at node %d | next node: %d\n",
                                   msg.pid, msg.currentNode, msg.nextNode);
                        }
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){15, 15, 35, 255});

        DrawRectangleRec(btn, LIGHTGRAY);
        DrawText(isRunning ? "STOP" : "PLAY", 25, 20, 20, BLACK);

        for (int i = 0; i < node; i++) {
            for (int j = 0; j < node; j++) {
                if (graph[i][j] != 0) {
                    DrawLineEx(pos[i], pos[j], 2.0f, SKYBLUE);

                    float angle = atan2f(pos[j].y - pos[i].y,
                                          pos[j].x - pos[i].x);

                    Vector2 tip = {
                        pos[j].x - 28 * cosf(angle),
                        pos[j].y - 28 * sinf(angle)
                    };

                    Vector2 left = {
                        tip.x - 14 * cosf(angle - 0.4f),
                        tip.y - 14 * sinf(angle - 0.4f)
                    };

                    Vector2 right = {
                        tip.x - 14 * cosf(angle + 0.4f),
                        tip.y - 14 * sinf(angle + 0.4f)
                    };

                    DrawLineEx(tip, left, 3.0f, MAROON);
                    DrawLineEx(tip, right, 3.0f, MAROON);

                    int labelX = (int)((pos[i].x + pos[j].x) / 2);
                    int labelY = (int)((pos[i].y + pos[j].y) / 2);

                    DrawText(TextFormat("%d", graph[i][j]),
                             labelX,
                             labelY,
                             20,
                             WHITE);
                }
            }
        }

        for (int i = 0; i < node; i++) {
            DrawCircleV(pos[i], 24, WHITE);
            DrawCircleV(pos[i], 21, (Color){180, 120, 220, 255});

            const char *text = TextFormat("%d", i);
            int width = MeasureText(text, 22);

            DrawText(text,
                     pos[i].x - width / 2,
                     pos[i].y - 11,
                     22,
                     BLACK);
    }
    

        for (int i = 0; i < travelersCount; i++) {
            Traveler *t = &travelers[i];

            if (t->pathCount <= 0 || t->finished) {
    		continue;
}

            Vector2 travelerPos;

            if (t->finished || t->pathIndex >= t->pathCount - 1) {
                travelerPos = pos[t->path[t->pathCount - 1]];
            } else {
                int from = t->path[t->pathIndex];
                int to = t->path[t->pathIndex + 1];
                int weight = graph[from][to];

                float move = 0.0f;

                if (weight > 0) {
                    move = (float)t->currentStep / weight;
                }

                if (move > 1.0f) {
                    move = 1.0f;
                }

                travelerPos = (Vector2){
                    pos[from].x + (pos[to].x - pos[from].x) * move,
                    pos[from].y + (pos[to].y - pos[from].y) * move
                };
            }
if (t->waitingForNode != -1) {
    DrawCircleV(travelerPos, 14, t->color);
    DrawCircleLines((int)travelerPos.x,
                    (int)travelerPos.y, 20,ORANGE);
                   
   
} else {
    DrawCircleV(travelerPos, 13, t->color);
}

//DrawText(TextFormat("%d", i + 1),
        // travelerPos.x - 4,
        // travelerPos.y - 8,
//16,
       //  BLACK);
}
        EndDrawing();
    }

    for (int i = 0; i < travelersCount; i++) {
        if (!travelers[i].finished) {
            kill(travelers[i].pid, SIGTERM);
        }

        waitpid(travelers[i].pid, NULL, 0);
    }

    CloseWindow();

    return 0;
}

