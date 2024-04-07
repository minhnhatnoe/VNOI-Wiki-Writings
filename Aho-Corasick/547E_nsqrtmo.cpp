#include <bits/stdc++.h>
using namespace std;
const int SQRT = 450;

struct query{
    int l, r, k, idx;
};

struct aho_corasick{
    struct node{
        int suffix_link = -1, exit_link = -1, cnt = 0, nxt[26];
        node() {fill(nxt, nxt+26, -1);}
    };
    vector<node> g = {node()};
    vector<int> insert_string(const string &s){
        vector<int> ptr = {0};
        for (char c: s){
            if (g[ptr.back()].nxt[c - 'a'] == -1){
                g[ptr.back()].nxt[c - 'a'] = g.size();
                g.emplace_back();
            }
            ptr.push_back(g[ptr.back()].nxt[c - 'a']);
        }
        g[ptr.back()].cnt++;
        return ptr;
    }
    void build_automaton(){
        for (deque<int> q = {0}; q.size(); q.pop_front()){
            int v = q.front(), suffix_link = g[v].suffix_link;
            if (v) g[v].exit_link = g[suffix_link].cnt ? suffix_link : g[suffix_link].exit_link;
            for (int i=0; i<26; i++){
                int &nxt = g[v].nxt[i], nxt_sf = v ? g[suffix_link].nxt[i] : 0;
                if (nxt == -1) nxt = nxt_sf;
                else{
                    g[nxt].suffix_link = nxt_sf;
                    q.push_back(nxt);
                }
            }
        }
    }
    vector<vector<int>> to_tree(){
        vector<vector<int>> tree(g.size());
        for (int i=1; i<g.size(); i++)
            tree[g[i].suffix_link].push_back(i);
        return tree;
    }
};

void dfs(const vector<vector<int>> &g, int &tdfs, vector<int> &tin, vector<int> &tout, int v){
    tin[v] = tdfs++;
    for (int u: g[v])
        dfs(g, tdfs, tin, tout, u);
    tout[v] = tdfs-1;
}
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    int n, q; cin >> n >> q;

    vector<string> s(n);
    for (int i=0; i<n; i++) cin >> s[i];

    vector<int> pfs(n);
    for (int i=0; i<n; i++) pfs[i] = s[i].size();
    partial_sum(pfs.begin(), pfs.end(), pfs.begin());

    aho_corasick ac;
    vector<vector<int>> ptrs(n);
    for (int i=0; i<n; i++) ptrs[i] = ac.insert_string(s[i]);
    ac.build_automaton();

    vector<vector<int>> g = ac.to_tree();
    vector<int> tin(g.size()), tout(g.size());
    int tdfs = 0;
    dfs(g, tdfs, tin, tout, 0);

    vector<query> a(q);
    for (int i=0; i<q; i++){
        int l, r, k; cin >> l >> r >> k;
        a[i] = {l-1, r-1, k-1, i};
    }
    sort(a.begin(), a.end(), [&pfs](const query &a, const query &b) -> bool{
        int la = a.l == 0 ? 0 : pfs[a.l-1], lb = b.l == 0 ? 0 : pfs[b.l-1];
        int ra = pfs[a.r], rb = pfs[b.r];

        int block_a = la / SQRT, block_b = lb / SQRT;
        if (block_a != block_b) return block_a < block_b;
        return ra == rb ? 0 : (ra < rb) ^ (block_a % 2);
    });

    vector<int> apr(g.size()), bapr((g.size()-1) / SQRT + 1);
    auto update_string = [&ptrs, &apr, &bapr, &tin](int ptr, int coef){
        for (int pos: ptrs[ptr]){
            pos = tin[pos];
            apr[pos] += coef;
            bapr[pos/SQRT] += coef;
        }
    };

    int l = 0, r = -1;
    vector<int> result(q);
    for (const query &qr: a){
        for (; r+1 <= qr.r; r++) update_string(r+1, 1);
        for (; l-1 >= qr.l; l--) update_string(l-1, 1);

        for (; r > qr.r; r--) update_string(r, -1);
        for (; l < qr.l; l++) update_string(l, -1);

        int v = ptrs[qr.k].back();
        int &rs = result[qr.idx];
        for (int i=tin[v]; i<=tout[v];){
            if (i + SQRT > tout[v]+1 || i % SQRT) rs += apr[i], i++;
            else rs += bapr[i / SQRT], i += SQRT;
        }
    }

    for (int v: result) cout << v << "\n";
}
