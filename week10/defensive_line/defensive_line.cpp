#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> v;
vector<int> sums; // [i] -> j, where the sum of the strengths from i to j is k (-1 if there is no such j)
vector<vector<int>> memo; // [attackers_left][from] -> value

int strategy(int from, int attackers_left, int n) {
    // Corner cases
    if (from == n) {
        // Empty interval -> we reached the end without using all the attackers -> fail
        return -1;
    }
    
    // Already computed
    if (memo[attackers_left][from] != -2) {
        return memo[attackers_left][from];
    }
    
    // One single attacker left
    if (attackers_left == 1) {
        if (from == n - 1) {
            // Only one player left to attack - we can either attack him, or not
            return sums[from] == -1 ? -1 : 1;
        }
        
        int val = sums[from] - from + 1;
        int without = strategy(from + 1, attackers_left, n);
        memo[attackers_left][from] = max(val, without);
        return memo[attackers_left][from];
    }
    
    // Actual algorithm
    int best = -1;
    for (int i = from ; i < n ; i++) {
        // We try to start at every attacker starting from `from`
        if (sums[i] == -1) {
            // No interval starting here
            continue;
        }
        
        int val = sums[i] - i + 1; // The number of players from i to j
        int with = strategy(sums[i] + 1, attackers_left - 1, n);
            
        if (with != -1) { // Otherwise, we can't attack this interval
            best = max(best, val + with);
        }
    }
    
    memo[attackers_left][from] = best;
    return best;
}

void testcase() {
    int n; cin >> n; // # defenders
    int m; cin >> m; // # attackers
    int k; cin >> k; // Attack strength
    
    v = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> v[i]; // Defense value of ith defender
    }
    
    // Precomputing the intervals
    sums = vector<int>(n, -1);
    int a = 0; int b = 0;
    int value = v[a];
    int intervals = 0;
    
    while (b != n) { // As long as there still are defenders to attack
        if (value == k) {
            // Found an interval of value k from a to b
            sums[a] = b;
            intervals++;
        }
        
        if (value <= k) {
            // Expand to the right
            b++;
            value += v[b];
        } else {
            // Shrink to the left
            value -= v[a];
            a++;
        }
    }
    
    // Computing the best solution (DP)
    if (intervals < m) {
        // There are fewer intervals than attackers -> some attackers slack off -> fail
        cout << "fail" << endl;
        return;
    }
    
    memo = vector<vector<int>>(m + 1, vector<int>(n, -2));
    value = strategy(0, m, n);
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
