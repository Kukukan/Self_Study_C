#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int N; // 이물질의 개수
int K; // 장비 최대 사용가능 횟수
// int X[50000]; // 이물질의 위치


int main() {
	// ll ans = -1;
	// InputData(); // 입력 받는 부분
    cin >> N >> K;
    vector<ll> X(N);
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}

	// 여기서부터 작성
	sort(X.begin(), X.end());
    // for(auto x : X) cout << x << " ";
    // cout << "\n";
	ll low = 0, high = INT_MAX;
	while(low < high) {
		ll mid = low + (high-low)/2;
		int cnt = 0;
		int j = 0;
		while(j < N) {
			cnt++;
			if(cnt > K) break;
			ll cover = X[j] + 2* mid;
			while(j < N && cover >= X[j]) {
				j++;
			}
		}
		if(j < N) {
			low = mid + 1;
			// ans = low;
		} else {
			high = mid;
		}
	}
	cout << low << endl;// 출력하는 부분
	return 0;
}