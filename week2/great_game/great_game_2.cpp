#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int play(int from, int turns, vector<vector<int> > &moves, vector<vector<int> > &t) {
    // This returns the number of turns left until the meeple reaches the end
    // No need to specify which meeple here - we run it once for Sherlock and once for Moriarty
    if (from == t.size() - 1) {
        // Done
        return turns;
    }

    if (moves[turns % 2][from] != -1) {
        return moves[turns % 2][from] + turns; 
        // Without adding the turns, we simply have the number of moves from this position,
        // but without counting the number of moves it took us to get there!
    }

    int path;
    if (turns % 2 == 0) {
        path = INT_MAX;
        for (int i = 0 ; i < t[from].size() ; i++) {
            path = min(path, play(t[from][i], turns + 1, moves, t));
        }
        // cerr << "From " << from << ", the fastest path takes " << pathpath - turns << " moves" << endl;
    } else {
        path = 0;
        for (int i = 0 ; i < t[from].size() ; i++) {
            path = max(path, play(t[from][i], turns + 1, moves, t));
        }
        // cerr << "From " << from << ", the slowest path takes " << path - turns << " moves" << endl;
    }

    moves[turns % 2][from] = path - turns; 
    // Removing the turns we did so far is needed, since this represents the number of moves from this position,
    // and the previous turns should not be counted
    return path;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        int m; cin >> m;

        int r; cin >> r; r--;
        int b; cin >> b; b--;

        vector<vector<int> > t(n, vector<int>(0)); // t[u] = all the positions reachable from u
        for (int i = 0 ; i < m ; i++) {
            int u; cin >> u; u--;
            int v; cin >> v; v--;
            t[u].push_back(v);
        }

        vector<vector<int> > moves(2, vector<int>(n - 1, -1));
        // cerr << "Moving the red meeple" << endl;
        int red = play(r, 0, moves, t);
        // cerr << "Moving the black meeple" << endl;
        int black = play(b, 0, moves, t);
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