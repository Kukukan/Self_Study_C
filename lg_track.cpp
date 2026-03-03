#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if(!(cin>>N)) return 0;
    vector<int> bff(N+1);
    for(int i=1;i<=N;i++) cin>>bff[i];

    vector<vector<int>> rev(N+1);
    for(int i=1;i<=N;i++) rev[bff[i]].push_back(i);

    vector<int> vis(N+1,0);
    vector<char> in_cycle(N+1, false);
    vector<int> cycle_len(N+1,0);

    for(int i=1;i<=N;i++){
        if(vis[i]) continue;
        int cur=i;
        while(!vis[cur]){ 
            vis[cur]=i; 
            cur=bff[cur]; 
        }
        if(vis[cur]==i){
            int v=cur; int len=0;
            do{ 
                in_cycle[v]=true; 
                len++; 
                v=bff[v]; 
            } while(v!=cur);
            v=cur;
            do{ 
                cycle_len[v]=len; 
                v=bff[v]; 
            } while(v!=cur);
        }
    }

    // dp for nodes not in cycles: longest chain length ending at that node (counting that node)
    vector<int> dp(N+1,0);
    function<int(int)> dfs = [&](int u)->int{
        if(dp[u]) return dp[u];
        dp[u]=1;
        for(int v: rev[u]){
            if(in_cycle[v]) continue;
            dp[u]=max(dp[u], 1 + dfs(v));
        }
        return dp[u];
    };

    int max_cycle = 0;
    for(int i=1;i<=N;i++) max_cycle = max(max_cycle, cycle_len[i]);

    int pair_sum = 0;
    vector<char> counted(N+1, false);
    for(int i=1;i<=N;i++){
        int j = bff[i];
        if(i < j && bff[j]==i){
            // mutual pair i <-> j
            // get best chain into i and into j (excluding any cycle nodes)
            int best_i = 0, best_j = 0;
            for(int v: rev[i]) if(!in_cycle[v]) best_i = max(best_i, dfs(v));
            for(int v: rev[j]) if(!in_cycle[v]) best_j = max(best_j, dfs(v));
            pair_sum += 2 + best_i + best_j;
        }
    }

    cout << max(max_cycle, pair_sum) << '\n';
    return 0;
}
