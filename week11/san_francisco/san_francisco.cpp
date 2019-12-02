#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<vector<pair<int, long>>> board;
vector<vector<long>> memo; // [moves left after this one][position] -> highest possible score

void testcase() {
    int n; cin >> n; // # holes
    int m; cin >> m; // # canals
    long x; cin >> x; // Target score
    int k; cin >> k; // Max moves allowed
    
    board = vector<vector<pair<int, long>>>(n, vector<pair<int, long>>(0));
    for (int i = 0 ; i < m ; i++) {
        int u; cin >> u;
        int v; cin >> v;
        long p; cin >> p;
        
        board[u].push_back(make_pair(v, p));
    }
    
    // Going back to the start
    for (int i = 0 ; i < n ; i++) {
        if (board[i].size() == 0) {
            // Nothing can be reached from position i, so we close our eyes and imagine very very hard that it's the same thing as the start position
            board[i] = board[0];
        }
    }
    
    memo = vector<vector<long>>(k, vector<long>(n, 0)); // [moves left after this one][position] -> highest possible score
    // On the penultimate move, we simply take the canal with the best value
    for (int j = 0 ; j < n ; j++) {
        long best = -1;
        for (auto it = board[j].begin() ; it != board[j].end() ; it++) {
            long p = it->second;
            best = max(best, p);
        }
        
        memo[0][j] = best;
    }
    
    // In general, with i moves left after this one and on position j, we find the best possible score
    for (int i = 1 ; i < k ; i++) {
        for (int j = 0 ; j < n ; j++) {
            long best = -1;
            for (auto it = board[j].begin() ; it != board[j].end() ; it++) {
                int to = it->first;
                long p = it->second;
                best = max(best, p + memo[i - 1][to]);
            }
            
            memo[i][j] = best;
            
            if (j == 0 && best >= x) {
                // We reached the target score
                cout << i + 1 << endl;
                return;
            }
        }
    }
    
    // We haven't ever reached a target score, so we stick it to the chief
    cout << "Impossible" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
