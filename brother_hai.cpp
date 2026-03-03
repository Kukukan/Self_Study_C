#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000000; // sentinel for never-blocked
const int MAX = 300;

int M, N, sx, sy;
vector<string> grid;
vector<vector<int>> blockTime;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

bool isValid(int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N && grid[x][y] != '#';
}

void computeBlockTimes() {
    blockTime.assign(M, vector<int>(N, INF));
    queue<pair<int, int>> q;
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            // accept both lowercase and uppercase 'p' / 'P'
            if (grid[i][j] == 'p' || grid[i][j] == 'P') {
                blockTime[i][j] = 0;
                q.push({i, j});
            }
        }
    }
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if (isValid(nx, ny) && blockTime[nx][ny] > blockTime[x][y] + 1) {
                blockTime[nx][ny] = blockTime[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
}

bool canEscape(long long waitTime) {
    // use long long for times to avoid narrowing/overflow
    const long long INFLL = (long long)INF + 5;
    vector<vector<long long>> dist(M, vector<long long>(N, INFLL));
    queue<pair<int, int>> q;
    
    // if the starting cell is already or will be blocked at or before waitTime,
    // Brother Hai cannot remain there for waitTime minutes
    if (blockTime[sx][sy] <= waitTime) {
        return false;
    }
    
    dist[sx][sy] = waitTime;
    q.push({sx, sy});
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == M-1 && y == N-1) {
            return true;
        }
        
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if (!isValid(nx, ny)) continue;
            
            long long nextTime = dist[x][y] + 1;
            // can move into (nx,ny) only if arrival time is strictly before
            // the blockade time at that cell
            if (nextTime < (long long)blockTime[nx][ny] && dist[nx][ny] > nextTime) {
                dist[nx][ny] = nextTime;
                q.push({nx, ny});
            }
        }
    }
    
    return false;
}

int main() {
    cin >> M >> N;
    cin >> sx >> sy;
    
    grid.resize(M);
    for (int i = 0; i < M; i++) {
        cin >> grid[i];
    }
    
    computeBlockTimes();

    // If there exists a path from start to destination that uses only
    // cells that will never be blocked (blockTime == INF), then Brother Hai
    // can wait arbitrarily long and still escape. In that case output LIMIT.
    {
        const long long LIMIT = 1000000000LL;
        vector<vector<int>> vis(M, vector<int>(N, 0));
        queue<pair<int,int>> q2;
        if (blockTime[sx][sy] == INF) {
            vis[sx][sy] = 1;
            q2.push({sx, sy});
        }
        bool reachable_never_blocked = false;
        while (!q2.empty()) {
            auto [x, y] = q2.front(); q2.pop();
            if (x == M-1 && y == N-1) { reachable_never_blocked = true; break; }
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!isValid(nx, ny)) continue;
                if (vis[nx][ny]) continue;
                if (blockTime[nx][ny] != INF) continue;
                vis[nx][ny] = 1;
                q2.push({nx, ny});
            }
        }
        if (reachable_never_blocked) {
            cout << LIMIT << '\n';
            return 0;
        }
    }

    if (!canEscape(0)) {
        cout << -1 << '\n';
        return 0;
    }

    // problem statement: if Brother Hai can wait indefinitely and still escape,
    // output 1e9. So constrain binary-search upper bound to 1e9.
    const long long LIMIT = 1000000000LL;
    long long left = 0, right = LIMIT;
    long long answer = 0;

    while (left <= right) {
        long long mid = left + (right - left) / 2;
        if (canEscape(mid)) {
            answer = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (answer >= LIMIT) cout << LIMIT << '\n';
    else cout << answer << '\n';
    
    return 0;
}