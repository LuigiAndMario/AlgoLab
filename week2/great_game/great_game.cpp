#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int play(int from, int moves, bool get_best, vector<vector<int> > &t) {
    // This returns the number of turns left until the meeple reaches the end
    // No need to specify which meeple here - we run it once for Sherlock and once for Moriarty
    if (from == t.size() - 1) {
        // Done
        return moves;
    }

    if (get_best) {
        int fastest_path = INT_MAX;
        for (int i = 0 ; i < t[from].size() ; i++) {
            fastest_path = min(fastest_path, play(t[from][i], moves + 1, false, t));
        }
        return fastest_path;
    } else {
        int slowest_path = 0;
        for (int i = 0 ; i < t[from].size() ; i++) {
            slowest_path = max(slowest_path, play(t[from][i], moves + 1, true, t));
        }
        return slowest_path;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        int m; cin >> m;

        int r; cin >> r; r--;
        int b; cin >> b; b--;
        // cerr << "Sherlock starts at " << r << " and Moriarty at " << b << endl;

        vector<vector<int> > t(n, vector<int>(0)); // t[u] = all the positions reachable from u
        for (int i = 0 ; i < m ; i++) {
            int u; cin >> u; u--;
            int v; cin >> v; v--;
            t[u].push_back(v);
        }

        /*
        for (int i = 0 ; i < n ; i++) {
            cerr << i << " -> ";
            for (int j = 0 ; j < t[i].size() ; j++) {
                cerr << t[i][j] << " ";
            }
            cerr << endl;
        }
        */

        int red = play(r, 0, true, t);
        int black = play(b, 0, true, t);
        // cerr << "Sherlock needs " << red << " moves and Moriarty " << black << endl;

        if (red < black) {
            // The red finishes first -> Sherlock wins
            cout << 0 << endl;
        } else if (red > black) {
            // The black finishes first -> Moriarty wins
            cout << 1 << endl;
        } else {
            cout << 1 - (red % 2) << endl;
        }
    }
    return 0;
}