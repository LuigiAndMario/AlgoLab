#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>

using namespace std;

void testcase() {
    int n; cin >> n; // # friends
    int m; cin >> m; // # books (too large for flow)

    vector<int> s(n); // Strength
    for (int i = 0 ; i < n ; i++) {
        cin >> s[i];
    }
    random_shuffle(s.begin(), s.end());
    sort(s.begin(), s.end());
    reverse(s.begin(), s.end());
    
    multiset<int, greater<int> > w;
    for (int i = 0 ; i < m ; i++) {
        int wi; cin >> wi;
        w.insert(wi);
    }
    
    if (*w.begin() > s[0]) {
        cout << "impossible" << endl;
        return;
    }
    
    int trips = 0;
    while (!w.empty()) { // As long as there still are boxes to move
        trips++;
        for (int i = 0 ; i < n ; i++) { // Find the heaviest box anyone can lift, and lift it
            auto box = w.lower_bound(s[i]);
            if (box != w.end()) {
                // There is a box that this friend can lift
                w.erase(box);
            } else {
                // Given that every next friend is weaker, there won't be any box they can lift
                break;
            }
        }
    }
    
    cout << (2 * trips) + (trips - 1) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
