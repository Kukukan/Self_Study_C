#include <iostream>
using namespace std;

int H; // 격자의 세로길이 vertical length of grid
int W; // 격자의 가로길이 horizontal length of grid
int A[100 + 10][100 + 10]; // 격자의 각 칸 each cell of the grid
int N; // 터치 횟수 number of touches
int R[100 + 10]; // 터치하는 칸의 행번호 Line number of the cell you touch
int C[100 + 10]; // 터치하는 칸의 열번호 Column number of the cell you touch

void InputData(void) {
	cin >> H >> W;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			cin >> A[i][j];
		}
	}
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> R[i] >> C[i];
	}
}

int dr[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
int dc[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

void Touch(int r, int c) {
	int V;

	// Toggle the touched cell
	if (A[r][c] == 0) V = 1;
	else V = 0;
	A[r][c] = V;

	// Check all 8 directions
	for (int k = 0; k < 8; k++) {
		int foundV = -1; // Position where first V is found
		bool foundBomb = false;
		
		// First pass: find first V and check for bombs in surrounded cells
		int steps = 0;
		for (int step = 1; ; step++) {
			int nr = r + step * dr[k];
			int nc = c + step * dc[k];
			
			if (nr < 0 || nr >= H || nc < 0 || nc >= W) break;
			
			if (A[nr][nc] == V) {
				foundV = step;
				break;
			}
			if (A[nr][nc] == 2) {
				foundBomb = true;
			}
		}
		
		// If we found V in this direction
		if (foundV != -1) {
			if (foundBomb) {
				// Bomb found in surrounded cells - change all cells in this direction
				for (int step = 1; ; step++) {
					int nr = r + step * dr[k];
					int nc = c + step * dc[k];
					if (nr < 0 || nr >= H || nc < 0 || nc >= W) break;
					A[nr][nc] = V;
				}
			} else {
				// No bomb - change only the surrounded cells
				for (int step = 1; step < foundV; step++) {
					int nr = r + step * dr[k];
					int nc = c + step * dc[k];
					A[nr][nc] = V;
				}
			}
		}
	}
}

int Solve(void) {
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		Touch(R[i], C[i]);
	}
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (A[i][j] == 1) cnt++;
		}
	}
	return cnt;
}

int main(void) {
	int ans = -1;
	InputData(); // 입력 Input
	ans = Solve();
	cout << ans; // 출력 Output
	return 0;
}