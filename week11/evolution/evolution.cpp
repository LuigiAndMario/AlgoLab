#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

vector<string> s;
vector<int> a;
map<string, int> as_int;
vector<int> parent_of;

// Actual query answering
void query(int sp, int b) {
    int cur = sp;
    int parent = parent_of[cur];
    
    if (parent == -1 || a[parent] > b) {
        // Ccur is LUCA, or the direct parent itself is too old.
        cout << s[sp] << " ";
        return;
    }
    
    while (a[parent] <= b) {
        cur = parent;
        parent = parent_of[parent];
        
        if (parent == -1) {
            // cur is LUCA
            cout << s[cur] << " ";
            return;
        }
    }
    
    cout << s[cur] << " ";
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
    
    parent_of = vector<int>(n, -1); // Every species has one and only parent
    for (int i = 0 ; i < n - 1 ; i++) {
        string s; cin >> s; // child
        int child = as_int[s];
        string p; cin >> p; // parent
        int parent = as_int[p];
        
        parent_of[child] = parent;
    }
    
    for (int i = 0 ; i < q ; i++) {
        string s; cin >> s; // Species
        int b; cin >> b; // Max age
        
        query(as_int[s], b);
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
