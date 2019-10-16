#include <iostream>
#include <vector>

using namespace std;

vector<int> v;

int play(vector<int> &coins, vector<vector<int> > &gains, int i, int j) {
    if (i == j) {
        // Done
        return v[i];
    }
    if (i > j || j < 0) {
        // Done too, but this time there's nothing left to be done
        return 0;
    }
    if (gains[i][j] != -1) {
        // Already computed
        return gains[i][j];
    }

    int take_first = coins[i] + min(
        play(coins, gains, i + 2, j),
        play(coins, gains, i + 1, j - 1)
    );
    int take_last = coins[j] + min(
        play(coins, gains, i + 1, j - 1),
        play(coins, gains, i, j - 2)
    );

    gains[i][j] = max(take_first, take_last);
    return gains[i][j];
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        v = vector<int>(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> v[i];
        }

        vector<vector<int> > gains(n, vector<int>(n, -1));
        cout << play(v, gains, 0, n - 1) << endl;
    }
    return 0;
}