#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> a, b;
vector<int> a_sum, b_sum; // [i] -> sum of all the volumes/weights from the top down to i (included)
vector<vector<int>> memo; // [a_cur][b_cur] -> cost

void testcase() {
    int n; cin >> n; // # items
    
    a = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> a[i];
    }
    
    b = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> b[i];
    }
    
    // Precomputing the sums
    a_sum = vector<int>(n + 1, 0);
    b_sum = vector<int>(n + 1, 0);
    for (int i = n - 1 ; i >= 0 ; i--) {
        a_sum[i] = a_sum[i + 1] + a[i];
        b_sum[i] = b_sum[i + 1] + b[i];
    }
    
    memo = vector<vector<int>>(n + 1, vector<int>(n + 1, -1));
    memo[0][0] = 0;
    
    for (int i = 1 ; i < n + 1 ; i++) {
        for (int j = 1 ; j < n + 1 ; j++) {
            int best = INT_MAX;
            
            // First, we compute the result if we take only one single a (starting from i), and any number of bs (starting from j)
            int taken_a = 1;
            int cost_a = (a_sum[i - taken_a] - a_sum[i]) - taken_a; // sum costs of as taken - number of as taken
            
            for (int k = 1 ; k <= j ; k++) {
                int taken_b = k;
                int val = memo[i - taken_a][j - taken_b];
                if (val != -1) {
                    int cost_b = (b_sum[j - taken_b] - b_sum[j]) - taken_b; // sum costs of bs taken - number of bs taken
                    int cost = val + (cost_a * cost_b);
                    best = min(best, cost);
                }
            }
            
            // Then, we compute the result if we take any number of as (starting from i), and only one single b (starting from j)
            int taken_b = 1;
            int cost_b = (b_sum[j - taken_b] - b_sum[j]) - taken_b;
            
            for (int k = 1 ; k <= i ; k++) {
                int taken_a = k;
                int val = memo[i - taken_a][j - taken_b];
                if (val != -1) {
                    int cost_a = (a_sum[i - taken_a] - a_sum[i]) - taken_a;
                    int cost = val + (cost_a * cost_b);
                    best = min(best, cost);
                }
            }
            
            memo[i][j] = best;
        }
    }
    
    cout << memo[n][n] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
