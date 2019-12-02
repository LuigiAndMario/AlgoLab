#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<vector<pair<int, int>>> board;

int get_moves(int moves, int k, int target_score, int pos) {
    // Corner cases
    if (target_score <= 0) {
        // We reached the target score
        return moves;
    }
    if (moves == k) {
        // We did our k moves without reaching the target score
        return INT_MAX;
    }
    
    int best = INT_MAX;
    for (auto it = board[pos].begin() ; it != board[pos].end() ; it++) {
        int to = it->first;
        int p = it->second;
        best = min(best, get_moves(moves + 1, k, target_score - p, to));
    }
    
    return best;
}

void testcase() {
    int n; cin >> n; // # holes
    int m; cin >> m; // # canals
    int x; cin >> x; // Target score
    int k; cin >> k; // Max moves allowed
    
    board = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(0));
    for (int i = 0 ; i < m ; i++) {
        int u; cin >> u;
        int v; cin >> v;
        int p; cin >> p;
        
        board[u].push_back(make_pair(v, p));
    }
    
    // Going back to the start
    for (int i = 0 ; i < n ; i++) {
        if (board[i].size() == 0) {
            // Nothing can be reached from position i, so we close our eyes and imagine very very hard that it's the same thing as the start position
            board[i] = board[0];
        }
    }
    
    int moves = get_moves(0, k, x, 0);
    if (moves > k) {
        cout << "Impossible" << endl;
    } else {
        cout << moves << endl;
    }
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
