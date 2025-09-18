#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> left_move(const vector<vector<int>>& board) {
    int n = board.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        vector<int> non_zeros;
        for (int j = 0; j < n; j++) {
            if (board[i][j] != 0) {
                non_zeros.push_back(board[i][j]);
            }
        }
        vector<int> combined;
        int j = 0;
        while (j < non_zeros.size()) {
            if (j + 1 < non_zeros.size() && non_zeros[j] == non_zeros[j+1]) {
                combined.push_back(non_zeros[j] * 2);
                j += 2;
            } else {
                combined.push_back(non_zeros[j]);
                j++;
            }
        }
        for (int k = 0; k < combined.size(); k++) {
            res[i][k] = combined[k];
        }
    }
    return res;
}

vector<vector<int>> right_move(const vector<vector<int>>& board) {
    int n = board.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        vector<int> non_zeros;
        for (int j = n-1; j >= 0; j--) {
            if (board[i][j] != 0) {
                non_zeros.push_back(board[i][j]);
            }
        }
        vector<int> combined;
        int j = 0;
        while (j < non_zeros.size()) {
            if (j + 1 < non_zeros.size() && non_zeros[j] == non_zeros[j+1]) {
                combined.push_back(non_zeros[j] * 2);
                j += 2;
            } else {
                combined.push_back(non_zeros[j]);
                j++;
            }
        }
        for (int k = 0; k < combined.size(); k++) {
            res[i][n-1-k] = combined[k];
        }
    }
    return res;
}

vector<vector<int>> up_move(const vector<vector<int>>& board) {
    int n = board.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    for (int j = 0; j < n; j++) {
        vector<int> non_zeros;
        for (int i = 0; i < n; i++) {
            if (board[i][j] != 0) {
                non_zeros.push_back(board[i][j]);
            }
        }
        vector<int> combined;
        int i = 0;
        while (i < non_zeros.size()) {
            if (i + 1 < non_zeros.size() && non_zeros[i] == non_zeros[i+1]) {
                combined.push_back(non_zeros[i] * 2);
                i += 2;
            } else {
                combined.push_back(non_zeros[i]);
                i++;
            }
        }
        for (int k = 0; k < combined.size(); k++) {
            res[k][j] = combined[k];
        }
    }
    return res;
}

vector<vector<int>> down_move(const vector<vector<int>>& board) {
    int n = board.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    for (int j = 0; j < n; j++) {
        vector<int> non_zeros;
        for (int i = n-1; i >= 0; i--) {
            if (board[i][j] != 0) {
                non_zeros.push_back(board[i][j]);
            }
        }
        vector<int> combined;
        int i = 0;
        while (i < non_zeros.size()) {
            if (i + 1 < non_zeros.size() && non_zeros[i] == non_zeros[i+1]) {
                combined.push_back(non_zeros[i] * 2);
                i += 2;
            } else {
                combined.push_back(non_zeros[i]);
                i++;
            }
        }
        for (int k = 0; k < combined.size(); k++) {
            res[n-1-k][j] = combined[k];
        }
    }
    return res;
}

vector<vector<int>> apply_move(const vector<vector<int>>& board, char move) {
    switch (move) {
        case 'L': return left_move(board);
        case 'R': return right_move(board);
        case 'U': return up_move(board);
        case 'D': return down_move(board);
    }
    return board;
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<vector<int>> board(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            string s;
            cin >> s;
            if (s == ".") {
                board[i][j] = 0;
            } else {
                board[i][j] = stoi(s);
            }
        }
    }

    vector<char> moves = {'L', 'R', 'U', 'D'};
    int best_value = 0;
    int best_count = 0;

    for (int i1 = 0; i1 < 4; i1++) {
        vector<vector<int>> b1 = apply_move(board, moves[i1]);
        for (int i2 = 0; i2 < 4; i2++) {
            vector<vector<int>> b2 = apply_move(b1, moves[i2]);
            for (int i3 = 0; i3 < 4; i3++) {
                vector<vector<int>> b3 = apply_move(b2, moves[i3]);
                for (int i4 = 0; i4 < 4; i4++) {
                    vector<vector<int>> b4 = apply_move(b3, moves[i4]);
                    for (int i5 = 0; i5 < 4; i5++) {
                        vector<vector<int>> b5 = apply_move(b4, moves[i5]);
                        int max_val = 0;
                        int count = 0;
                        for (int i = 0; i < n; i++) {
                            for (int j = 0; j < n; j++) {
                                if (b5[i][j] > max_val) {
                                    max_val = b5[i][j];
                                    count = 1;
                                } else if (b5[i][j] == max_val) {
                                    count++;
                                }
                            }
                        }
                        if (max_val > best_value) {
                            best_value = max_val;
                            best_count = count;
                        } else if (max_val == best_value) {
                            if (count > best_count) {
                                best_count = count;
                            }
                        }
                    }
                }
            }
        }
    }

    cout << best_value << " " << best_count << endl;
    return 0;
}