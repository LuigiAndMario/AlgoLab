#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> a, b;
const int FIRED = INT_MAX;

int get_cost(int val, int a_cur, int b_cur, int n) {
    // Corner cases
    if (a_cur == n && b_cur == n) {
        // We emptied both stacks, so we just return the value
        return val;
    } else if (a_cur >= n || b_cur >= n) {
        // We emptied one stack but not the other, so we get fired
        return FIRED;
    }
    
    int a_val = 0; // Value from a
    int b_val = 0; // Value from b
    int best = INT_MAX;
     
    for (int i = a_cur ; i < n ; i++) {
        a_val += (a[i] - 1);
        for (int j = b_cur ; j < n ; j++) {
            b_val += (b[j] - 1);
            int cost = val + (a_val * b_val);
            best = min(best, get_cost(cost, i + 1, j + 1, n));
        }
        b_val = 0; // "Resetting" b_val since it gets modified in the loop
     }
    
    return best;
}

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
    
    int cost = get_cost(0, 0, 0, n);
    cout << cost << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
