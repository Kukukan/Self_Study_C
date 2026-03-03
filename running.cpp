#include <bits/stdc++.h>
using namespace std;

int N;//선수의 인원수 number of playersnumber of players
long long T;//시간 time
long long P[100000 + 10];//선수 초기 위치 player initial position
long long S[100000 + 10];//선수 속도 player speed
// int group_first[100000 + 10];//각 그룹의 선두선수 leader of each group


void InputData() {
	cin >> N >> T;
	for (int i = 0; i < N; i++) {
		cin >> P[i] >> S[i];
	}
}

int main() {
	// int ans = -1;
	InputData();//입력 Input

	//코드를 작성하세요 Write the code
    stack<pair<long long, long long>> st; // {position, speed}
    for(int i = 0; i < N; ++i) {
        long long final_pos = P[i] + S[i] * T;
        while(!st.empty() && st.top().first >= final_pos) {
            st.pop();
        }
        st.push({final_pos, i+1});
    }

	//출력 Output
    cout << st.size() << endl;
    while(!st.empty()) {
        cout << st.top().second << " ";
        st.pop();
    }
	// cout << ans << endl;
	// for (int i = 0; i < ans; i++) cout << group_first[i] << " ";
	return 0;
}