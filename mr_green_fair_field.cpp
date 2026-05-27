#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

bool is_carry(vector<ll> x, ll t, int p)
{
    int ok = 1;
    ll total_s = 0;
    for(ll i = 0; i < x.size(); ++i) {
        total_s += x[i];
        if(total_s > t) {
            ok++;
            total_s = x[i];
        }
        if(x[i] == -1) {
            continue;
        }
    }
    return ok <= p;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    ll n;
    int k;
    cin >> n >> k;
    vector<ll> s_bag;
    s_bag.resize(n);
    for(ll i = 0; i < n; ++i) {
        cin >> s_bag[i];
        if(s_bag[i] == -1) {
            s_bag[i] = 0;
        }
    }
    
    ll l = *max_element(s_bag.begin(), s_bag.end());
    // cout << l;
    ll r = accumulate(s_bag.begin(), s_bag.end(), 0LL);
    ll ans = r;
    while(l < r) {
        ll m = l + (r-l)/2;
        if(is_carry(s_bag, m, k)) {
            ans = m;
            r = m;
        } else {
            l = m + 1;
        }
    }
    cout << ans;
    return 0;
}
