#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<ll> s(N);
    for (int i = 0; i < N; ++i) cin >> s[i];

    // quick bounds check
    for (int i = 0; i < N; ++i) {
        if (s[i] < 0 || s[i] > N-1) {
            cout << "NO\n";
            return 0;
        }
    }

    sort(s.begin(), s.end()); // nondecreasing

    ll total = 0;
    for (auto v : s) total += v;
    ll matches = 1LL * N * (N - 1) / 2;
    if (total != matches) {
        cout << "NO\n";
        return 0;
    }

    ll prefix = 0;
    for (int k = 1; k <= N; ++k) {
        prefix += s[k-1];
        ll need = 1LL * k * (k - 1) / 2;
        if (prefix < need) {
            cout << "NO\n";
            return 0;
        }
    }

    cout << "YES\n";
    return 0;
}