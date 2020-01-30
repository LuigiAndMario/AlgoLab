#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

vector<string> s;
vector<int> a;
map<string, int> as_int;
vector<vector<int>> children_of;
vector<vector<pair<int, int>>> queries;
vector<int> path;
vector<int> result;

int bs(int b) {
    int bottom = -1;
    int top = path.size() - 1;
    
    while (bottom < top - 1) {
        int mid = (bottom + top) / 2;
        
        if (a[path[mid]] <= b) {
            top = mid;
        } else {
            bottom = mid;
        }
    }
    
    return path[top];
}

void dfs(int cur) {
    for (auto it = queries[cur].begin() ; it != queries[cur].end() ; it++) {
        result[it->second] = bs(it->first);
    }
    
    for (auto it = children_of[cur].begin() ; it != children_of[cur].end() ; it++) {
        path.push_back(*it);
        dfs(*it);
    }
    
    path.pop_back();
}

// Just the preprocessing
void testcase() {
    int n; cin >> n; // # species
    int q; cin >> q; // # queries
    
    s = vector<string>(n);
    a = vector<int>(n);
    map<string, int> as_int;
    
    for (int i = 0 ; i < n ; i++) {
        cin >> s[i]; // Name
        cin >> a[i]; // Age
        
        as_int[s[i]] = i; // inverse of s (s[as_int[x]] = x)
    }
    
    children_of = vector<vector<int>>(n);
    for (int i = 0 ; i < n - 1 ; i++) {
        string s; cin >> s; // child
        int child = as_int[s];
        string p; cin >> p; // parent
        int parent = as_int[p];
        
        children_of[parent].push_back(child);
    }
    
    queries = vector<vector<pair<int, int>>>(n);
    for (int i = 0 ; i < q ; i++) {
        string s; cin >> s; // Species
        int species = as_int[s];
        int b; cin >> b; // Max age
        
        queries[species].push_back(make_pair(b, i));
    }
    
    path = vector<int>(0);
    int root = max_element(a.begin(), a.end()) - a.begin();
    path.push_back(root);
    result = vector<int>(q);
    
    dfs(root);
    
    for (int i = 0 ; i < q ; i++) {
        cout << s[result[i]] << " ";
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
