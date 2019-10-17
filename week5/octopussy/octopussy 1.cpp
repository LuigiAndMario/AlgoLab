#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> bomb;
vector<bool> defused;

bool more_bombs() {
    for (auto it = defused.begin() ; it != defused.end() ; it++) {
        if (!*it) {
            return true;
        }
    }
    return false;
}

bool children_defused(int i) {
    if (i >= (bomb.size() - 1) / 2) {
        // Bottom bomb - no children
        return true;
    }
    
    return defused[2*i + 1] && defused[2*i + 2];
}

void testcase() {
    int n; cin >> n; // assumed to be odd (as per the description)
    
    bomb = vector<int>(n);
    cin >> bomb[0]; // This bomb has no parent bomb, so no time to propagate
    for (int i = 1 ; i < n ; i++) {
        int parent = i % 2 == 0 ? (i - 2) / 2 : (i - 1) / 2;
        int ti; cin >> ti;
        bomb[i] = min(bomb[parent] - 1, ti);
    }
    
    defused = vector<bool>(n, false);
    int time = 0;
    while (more_bombs()) {
        int soonest = INT_MAX;
        int next;
        for (int i = 0 ; i < n ; i++) {
            if (!defused[i] && children_defused(i)) {
                if (bomb[i] < soonest) {
                    soonest = bomb[i];
                    next = i;
                }
            }
        }
        
        if (soonest <= time) {
            // This bomb already exploded
            cout << "no" << endl;
            return;
        }
        
        defused[next] = true;
        time++;
    }
    
    cout << "yes" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
