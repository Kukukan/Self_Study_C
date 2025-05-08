#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> read_board(int& n) {
    cin >> n;
    vector<string> board(n);
    for (int i = 0; i < n; ++i) {
        cin >> board[i];
    }
    return board;
}

int backtrack(const vector<string>& board, int n, int row, 
             int cols, int diag1, int diag2) {
    if (row == n) return 1;
    
    int count = 0;
    for (int col = 0; col < n; ++col) {
        if (board[row][col] == '.') {
            int col_mask = 1 << col;
            int d1 = row - col + (n - 1);
            int d2 = row + col;
            
            if (!(cols & col_mask) && 
                !(diag1 & (1 << d1)) && 
                !(diag2 & (1 << d2))) {
                
                count += backtrack(board, n, row + 1,
                                  cols | col_mask,
                                  diag1 | (1 << d1),
                                  diag2 | (1 << d2));
            }
        }
    }
    return count;
}

int count_queen_arrangements(const vector<string>& board, int n) {
    return backtrack(board, n, 0, 0, 0, 0);
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n;
    vector<string> board = read_board(n);
    cout << count_queen_arrangements(board, n) << endl;
    return 0;
}