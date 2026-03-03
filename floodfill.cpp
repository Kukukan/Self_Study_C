#include <bits/stdc++.h>
using namespace std;

const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

// void floodFill(vector<vector<int>>& grid, int x, int y, int target, int replacement) {
//     if (target == replacement) return;
//     int n = grid.size(), m = grid[0].size();
//     if (x < 0 || x >= n || y < 0 || y >= m) return;
//     if (grid[x][y] != target) return;

//     grid[x][y] = replacement;
//     for (int dir = 0; dir < 4; ++dir) {
//         floodFill(grid, x + dx[dir], y + dy[dir], target, replacement);
//     }
// }

// void floodFillBFS(vector<vector<int>>& grid, int x, int y, int target, int replacement) {
//     int n = grid.size(), m = grid[0].size();
//     if (grid[x][y] != target) return;

//     queue<pair<int, int>> q;
//     q.push({x, y});
//     grid[x][y] = replacement;

//     while (!q.empty()) {
//         auto [cx, cy] = q.front(); q.pop();
//         for (int dir = 0; dir < 4; ++dir) {
//             int nx = cx + dx[dir], ny = cy + dy[dir];
//             if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == target) {
//                 grid[nx][ny] = replacement;
//                 q.push({nx, ny});
//             }
//         }
//     }
// }

vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        // int dx[] = {0, 0, 1, -1};
        // int dy[] = {1, -1, 0, 0};
        int ori = image[sr][sc];
        if(ori == color) {
            return image;
        }
        int old = image[sr][sc];
        queue<pair<int,int>> q;
        q.push({sr,sc});
        image[sr][sc] = color;
        while(!q.empty()) {
            auto [nx,ny] = q.front();
            q.pop();
            for(int dir = 0; dir < 4; dir++) {
                int cx = nx + dx[dir];
                int cy = ny + dy[dir];
                if(cx >= 0 && cx < image.size() && cy >=0 && cy < image[0].size() && old == image[cx][cy]) {
                    image[cx][cy] = color;
                    q.push({cx,cy});
                }
            }
        }
        return image; 
    }

int main() {
    vector<vector<int>> grid = {
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 1},
    };

    int startX = 1, startY = 1;
    int target = 1, replacement = 2;
    floodFill(grid, startX, startY, replacement);

    for (const auto& row : grid) {
        for (int cell : row) cout << cell << " ";
        cout << endl;
    }
    return 0;
}