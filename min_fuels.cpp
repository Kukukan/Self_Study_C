#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ll target, startFuel, n;
    cin >> target >> startFuel >> n;
    vector<pair<ll, ll>> stations;
    for (int i = 0; i < n; i++) {
        ll pos, fuel;
        cin >> pos >> fuel;
        stations.push_back({pos, fuel});
    }
    
    priority_queue<ll> pq;
    int i = 0;
    int stops = 0;
    ll currentFuel = startFuel;
    
    while (currentFuel < target) {
        while (i < n && stations[i].first <= currentFuel) {
            pq.push(stations[i].second);
            i++;
        }
        
        if (pq.empty()) {
            cout << -1 << endl;
            return 0;
        }
        
        currentFuel += pq.top();
        pq.pop();
        stops++;
    }
    
    cout << stops << endl;
    return 0;
}