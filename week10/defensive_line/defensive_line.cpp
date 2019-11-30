#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> v;
vector<pair<int, int> > intervals;

vector<vector<int> > memo; // prev x attackers_left -> value

int strategy(int value, int cur, int prev, int attackers_left, bool added_stuff) {
    //
    // Corner cases
    //
    if (attackers_left == 0) {
        // We used all of our attackers
        return value;
    }
    
    if (cur == intervals.size()) {
        // Reached the last interval, and there are fewer intervals than attackers -> some attackers slack off -> fail
        return -1;
    }
    
    //
    // Already computed
    //
    if (memo[prev][attackers_left] != -2) {
        return memo[prev][attackers_left];
    }
    
    //
    // Actual algorithm
    //
    pair<int, int> interval_cur = intervals[cur];
    int a_cur = interval_cur.first + 1;
    int b_cur = interval_cur.second + 1;
    int v_cur = b_cur - a_cur + 1;
    
    pair<int, int> interval_prev = intervals[prev];
    int b_prev = interval_prev.second + 1;
    
    if (!added_stuff || a_cur > b_prev) {
        // Option 1: We have not added anything just yet (so prev = 0)
        // Option 2: No overlap between this and the previous interval
        // -> We can add it (prev <- cur, attackers - 1) or leave it (prev <- prev), and move on to the next interval (cur + 1)
        int with = strategy(value + v_cur, cur + 1, cur, attackers_left - 1, true);
        int without = strategy(value, cur + 1, prev, attackers_left, added_stuff);
        memo[prev][attackers_left] = max(with, without);
    } else {
        // We cannot add this interval - we move onto the next
        int without = strategy(value, cur + 1, prev, attackers_left, added_stuff);
        memo[prev][attackers_left] = without;
    }
    
    return memo[prev][attackers_left];
}

void testcase() {
    int n; cin >> n; // # defenders
    int m; cin >> m; // # attackers
    int k; cin >> k; // Attack strength
    
    v = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> v[i]; // Defense value of ith defender
    }
    
    //
    // Computing all the intervals of defenders that we can attack (sliding window)
    //
    intervals = vector<pair<int, int> >(0);
    int a = 0; int b = 0;
    int value = v[a];
    
    while (b != n) { // As long as there still are defenders to attack
        if (value == k) {
            // Found an interval of length k - we add it
            intervals.push_back(make_pair(a, b));
            // Then we expand to the left
            value -= v[a];
            a++;
            if (a > b) { // And expand to the right if this was a one-defender interval
                b++;
                value += v[b];
            }
        } else if (value > k) {
            // Too much defense power - we expand to the left
            value -= v[a];
            a++;
            if (a > b) { // And expand to the right if this was a one-defender interval
                b++;
                value += v[b];
            }
        } else {
            // Not enough defense power - we expand to the right
            b++;
            value += v[b];
        }
    }
    
    //
    // Computing the best solution (DP)
    //
    if (intervals.size() < m) {
        // There are fewer intervals than attackers -> some attackers slack off -> fail
        cout << "fail" << endl;
        return;
    }
    
    memo = vector<vector<int> >(intervals.size(), vector<int>(m + 1, -2));
    value = strategy(0, 0, 0, m, false);
    if (value == -1) {
        cout << "fail" << endl;
    } else {
        cout << value << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
