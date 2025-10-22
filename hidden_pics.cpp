#include <iostream>
using namespace std;

int N;
int a[10 + 10][10 + 10];
int X1, X2, X3;

void InputData(void) {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> a[i][j];
		}
	}
	cin >> X1 >> X2 >> X3;
}

int main(void) {
	int ans = 0;
	InputData();
	
	int di[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int dj[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (a[i][j] != X1) continue;
			
			for (int k = 0; k < 8; k++) {
				int ni1 = i + di[k];
				int nj1 = j + dj[k];
				if (ni1 < 0 || ni1 >= N || nj1 < 0 || nj1 >= N) continue;
				if (a[ni1][nj1] != X2) continue;
				
				int ni2 = ni1 + di[k];
				int nj2 = nj1 + dj[k];
				if (ni2 < 0 || ni2 >= N || nj2 < 0 || nj2 >= N) continue;
				if (a[ni2][nj2] != X3) continue;
				
				ans++;
			}
		}
	}
	
	cout << ans;
	return 0;
}