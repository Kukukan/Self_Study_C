#include <bits/stdc++.h>
using namespace std;


using ll = long long;

// Count how many values in matrix are <= mid
ll countLessEqual(ll n, ll mid) {
    ll count = 0;
    for (int i = 1; i <= n; i++) {
        // For each row i, count numbers <= mid
        count += min(n, mid / i);
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    ll n, k;
    cin >> n >> k;
    
    // Binary search on answer
    ll left = 1, right = n * n;
    ll answer = 0;
    
    while (left <= right) {
        ll mid = left + (right - left) / 2;
        ll count = countLessEqual(n, mid);
        
        if (count >= k) {
            answer = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    cout << answer << endl;
    return 0;
}