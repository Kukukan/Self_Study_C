#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(cin >> u >> v)) return 0;
        edges[i] = {u, v};
    }
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, INT_MIN));
    queue<pair<int, int>> q;
    vector<int> indeg(n + 1, 0);
    for (const auto& [u, v] : edges) {
        indeg[v]++;
    }
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            dp[i][1] = 0;
            q.push({i, 1});
        }
    }
    while(!q.empty()) {
        auto [u, len] = q.front(); q.pop();
        for (const auto& [x, y] : edges) {
            if (x == u) {
                dp[y][len + 1] = max(dp[y][len + 1], dp[u][len] + 1);
                indeg[y]--;
                if (indeg[y] == 0) {
                    q.push({y, len + 1});
                }
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            ans = max(ans, dp[i][j]);
        }
    }
    cout << ans;
    return 0;
}