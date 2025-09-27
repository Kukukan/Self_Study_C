#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool canDeliver(const vector<ll>& w, int k, ll cap) {
    int trips = 1;
    ll cur = 0;
    for (ll weight : w) {
        if (weight > cap) return false;
        if (cur + weight > cap) { 
            ++trips; cur = weight; 
        }
        else cur += weight;
        if (trips > k) return false;
    }
    return true;
}

ll findMinCapacity(vector<ll>& w, int k) {
    ll lo = *max_element(w.begin(), w.end());
    ll hi = accumulate(w.begin(), w.end(), 0LL);
    ll ans = hi;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        if (canDeliver(w, k, mid)) { 
            ans = mid; 
            hi = mid - 1; 
        }
        else lo = mid + 1;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<ll> w(n);
    for (int i = 0; i < n; ++i) cin >> w[i];
    cout << findMinCapacity(w, k);
    return 0;
}
