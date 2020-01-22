#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<bool> memo;

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
    
    memo = vector<bool>(k, false);
    for (int d = 0 ; d < n ; d++) { // d the current disk
        vector<bool> reachable_heights(k, false);
        for (int h = 0 ; h < k ; h++) { // h the current reachable height
            reachable_heights[(h + disks[d]) % k] = memo[h]; // true only if h is already reachable
        }
        for (int h = 0 ; h < k ; h++) {
            memo[h] = memo[h] || reachable_heights[h];
        }
        
        memo[disks[d]] = true; // Because we can also simply not have anything and place this disk
    }
    
    cout << (memo[i] ? "yes" : "no") << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
