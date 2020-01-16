#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> a;
vector<int> b;
vector<int> a_sums;
vector<int> b_sums;
vector<vector<int>> memo;

void testcase() {
    int n; cin >> n; // # parcels
    
    a = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> a[i];
        a[i]--;
    }
    
    b = vector<int>(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> b[i];
        b[i]--;
    }
    
    a_sums = vector<int>(n); // [i] = sum from j = i to n - 1 of a[j]
    b_sums = vector<int>(n); // [i] = sum from j = i to n - 1 of a[j]
    a_sums[0] = a[0];
    b_sums[0] = b[0];
    for (int i = 1 ; i < n ; i++) {
        a_sums[i] = a_sums[i - 1] + a[i];
        b_sums[i] = b_sums[i - 1] + b[i];
    }
    
    vector<vector<int>> memo(n, vector<int>(n, 0)); // [i][j] = value if there is i left on a and j left on b
    memo[0][0] = a[0] * b[0];
    
    for (int i = 1 ; i < n ; i++) {
        memo[i][0] = a_sums[i] * b[0];
        memo[0][i] = a[0] * b_sums[i];
    }
    
    for (int i = 1 ; i < n ; i++) {
        for (int j = 1 ; j < n ; j++) {
            int prev = min(memo[i - 1][j - 1], min(memo[i][j - 1], memo[i - 1][j]));
            memo[i][j] = a[i] * b[j] + prev;
        }
    }
    
    cout << memo[n - 1][n - 1] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
