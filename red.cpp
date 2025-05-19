#include <bits/stdc++.h>
using namespace std;

int coutnstrok(const string &root, int k, char prim, const string &relate)
{
    int res = 0;
    // bool needed = false;
    int step = 0;

    for(auto c : root) {
        bool needed = (c == prim);
        if(!needed) {
            for(auto re : relate) {
                if(c == re) {
                    needed = true;
                    break;
                }
            }
        }
        if(needed) {
            step++;
        } else {
            if(step > 0) {
                res += (step + k-1)/k;
                step = 0;
            }
        }
    }

    if(step > 0) {
        res += (step + k-1)/k;
    }

    return res;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<char> rgb = {'R', 'G', 'B'};
    vector<string> comb = {"YPW", "YCW", "PCW"};
    int t;  cin >> t;
    while (t--) {
        int n;  cin >> n;
        string s;   cin >> s;

        int result = 0;
        for(int i = 0; i < 3; ++i) {
            result += coutnstrok(s, n, rgb[i], comb[i]);
        }

        cout << result  << endl;
    }
    

    return 0;
}
