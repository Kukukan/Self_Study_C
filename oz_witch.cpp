#include <bits/stdc++.h>
using namespace std;

const int MAXN = 16;
vector<int> types;  // 1 for good, 2 for evil
vector<vector<bool>> dislikes;
vector<bool> used;
vector<int> arrangement;
int N, M, K;

int total = 0;

// Check if adding person at position pos creates valid arrangement
bool isValid(int pos, int person) {
    // Check dislike pairs with previous person
    if (pos > 0 && dislikes[arrangement[pos-1]][person]) return false;
    // For first person, check with last position
    if (pos == 0 && arrangement.size() > 1 && dislikes[arrangement.back()][person]) return false;

    // Check consecutive witches constraint
    int count = 1;
    int currentType = types[person];
    
    // Look backward
    for (int i = pos-1; i >= 0 && types[arrangement[i]] == currentType; i--) count++;
    // For first positions, check from end of array
    if (pos == 0) {
        for (int i = arrangement.size()-1; i >= 0 && types[arrangement[i]] == currentType; i--) count++;
    }
    if (count > K) return false;
    
    // For last position, also check forward to first positions
    if (pos == N-1) {
        count = 1;
        for (int i = 0; i < arrangement.size()-1 && types[arrangement[i]] == currentType; i++) count++;
        if (count > K) return false;
    }
    
    return true;
}

void solve(int pos) {
    if (pos == N) {
        // Check if first and last witches can sit together
        if (!dislikes[arrangement[0]][arrangement[N-1]]) {
            // Check final consecutive count wrapping around
            int count = 1;
            int type = types[arrangement[0]];
            for (int i = N-1; i >= 0 && types[arrangement[i]] == type; i--) count++;
            for (int i = 1; i < N && types[arrangement[i]] == type; i++) count++;
            if (count <= K) total++;
        }
        return;
    }
    
    for (int i = 1; i < N; i++) {  // Start from 1 since 0 is fixed at start
        if (!used[i]) {
            if (isValid(pos, i)) {
                used[i] = true;
                arrangement[pos] = i;
                solve(pos + 1);
                used[i] = false;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> K;
    
    // Initialize vectors
    types.resize(N);
    dislikes.resize(N, vector<bool>(N, false));
    used.resize(N, false);
    arrangement.resize(N);
    
    // Read witch types
    for (int i = 0; i < N; i++) {
        cin >> types[i];
    }
    
    // Read dislike pairs
    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;  // Convert to 0-based indexing
        dislikes[a][b] = dislikes[b][a] = true;
    }
    
    // Fix first witch at position 0 to handle rotations
    used[0] = true;
    arrangement[0] = 0;
    solve(1);
    
    cout << total << "\n";
    return 0;
}