#include <stdio.h>

#define MAX 20
#define INF 1000

struct Router {
    int distance[MAX];
    int adjNodes[MAX];
} node[MAX];

int costMatrix[MAX][MAX], n;

void readCostMatrix() {
    int i, j;
    printf("Enter cost matrix (-1 for no direct connection):\n");
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            scanf("%d", &costMatrix[i][j]);
            if (costMatrix[i][j] < 0) costMatrix[i][j] = INF;
            node[i].distance[j] = costMatrix[i][j];
            node[i].adjNodes[j] = j;
        }
    }
}

void calcRoutingTable() {
    int i, j, k;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            for (k = 0; k < n; ++k) {
                if (node[i].distance[j] > costMatrix[i][k] + node[k].distance[j]) {
                    node[i].distance[j] = node[i].distance[k] + node[k].distance[j];
                    node[i].adjNodes[j] = k;
                }
            }
        }
    }
}

void displayRoutes() {
    int i, j;
    printf("\nRouting Tables:\n");
    for (i = 0; i < n; ++i) {
        printf("Router %d:\n", i + 1);
        for (j = 0; j < n; ++j) {
            if (node[i].distance[j] == INF)
                printf("Node %d via - : No Path\n", j + 1);
            else
                printf("Node %d via %d : Distance %d\n", j + 1, node[i].adjNodes[j] + 1, node[i].distance[j]);
        }
        printf("\n");
    }
}

int main() {
    printf("Enter number of routers: ");
    scanf("%d", &n);

    readCostMatrix();
    calcRoutingTable();
    displayRoutes();

    return 0;
}

