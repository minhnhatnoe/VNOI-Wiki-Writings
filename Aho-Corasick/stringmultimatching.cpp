#include <bits/stdc++.h>
using namespace std;

struct aho_corasick{
    struct node{
        int suffix_link = -1, exit_link = -1, nxt[128];
        vector<int> leaf;
        node() {fill(nxt, nxt+128, -1);}
    };
    vector<node> g = {node()};
    void insert_string(const string &s, int sidx){
        int p = 0;
        for (char c: s){
            if (g[p].nxt[c] == -1){
                g[p].nxt[c] = g.size();
                g.emplace_back();
            }
            p = g[p].nxt[c];
        }
        g[p].leaf.push_back(sidx);
    }
    void build_automaton(){
        for (deque<int> q = {0}; q.size(); q.pop_front()){
            int v = q.front(), suffix_link = g[v].suffix_link;
            if (v) g[v].exit_link = g[suffix_link].leaf.size() ? suffix_link : g[suffix_link].exit_link;
            for (int i=0; i<128; i++){
                int &nxt = g[v].nxt[i], nxt_sf = v ? g[suffix_link].nxt[i] : 0;
                if (nxt == -1) nxt = nxt_sf;
                else{
                    g[nxt].suffix_link = nxt_sf;
                    q.push_back(nxt);
                }
            }
        }
    }
    vector<int> get_sindex(int p){
        vector<int> a;
        for (int v = g[p].leaf.size() ? p : g[p].exit_link; v != -1; v = g[v].exit_link)
            for (int j: g[v].leaf)
                a.push_back(j);
        return a;
    }
};

signed main(){
    cin.tie(0)->sync_with_stdio(0);
    string n_line;
    while (getline(cin, n_line)){
        int n = stoi(n_line);

        vector<int> s_size(n);
        aho_corasick ac;
        for (int i=0; i<n; i++){
            string s; getline(cin, s);
            ac.insert_string(s, i);
            s_size[i] = s.size();
        }
        ac.build_automaton();

        vector<vector<int>> result(n);
        string t; getline(cin, t);
        for (int i=0, p=0; i<t.size(); i++){
            p = ac.g[p].nxt[t[i]];
            for (int j: ac.get_sindex(p))
                result[j].push_back(i - s_size[j] + 1);
        }
        
        for (const vector<int> &v: result){
            if (v.size() == 0) cout << "\n";
            else for (int i=0; i<v.size(); i++)
                cout << v[i] << " \n"[i == v.size()-1];
        }
    }
}