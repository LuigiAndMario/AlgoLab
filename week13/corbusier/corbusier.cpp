#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> memo;

bool possible(vector<int> &disks, int n, int cur, int h, int i, int k, bool already_placed) {
    // End of recursion
    if (((h % k) == i) && already_placed) {
        return true;
    } else if (cur == n) {
        return false;
    }
    
    // If it is already precomputed
    if (memo[cur][h] != -1) {
        return memo[cur][h];
    }
    
    // Actual algorithm
    bool with = possible(disks, n, cur + 1, (h + disks[cur]) % k, i, k, true);
    bool without = possible(disks, n, cur + 1, h, i, k, already_placed);
    
    memo[cur][h] = with || without ? 1 : 0;
    return with || without;
}

void testcase() {
    int n; cin >> n; // # disks
    int i; cin >> i; // Parameter 1 of the modulor
    int k; cin >> k; // parameter 2 of the modulor
    i %= k;
    
    vector<int> disks(n);
    for (int j = 0 ; j < n ; j++) {
        int h; cin >> h;
        
        disks[j] = (h % k);
    }
    // Sorting disks by descending order (largest first)
    sort(disks.begin(), disks.end());
    reverse(disks.begin(), disks.end());
    
    memo = vector<vector<int>>(n, vector<int>(k, -1));
    
    cout << (possible(disks, n, 0, 0, i, k, false) ? "yes" : "no") << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
