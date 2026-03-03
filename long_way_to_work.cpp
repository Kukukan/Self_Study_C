#include <bits/stdc++.h>

using namespace std;

using pii = pair<int,int>;
using ll = long long;

bool compare(pii a, pii b) {
    return a.first < b.first;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    ll G, B, D;
    cin >> N >> G >> B >> D;
    vector<pii> s(N);
    for (int i = 0; i < N; i++) {
        cin >> s[i].first >> s[i].second;
    }

    if (N == 0) {
        if (D <= B) {
            cout << 0 << endl;
        } else {
            cout << -1 << endl;
        }
        return 0;
    }

    sort(s.begin(), s.end(), compare);

    if (s[0].first > B) {
        cout << -1 << endl;
        return 0;
    }
    for (int i = 1; i < N; i++) {
        if (s[i].first - s[i-1].first > G) {
            cout << -1 << endl;
            return 0;
        }
    }
    if (D - s[N-1].first > G) {
        cout << -1 << endl;
        return 0;
    }

    vector<pii> stations(N+1);
    for (int i = 0; i < N; i++) {
        stations[i] = s[i];
    }
    stations[N] = {D, 0};

    vector<int> next_cheaper(N+1, -1);
    stack<int> st;
    for (int i = N; i >= 0; i--) {
        while (!st.empty() && stations[st.top()].second >= stations[i].second) {
            st.pop();
        }
        if (!st.empty()) {
            next_cheaper[i] = st.top();
        }
        st.push(i);
    }

    ll cost = 0;
    ll current_fuel = B - stations[0].first;

    for (int i = 0; i < N; i++) {
        if (current_fuel < 0) {
            cout << -1 << endl;
            return 0;
        }

        int j = next_cheaper[i];
        if (j == -1) {
            j = N;
        }
        ll d = stations[j].first - stations[i].first;
        ll buy = min(d, G) - current_fuel;
        if (buy < 0) buy = 0;

        cost += buy * stations[i].second;
        current_fuel += buy;

        ll distance_to_next = stations[i+1].first - stations[i].first;
        current_fuel -= distance_to_next;
    }

    cout << cost << endl;

    return 0;
}