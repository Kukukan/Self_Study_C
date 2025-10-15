#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<ll> heights(n);
    for(int i = 0; i < n; ++i) {
        cin >> heights[i];
    }
    
    // Maintain a monotonic increasing stack of (index, height)
    // For each position, find the first smaller height to left and right
    stack<int> st;
    vector<int> left(n), right(n, n);
    
    // Find first smaller element to the left
    for(int i = 0; i < n; ++i) {
        while(!st.empty() && heights[st.top()] >= heights[i]) {
            st.pop();
        }
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    
    // Clear stack for reuse
    while(!st.empty()) st.pop();
    
    // Find first smaller element to the right
    for(int i = n-1; i >= 0; --i) {
        while(!st.empty() && heights[st.top()] >= heights[i]) {
            st.pop();
        }
        right[i] = st.empty() ? n : st.top();
        st.push(i);
    }
    
    // For each bar, compute max rectangle with this bar as minimum height
    ll max_area = 0;
    for(int i = 0; i < n; ++i) {
        // Width is (right[i] - left[i] - 1) because right and left are exclusive
        ll width = right[i] - left[i] - 1;
        ll area = width * heights[i];
        max_area = max(max_area, area);
    }
    
    cout << max_area << '\n';
    return 0;
}