#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // Build prefix sums for even/odd positions
    // preEven[i] = sum of a[j] for j in [0..i-1] where j is even
    // preOdd[i]  = sum of a[j] for j in [0..i-1] where j is odd
    vector<long long> preEven(n + 1, 0), preOdd(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        preEven[i + 1] = preEven[i] + ((i % 2 == 0) ? a[i] : 0);
        preOdd[i + 1]  = preOdd[i]  + ((i % 2 == 1) ? a[i] : 0);
    }

    long long total_even = preEven[n];
    long long total_odd = preOdd[n];

    long long ways = 0;
    for (int i = 0; i < n; ++i) {
        // sums strictly to the left of i
        long long left_even = preEven[i];
        long long left_odd  = preOdd[i];

        // sums strictly to the right of i with original parity
        long long right_even_orig = total_even - preEven[i + 1];
        long long right_odd_orig  = total_odd  - preOdd[i + 1];

        // after removing a[i], elements to the right shift left by 1 so
        // their parity flips. New sums:
        // newEven = left_even + right_odd_orig
        // newOdd  = left_odd  + right_even_orig
        long long newEven = left_even + right_odd_orig;
        long long newOdd  = left_odd  + right_even_orig;

        if (newEven == newOdd) ++ways;
    }

    cout << ways << '\n';
    return 0;
}
