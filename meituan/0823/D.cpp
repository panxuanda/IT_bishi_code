/*时限3s，C++解决下列问题，看看需不需要树链剖分：
给定一棵以节点 1为根的树，树上共有,个节点，其中某些节点被标记为"红点"。每条边(u,v)具有正整数权重 Wuv。

接下来有 q次操作，每次操作有两种类型:1.切换节点v的红点状态(若为红点则变为非红点，反之亦然);

2.查询节点 v到所有当前红点的带权距离之和 S,。

请对所有查询操作输出对应结果。

【名词解释】

。带权距离:带权距离 指路径上所有边权的总和。

1输入描述

第一行输入两个整数 n,q(1 ≦ n,q≦2x 105)，分别表示节点数和操作数。

第二行输入n, 个整数 C1,C2,·..,Cn 属于{0,1}，其中 Ci= 1表示第

i个节点初始为红点，C= 0 表示非红点。接下来n -1行，每行输入三个整数ui,vi,wi(l≤ui,vi≤n,ui≠vi,l≤wi≤10^6)，表示一条无向带权边。随后q行，每行输入两个整数t和v(t ∈{1,2},1 ≤v≤n)，表示一次操作。

保证所有输入的边构成一棵树，并且至少存在一个操作 2。

输出描述

对于每个操作类型 2，输出一行整数，表示节点到所有当前红点的带权距离之和S



输入：

5 5

1 0 1 0 1

1 2 1

1 3 2

3 4 3

3 5 4

2 1

2 2

1 3

2 2

2 2

输出：

8

11

8

8
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n, q;
const int MAXN = 200000 + 5;

vector<pair<int,int>> adj[MAXN]; // (to, weight)
int sz[MAXN];
bool removed_[MAXN];
int parent_centroid[MAXN];

void dfs_size(int u, int p){
    sz[u] = 1;
    for(auto [v,w]: adj[u]){
        if(v==p || removed_[v]) continue;
        dfs_size(v,u);
        sz[u] += sz[v];
    }
}

int find_centroid(int u, int p, int total){
    for(auto [v,w]: adj[u]){
        if(v==p || removed_[v]) continue;
        if(sz[v] > total/2) return find_centroid(v,u,total);
    }
    return u;
}

// collect nodes in one partition and distances to centroid
void collect_nodes(int start,int p, ll dist, vector<pair<int,ll>>& out){
    out.emplace_back(start, dist);
    for(auto [v,w]: adj[start]){
        if(v==p || removed_[v]) continue;
        collect_nodes(v, start, dist + w, out);
    }
}

// For each node we need list of (centroid, dist_to_centroid, childIndex)
// childIndex == -1 means node is the centroid itself (no child partition)
struct AncInfo { int c; int idx; ll dist; };
vector<AncInfo> anc[MAXN];

// For each centroid node c:
// totCnt[c], totSumDist[c] : stats for all red nodes in c's component
// cntChild[c][i], sumChild[c][i] : stats for the i-th partition (child) of centroid c
ll totCnt[MAXN], totSumDist[MAXN];
vector<ll> cntChild[MAXN], sumChild[MAXN];

void build_centroid(int entry, int p){
    dfs_size(entry, -1);
    int c = find_centroid(entry, -1, sz[entry]);
    removed_[c] = true;
    parent_centroid[c] = p;

    // centroid itself: add anc with idx = -1 and dist = 0
    anc[c].push_back({c, -1, 0});

    // for each not removed neighbor, collect nodes and assign them childIndex
    int childIndex = 0;
    for(auto [v,w]: adj[c]){
        if(removed_[v]) continue;
        vector<pair<int,ll>> nodes;
        collect_nodes(v, c, w, nodes); // distances from node to centroid c
        // make storage for this child partition
        cntChild[c].push_back(0);
        sumChild[c].push_back(0);
        for(auto &pr : nodes){
            int node = pr.first;
            ll d = pr.second;
            anc[node].push_back({c, childIndex, d});
        }
        ++childIndex;
    }

    // recurse on partitions
    for(auto [v,w] : adj[c]){
        if(removed_[v]) continue;
        build_centroid(v, c);
    }
    // do not unmark removed_ -- centroid stays removed in decomposition
}

vector<int> color; // 0/1 current color

void update_node(int v, int delta){
    // delta = +1 for add red, -1 for remove red
    for(auto &a : anc[v]){
        int c = a.c;
        int idx = a.idx;
        ll d = a.dist;
        totCnt[c] += delta;
        totSumDist[c] += delta * d;
        if(idx != -1){
            cntChild[c][idx] += delta;
            sumChild[c][idx] += delta * d;
        }
    }
}

ll query_node(int v){
    ll ans = 0;
    for(auto &a : anc[v]){
        int c = a.c;
        int idx = a.idx;
        ll d = a.dist;
        // contribution of all red nodes stored at centroid c:
        // totSumDist[c] + totCnt[c]*d
        // but need to subtract nodes that are in the same child partition as v (because they will be counted when visiting deeper centroid)
        ll contrib = totSumDist[c] + totCnt[c] * d;
        if(idx != -1){
            contrib -= (sumChild[c][idx] + cntChild[c][idx] * d);
        }
        ans += contrib;
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q;
    color.assign(n+1,0);
    for(int i=1;i<=n;i++){
        int ci; cin >> ci; color[i] = ci;
    }
    for(int i=0;i<n-1;i++){
        int u,v,w; cin >> u >> v >> w;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    // build centroid decomposition (parent_centroid stores centroid-tree parent)
    build_centroid(1, -1);

    // initialize totals by adding initial red nodes
    for(int i=1;i<=n;i++){
        if(color[i]) update_node(i, +1);
    }

    // process queries
    for(int i=0;i<q;i++){
        int t, v; cin >> t >> v;
        if(t==1){
            // toggle
            if(color[v]){
                // currently red -> remove
                update_node(v, -1);
                color[v] = 0;
            } else {
                update_node(v, +1);
                color[v] = 1;
            }
        } else {
            // query
            cout << query_node(v) << '\n';
        }
    }
    return 0;
}
