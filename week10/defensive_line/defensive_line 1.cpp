#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> v;
vector<pair<int, int> > intervals;

int strategy(int value, int num_intervals, int cur, int attackers, int b_prev) {
    if (attackers == num_intervals) {
        // We used all of our attackers
        return value;
    }
    
    if (cur == intervals.size()) {
        // Reached the last interval
        if (num_intervals < attackers) {
            // There are fewer intervals than attackers -> some attackers slack off -> fail
            return -1;
        }
        return value;
    }
    
    pair<int, int> interval_cur = intervals[cur];
    int a_cur = interval_cur.first;
    int b_cur = interval_cur.second;
    
    if (a_cur > b_prev) {
        // No overlap between this and the previous interval
        // -> we can add it and move on
        // Or we can leave it and move on
        int with = strategy(value + b_cur - a_cur + 1, num_intervals + 1, cur + 1, attackers, b_cur);
        int without = strategy(value, num_intervals, cur + 1, attackers, b_prev);
        return max(with, without);
    } else {
        // We cannot add this interval - we move onto the next
        int without = strategy(value, num_intervals, cur + 1, attackers, b_prev);
        return without;
    }
}

void testcase() {
    int n; cin >> n; // # defenders
    int m; cin >> m; // # attackers
    int k; cin >> k; // Attack strength
    
    v = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> v[i]; // Defense value of ith defender
    }
    
    // Computing all the intervals of defenders that we can attack
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
    
    if (intervals.size() < m) {
        // There are fewer intervals than attackers -> some attackers slack off -> fail
        cout << "fail" << endl;
        return;
    }
    
    value = strategy(0, 0, 0, m, -1);
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
