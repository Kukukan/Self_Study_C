#include <bits/stdc++.h>
using namespace std;

// Function to read input values
vector<int> read_input(int& total_floors) {
    cin >> total_floors;
    vector<int> battery_power(total_floors);
    for (int& power : battery_power) {
        cin >> power;
    }
    return battery_power;
}

// BFS function to calculate minimum escape steps
int calculate_min_teleports(const vector<int>& battery_power, int total_floors) {
    vector<bool> visited(total_floors, false);
    queue<pair<int, int>> q; // (current_floor, steps_taken)

    // Start from floor 0 with 0 steps
    q.push({0, 0});
    visited[0] = true;

    while (!q.empty()) {
        int current_floor = q.front().first;
        int steps = q.front().second;
        q.pop();

        int current_power = battery_power[current_floor];

        // Try all three possible jump variations
        vector<int> jumps = {current_power - 1, current_power, current_power + 1};
        for (int jump : jumps) {
            int next_floor = current_floor + jump;

            // Escape successful condition
            if (next_floor >= total_floors) {
                return steps + 1;
            }

            // Check valid unvisited floor
            if (next_floor >= 0 && next_floor < total_floors && !visited[next_floor]) {
                visited[next_floor] = true;
                q.push({next_floor, steps + 1});
            }
        }
    }

    return -1; // Should never reach here according to problem statement
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int total_floors;
    vector<int> battery_power = read_input(total_floors);
    
    int result = calculate_min_teleports(battery_power, total_floors);
    cout << result << endl;

    return 0;
}