#include <bits/stdc++.h>
using namespace std;

using pii = pair<int,int>;
const int INF = 1e9;

int R, C;
vector<string> grid;

int dr[4] = {-1, 1, 0, 0}; // U, D, L, R
int dc[4] = {0, 0, -1, 1};

inline bool inb(int r, int c) { return r>=0 && r<R && c>=0 && c<C; }

vector<vector<char>> cameraSeen;
vector<vector<int>> waterState;
vector<pii> exitCells;
vector<vector<int>> exitId;

int arrowToDir(char ch) {
    if (ch == 'U') return 0;
    if (ch == 'D') return 1;
    if (ch == 'L') return 2;
    if (ch == 'R') return 3;
    return -1;
}

void markCameraSightlines() {
    cameraSeen.assign(R, vector<char>(C, 0));
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == 'C') {
                for (int d = 0; d < 4; ++d) {
                    int nr = r + dr[d], nc = c + dc[d];
                    while (inb(nr,nc) && grid[nr][nc] != 'W') {
                        if (grid[nr][nc] == '.' || grid[nr][nc] == 'S') cameraSeen[nr][nc] = 1;
                        nr += dr[d]; nc += dc[d];
                    }
                }
            }
        }
    }
}

int getExitId(int r, int c) {
    if (exitId[r][c] != -1) return exitId[r][c];
    int id = (int)exitCells.size();
    exitCells.emplace_back(r,c);
    exitId[r][c] = id;
    return id;
}

int computeWaterExit(int r, int c) {
    int &st = waterState[r][c];
    if (st != -2) {
        if (st == -3) return -1;
        return st;
    }
    st = -3;

    int d = arrowToDir(grid[r][c]);
    if (d == -1) { st = -1; return st; }
    int nr = r + dr[d], nc = c + dc[d];
    if (!inb(nr,nc)) { st = -1; return st; }

    char ch = grid[nr][nc];
    if (ch == 'W') { st = -1; return st; }
    if (ch == '.' || ch == 'S') {
        if (cameraSeen[nr][nc]) { st = -1; return st; }
        st = getExitId(nr,nc);
        return st;
    }
    if (ch == 'C') { st = -1; return st; }
    if (ch == 'U' || ch == 'D' || ch == 'L' || ch == 'R') {
        int res = computeWaterExit(nr,nc);
        if (res < 0) { st = -1; return st; }
        st = res; return st;
    }
    st = -1; return st;
}

vector<vector<int>> bfsDistances(pii start) {
    vector<vector<int>> dist(R, vector<int>(C, INF));
    deque<pii> q;
    // if start is camera-seen or camera, prisoner is caught immediately
    if (grid[start.first][start.second] == 'C' || cameraSeen[start.first][start.second]) return dist;

    dist[start.first][start.second] = 0;
    q.emplace_back(start);

    while (!q.empty()) {
        auto [r,c] = q.front(); q.pop_front();
        int curd = dist[r][c];
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!inb(nr,nc)) continue;
            char ch = grid[nr][nc];
            if (ch == 'W') continue;
            if (ch == 'C') continue;
            if (ch == '.' || ch == 'S') {
                if (cameraSeen[nr][nc]) continue;
                if (dist[nr][nc] > curd + 1) {
                    dist[nr][nc] = curd + 1;
                    q.emplace_back(nr,nc);
                }
            } else {
                int exit = computeWaterExit(nr,nc);
                if (exit < 0) continue;
                auto [er, ec] = exitCells[exit];
                if (dist[er][ec] > curd + 1) {
                    dist[er][ec] = curd + 1;
                    q.emplace_back(er,ec);
                }
            }
        }
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> R >> C)) return 0;
    grid.resize(R);
    for (int r = 0; r < R; ++r) cin >> grid[r];

    pii start = {-1,-1};
    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) if (grid[r][c] == 'S') start = {r,c};

    markCameraSightlines();

    waterState.assign(R, vector<int>(C, -2));
    exitId.assign(R, vector<int>(C, -1));
    exitCells.clear();

    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) {
        char ch = grid[r][c];
        if (ch=='U' || ch=='D' || ch=='L' || ch=='R') {
            if (waterState[r][c] == -2) {
                int tmp = computeWaterExit(r,c);
                (void)tmp;
            }
        }
    }

    vector<vector<int>> dist = bfsDistances(start);

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == '.') {
                if (dist[r][c] >= INF) cout << -1 << '\n';
                else cout << dist[r][c] << '\n';
            }
        }
    }
    return 0;
}