#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> Box(150, vector<int>(150));

void InputData() {
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> Box[i][j];
        }
    }
}

int main() {
	int ans = -1;
    InputData();
    
    int total = 0;
    int maxRow = 0;
    int maxCol = 0;
    
    for (int j = 0; j < N; j++) {
		int rowSum = 0;
        for (int i = 0; i < N; i++) {
			rowSum += Box[i][j];
        }
		total += rowSum;
		maxRow = max(rowSum, maxRow);
    }
	
	for (int i = 0; i < N; i++) {
		int colSum = 0;
        for (int j = 0; j < N; j++) {
			colSum += Box[i][j];
        }
		maxCol = max(colSum, maxCol);
    }
		
	int wrap = max(maxRow, maxCol);
	ans = N* wrap - total;
    
    cout << ans << endl;
    
    return 0;
}