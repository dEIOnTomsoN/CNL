#include <stdio.h>
#include <limits.h>

#define MAX 100
#define INF 1000

void dijkstra(int cost[MAX][MAX], int count, int src_router) {
    int dist[MAX], last[MAX], flag[MAX] = {0};
    int i, j, v, min, w;

    for (v = 0; v < count; v++) {
        last[v] = src_router;
        dist[v] = cost[src_router][v];
    }
    flag[src_router] = 1;

    for (i = 0; i < count - 1; i++) {
        min = INF;
        for (w = 0; w < count; w++) {
            if (!flag[w] && dist[w] < min) {
                v = w;
                min = dist[w];
            }
        }
        flag[v] = 1;

        for (w = 0; w < count; w++) {
            if (!flag[w] && (min + cost[v][w]) < dist[w]) {
                dist[w] = min + cost[v][w];
                last[w] = v;
            }
        }
    }

    printf("\nRouting Table (Shortest Paths from Router %d):\n", src_router);
    for (i = 0; i < count; i++) {
        printf("To Router %d: Path %d", i, i);
        w = i;
        while (w != src_router) {
            printf(" <- %d", last[w]);
            w = last[w];
        }
        printf(" | Cost: %d\n", dist[i]);
    }
}

int main() {
    int count, src_router, i, j;
    int cost[MAX][MAX];

    printf("Enter the number of routers: ");
    scanf("%d", &count);

    printf("Enter the cost matrix (-1 for no direct connection):\n");
    for (i = 0; i < count; i++) {
        for (j = 0; j < count; j++) {
            scanf("%d", &cost[i][j]);
            if (cost[i][j] < 0) cost[i][j] = INF;
        }
    }

    printf("Enter the source router: ");
    scanf("%d", &src_router);

    dijkstra(cost, count, src_router);
    return 0;
}

