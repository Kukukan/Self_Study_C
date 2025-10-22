#include <iostream>
using namespace std;

int R, C; // 격자의 행의 수, 열의 수
int Mat[50][50]; // 격자 데이터


void InputData(void) {
	cin >> R >> C;
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			cin >> Mat[r][c];
		}
	}
}

void OutputData(void) {

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cout << Mat[i][j] << " ";
		}
		cout << endl;
	}
}

#define SWAP(a, b) {int temp = a;a = b;b = temp;}
void Rotate(int sr, int sc, int er, int ec) {
    if(sr >= er || sc >= ec) return;
	// int a = Mat[sr][sc];

	// Move elements from left to right in top row
    for (int c = sc; c < ec; c++) {
        // Mat[sr][c] = Mat[sr][c + 1];
        SWAP(Mat[sr][c], Mat[sr][c + 1]);
    }
    
    // Move elements from top to bottom in right column
    for (int r = sr; r < er; r++) {
        // Mat[r][ec] = Mat[r + 1][ec];
        SWAP(Mat[r][ec], Mat[r + 1][ec]);
    }
    
    // Move elements from right to left in bottom row
    for (int c = ec; c > sc; c--) {
        // Mat[er][c] = Mat[er][c - 1];
        SWAP(Mat[er][c], Mat[er][c - 1]);
    }
    
    // Move elements from bottom to top in left column
    for (int r = er; r > sr; r--) {
        // Mat[r][sc] = Mat[r - 1][sc];
        SWAP(Mat[r][sc], Mat[r - 1][sc]);
    }

    SWAP(Mat[sr + 1][sc], Mat[sr][sc]);
}

void Solve(void) {
	int n = R > C ? R : C;
	for (int i = 0; i < n; i++) {
		Rotate(0 + i, 0 + i, R - 1 - i, C - 1 - i);
	}
}

int main(void) {
	InputData();
	Solve();
	OutputData();
	return 0;
}