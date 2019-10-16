#include <iostream>
#include <vector>

using namespace std;

vector<int> v;

int play(bool my_turn, vector<int> &coins, int i, int j) {
    if (i == j + 1) {
        return 0;
    } else {
        if (my_turn) {
            return max(
                coins[i] + play(false, coins, i + 1, j),
                coins[j] + play(false, coins, i, j - 1)
            );
        } else {
            return min(
                play(true, coins, i + 1, j),
                play(true, coins, i, j - 1)
            );
        }
    }
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

        cout << play(true, v, 0, n - 1) << endl;
    }

    return 0;
}