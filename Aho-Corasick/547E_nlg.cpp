#include <bits/stdc++.h>
using namespace std;

struct query{
    int r, k, idx, coef;
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

struct BIT{
    static const int OFFSET = 2;
    vector<int> g;
    BIT(int n): g(n + OFFSET, 0) {}
    void update(int p, int v){
        for (p += OFFSET; p < g.size(); p += p & (-p))
            g[p] += v;
    }
    int query(int p){
        int v = 0;
        for (p += OFFSET; p; p -= p & (-p))
            v += g[p];
        return v;
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

    aho_corasick ac;
    vector<vector<int>> ptrs(n);
    for (int i=0; i<n; i++) ptrs[i] = ac.insert_string(s[i]);
    ac.build_automaton();

    vector<vector<int>> g = ac.to_tree();
    vector<int> tin(g.size()), tout(g.size());
    int tdfs = 0;
    dfs(g, tdfs, tin, tout, 0);

    vector<query> a;
    for (int i=0; i<q; i++){
        int l, r, k; cin >> l >> r >> k;
        a.push_back({r-1, k-1, i, 1});
        if (l != 1) a.push_back({l-2, k-1, i, -1});
    }
    sort(a.begin(), a.end(), [](const query &a, const query &b){return a.r < b.r;});

    vector<int> result(q);
    BIT bit(g.size());

    int ptr_s = 0;
    for (const query &qr: a){
        for (; ptr_s <= qr.r; ptr_s++)
            for (int pos: ptrs[ptr_s])
                bit.update(tin[pos], 1);
        int v = ptrs[qr.k].back();
        result[qr.idx] += qr.coef * (bit.query(tout[v]) - bit.query(tin[v] - 1));
    }

    for (int v: result) cout << v << "\n";
}
