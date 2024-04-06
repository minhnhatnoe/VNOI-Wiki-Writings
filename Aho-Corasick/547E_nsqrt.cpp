#include <bits/stdc++.h>
using namespace std;

struct query{
    int r, k, idx, coef;
};

struct aho_corasick{
    struct node{
        int suffix_link = -1, exit_link = -1, cnt = 0, apr = 0, nxt[26];
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
    void update(int ptr){
        for (ptr = g[ptr].cnt ? ptr : g[ptr].exit_link; ptr != -1; ptr = g[ptr].exit_link)
            g[ptr].apr++;
    }
};

signed main(){
    cin.tie(0)->sync_with_stdio(0);
    int n, q; cin >> n >> q;

    vector<string> s(n);
    for (int i=0; i<n; i++) cin >> s[i];

    aho_corasick ac;
    vector<vector<int>> ptrs(n);
    for (int i=0; i<n; i++) ptrs[i] = ac.insert_string(s[i]);
    ac.build_automaton();

    vector<query> a;
    for (int i=0; i<q; i++){
        int l, r, k; cin >> l >> r >> k;
        a.push_back({r-1, k-1, i, 1});
        if (l != 1) a.push_back({l-2, k-1, i, -1});
    }
    sort(a.begin(), a.end(), [](const query &a, const query &b){return a.r < b.r;});

    vector<int> result(q);

    int ptr_s = 0;
    for (const query &qr: a){
        for (; ptr_s <= qr.r; ptr_s++)
            for (int pos: ptrs[ptr_s])
                ac.update(pos);
        int v = ptrs[qr.k].back();
        result[qr.idx] += qr.coef * ac.g[v].apr;
    }

    for (int v: result) cout << v << "\n";
}
