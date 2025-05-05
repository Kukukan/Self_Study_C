#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, h;
vector<int> batches;

// Function to check if a given speed K is sufficient
bool canInspectAll(int k) {
    long long totalHours = 0;
    for (int donuts : batches) {
        totalHours += (donuts + k - 1) / k; // Equivalent to ceil(donuts / k)
        if (totalHours > h) return false;
    }
    return totalHours <= h;
}

int findMinSpeed() {
    int left = 1, right = *max_element(batches.begin(), batches.end());
    int result = right;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (canInspectAll(mid)) {
            result = mid;  // Found a feasible K, try reducing it
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return result;
}

int main() {
    cin >> n;
    batches.resize(n);

    for (int i = 0; i < n; i++) {
        cin >> batches[i];
    }

    cin >> h;

    cout << findMinSpeed() << endl;
    return 0;
}