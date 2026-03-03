#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int N;
int M;
vector<vector<tuple<int, int, int>>> adj;
vector<tuple<int, int, int>> roads;

vector<int> dijkstra(int start, const vector<vector<tuple<int, int, int>>> &adj, const set<int> &removed_roads) {
    vector<int> dist(N+1, INF);
    dist[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();

        if (current_dist > dist[current_node]) continue;

        for (auto &edge : adj[current_node]) {
            int to = get<0>(edge);
            int weight = get<1>(edge);
            int road_id = get<2>(edge);

            if (removed_roads.find(road_id) != removed_roads.end()) continue;

            if (dist[to] > dist[current_node] + weight) {
                dist[to] = dist[current_node] + weight;
                pq.push({dist[to], to});
            }
        }
    }

    return dist;
}

int main() {
    cin >> N >> M;
    roads.resize(M);
    adj.resize(N+1);

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        roads[i] = {u, v, c};
        adj[u].push_back({v, c, i});
        adj[v].push_back({u, c, i});
    }

    vector<int> dist1 = dijkstra(1, adj, set<int>());
    vector<int> distN = dijkstra(N, adj, set<int>());
    int original_dist = dist1[N];

    vector<int> possible_new_dists;

    for (int i = 0; i < M; i++) {
        int u = get<0>(roads[i]);
        int v = get<1>(roads[i]);
        int c = get<2>(roads[i]);

        if ((dist1[u] + c + distN[v] == original_dist) || (dist1[v] + c + distN[u] == original_dist)) {
            set<int> removed = {i};
            vector<int> new_dist = dijkstra(1, adj, removed);
            int new_path = new_dist[N];
            if (new_path != INF) {
                possible_new_dists.push_back(new_path);
            }
        }
    }

    if (possible_new_dists.empty()) {
        cout << "INF" << endl;
    } else {
        int max_new = *max_element(possible_new_dists.begin(), possible_new_dists.end());
        cout << max_new << endl;
    }

    return 0;
}