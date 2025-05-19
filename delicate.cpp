#include <bits/stdc++.h>
using namespace std;

int calculate(int wt)
{
    int cnt = 0;
    for(int n = 1; ; ++n) {
        long long tmp = (long long)(n*(n-1)/2);
        if(tmp >= wt) break;
        long long ts = wt - tmp;
        if(ts % n == 0) {
            if(ts/n >= 1) {
                ++cnt;
            }
        }
    }
    return cnt;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t;  cin>>t;
    while(t--) {
        int w;  cin >> w;
        cout << calculate(w) << endl;
    }
    return 0;
}
