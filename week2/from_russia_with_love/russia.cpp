#include <iostream>
#include <vector>

using namespace std;

vector<int> x;
vector<vector<vector<int> > > gains;

int play(int a, int b, int k, int turn, int m) {
    // a, b the left (resp. right) position in x
    if (a == b) {
        return turn == k ? x[a] : 0;
    }

    if (a > b || b < 0) {
        return 0;
    }

    if (gains[turn == k][a][b] != -1) {
        return gains[turn == k][a][b];
    }

    if (k == turn) {
        int take_first = x[a] + play(a + 1, b, k, (turn + 1) % m, m);
        int take_last = x[b] + play(a, b - 1, k, (turn + 1) % m, m);
	
        gains[turn == k][a][b] = max(take_first, take_last);
    } else {
        int take_first = play(a + 1, b, k, (turn + 1) % m, m);
        int take_last = play(a, b - 1, k, (turn + 1) % m, m);
	
        gains[turn == k][a][b] = min(take_first, take_last);
    }
	
    return gains[turn == k][a][b];
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        int m; cin >> m;
        int k; cin >> k;

        x = vector<int>(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> x[i];
        }

        gains = vector<vector<vector<int> > >(2, vector<vector<int> >(n, vector<int>(n, -1)));
        cout << play(0, n - 1, k, 0, m) << endl;
    }
    return 0;
}
