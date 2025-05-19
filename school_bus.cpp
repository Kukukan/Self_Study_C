#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Apartment {
    int distance;
    int students;
};

ll process_group(vector<pair<int, int>>& group, int K) {
    ll total = 0;
    int current_capacity = K;
    int current_max = 0;

    for (auto& apt : group) {
        int students = apt.second;
        while (students > 0) {
            int take = min(students, current_capacity);
            students -= take;
            current_capacity -= take;
            current_max = max(current_max, apt.first);

            if (current_capacity == 0) {
                total += 2 * current_max;
                current_capacity = K;
                current_max = 0;
            }
        }
    }

    if (current_max > 0) {
        total += 2 * current_max;
    }

    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K, S;
    cin >> N >> K >> S;

    vector<pair<int, int>> left, right;

    for (int i = 0; i < N; ++i) {
        int x, s;
        cin >> x >> s;
        if (x < S) {
            left.push_back({S - x, s});
        } else if (x > S) {
            right.push_back({x - S, s});
        }
    }

    sort(left.begin(), left.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;
    });

    sort(right.begin(), right.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;
    });

    ll left_total = process_group(left, K);
    ll right_total = process_group(right, K);

    cout << left_total + right_total << endl;

    return 0;
}