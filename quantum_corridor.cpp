#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll K;
    if (!(cin >> n >> K)) return 0;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n == 1) {
        cout << 0;
        return 0;
    }

    unordered_map<ll, vector<int>> pos;
    pos.reserve(n * 2);
    for (int i = 0; i < n; ++i) pos[a[i]].push_back(i);

    vector<int> dist(n, -1);
    queue<int> q;
    q.push(0);
    dist[0] = 0;

    // BFS
    while (!q.empty()) {
        int idx = q.front(); q.pop();
        if (idx == n - 1) break;

        // walk to neighbors
        for (int ni : {idx - 1, idx + 1}) {
            if (ni >= 0 && ni < n && dist[ni] == -1) {
                dist[ni] = dist[idx] + 1;
                q.push(ni);
            }
        }

        // teleport to values v+K and v-K
        ll v = a[idx];
        
        // Check for overflow before computing targets
        vector<ll> targets;
        if (K >= 0) {
            if (v <= LLONG_MAX - K) targets.push_back(v + K);
            if (v >= LLONG_MIN + K) targets.push_back(v - K);
        } else {
            if (v >= LLONG_MIN - K) targets.push_back(v + K);
            if (v <= LLONG_MAX + K) targets.push_back(v - K);
        }

        for (ll tv : targets) {
            // Check if there are positions with the target value
            auto it = pos.find(tv);
            if (it == pos.end()) continue;  // No positions with this value, skip
            // Teleport to all positions with the target value
            for (int j : it->second) {
                if (dist[j] == -1) {
                    dist[j] = dist[idx] + 1;
                    q.push(j);
                }
            }
            // Remove the target value from the map to prevent future redundant checks
            pos.erase(it);
        }
    }

    cout << dist[n - 1];
    return 0;
}