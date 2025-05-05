#include <iostream>
#include <vector>

using namespace std;

int n;
vector<string> board;
vector<bool> colUsed, diag1Used, diag2Used;
int solutions = 0;

void solve(int row) {
    if (row == n) {
        solutions++;
        return;
    }
    
    for (int col = 0; col < n; col++) {
        if (board[row][col] == '*') continue; // Skip forbidden squares
        if (colUsed[col] || diag1Used[row - col + n - 1] || diag2Used[row + col]) continue;

        // Place queen
        colUsed[col] = diag1Used[row - col + n - 1] = diag2Used[row + col] = true;
        
        solve(row + 1);
        
        // Remove queen (backtracking)
        colUsed[col] = diag1Used[row - col + n - 1] = diag2Used[row + col] = false;
    }
}

int main() {
    cin >> n;
    board.resize(n);
    colUsed.assign(n, false);
    diag1Used.assign(2 * n, false);
    diag2Used.assign(2 * n, false);

    for (int i = 0; i < n; i++) {
        cin >> board[i];
    }

    solve(0);
    cout << solutions << endl;

    return 0;
}