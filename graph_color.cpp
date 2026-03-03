#include <iostream>
using namespace std;

const int V = 4;

void printSolution(int color[]) {
    cout << "Assigned colors: ";
    for (int i = 0; i < V; i++) {
        cout << color[i] << " ";
    }
    cout << endl;
}

bool isSafe(int v, bool graph[V][V], int color[], int c) {
    for (int i = 0; i < V; i++) {
        if (graph[v][i] && color[i] == c) {
            return false;
        }
    }
    return true;
}

bool graphColoring(bool graph[V][V], int m, int color[], int v) {
    if (v == V) {
        printSolution(color);
        return true;
    }

    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            
            if (graphColoring(graph, m, color, v + 1)) {
                return true;
            }
            
            color[v] = 0;
        }
    }
    return false;
}

int main() {
    bool graph[V][V] = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };

    int m = 3; // Number of colors
    int color[V] = {0};

    if (!graphColoring(graph, m, color, 0)) {
        cout << "No solution exists" << endl;
    }

    return 0;
}