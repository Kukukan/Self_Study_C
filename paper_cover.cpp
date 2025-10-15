#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<vector<int>> grid(10, vector<int>(10));
vector<int> papers = {0, 5, 5, 5, 5, 5};
int minPapers = INT_MAX;

bool canPlace(int r, int c, int size) {
    if (r + size > 10 || c + size > 10) return false;
    
    for (int i = r; i < r + size; i++) {
        for (int j = c; j < c + size; j++) {
            if (grid[i][j] != 1) return false;
        }
    }
    return true;
}

void placePaper(int r, int c, int size, int value) {
    for (int i = r; i < r + size; i++) {
        for (int j = c; j < c + size; j++) {
            grid[i][j] = value;
        }
    }
}

bool allCovered() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 1) return false;
        }
    }
    return true;
}

void backtrack(int r, int c, int usedPapers) {
    // Prune if we're already worse than best solution
    if (usedPapers >= minPapers) return;
    
    // If we've processed all cells, check if all 1s are covered
    if (r == 10) {
        if (allCovered()) {
            minPapers = min(minPapers, usedPapers);
        }
        return;
    }
    
    // Calculate next position
    int nextR = r, nextC = c + 1;
    if (nextC == 10) {
        nextR++;
        nextC = 0;
    }
    
    // If current cell is 0, move to next
    if (grid[r][c] == 0) {
        backtrack(nextR, nextC, usedPapers);
        return;
    }
    
    // Try placing papers of all sizes (largest first for better pruning)
    for (int size = 5; size >= 1; size--) {
        if (papers[size] > 0 && canPlace(r, c, size)) {
            papers[size]--;
            placePaper(r, c, size, 0);
            
            backtrack(nextR, nextC, usedPapers + 1);
            
            placePaper(r, c, size, 1);
            papers[size]++;
        }
    }
}

int main() {
    // Read input
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cin >> grid[i][j];
        }
    }
    
    backtrack(0, 0, 0);
    
    if (minPapers == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << minPapers << endl;
    }
    
    return 0;
}