#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    if(!(cin >> N >> Q)) return 0;
    string s; cin >> s;
    // 1-indexed convenience
    s = " " + s;

    vector<int> pref(N+1,0), suff(N+2,0);
    {
        vector<char> st;
        int ans = 0;
        for(int i=1;i<=N;i++){
            if(i>1 && s[i]==s[i-1]){
                pref[i] = ans;
                continue;
            }
            char c = s[i];
            while(!st.empty() && st.back() > c) st.pop_back();
            if(st.empty() || st.back() < c){ 
                st.push_back(c); 
                ++ans; 
            }
            pref[i] = ans;
        }
    }
    {
        vector<char> st;
        int ans = 0;
        for(int i=N;i>=1;i--){
            if(i<N && s[i]==s[i+1]){ suff[i] = ans; continue; }
            char c = s[i];
            while(!st.empty() && st.back() > c) st.pop_back();
            if(st.empty() || st.back() < c){ 
                st.push_back(c); 
                ++ans; 
            }
            suff[i] = ans;
        }
    }

    for(int qi=0; qi<Q; ++qi){
        int a,b; cin >> a >> b;
        int left = (a-1>=1) ? pref[a-1] : 0;
        int right = (b+1<=N) ? suff[b+1] : 0;
        cout << (left + right) << '\n';
    }
    return 0;
}
