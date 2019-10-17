#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

struct Bomb {
    int i;
    int t;
    bool left_defused;
    bool right_defused;
};

vector<Bomb> bomb;

struct by_time {
    bool operator() (Bomb b1, Bomb b2) {
        return b1.t > b2.t;
    }
};

int parent_of(Bomb b) {
    if (b.i == 0) {
        return 0; // No parent
    }
    
    return (b.i - (b.i % 2 == 0 ? 2 : 1)) / 2;
}

void testcase() {
    int n; cin >> n; // assumed to be odd (as per the description)
    
    bomb = vector<Bomb>(n);
    int t0; cin >> t0;
    bomb[0] = {0, t0, false, false}; // This bomb has no parent bomb, so no time to propagate
    for (int i = 1 ; i < n ; i++) {
        int parent = i % 2 == 0 ? (i - 2) / 2 : (i - 1) / 2;
        int ti; cin >> ti;
        bomb[i] = {i, min(bomb[parent].t - 1, ti), false, false};
    }
    
    priority_queue<Bomb, vector<Bomb>, by_time> to_defuse;
    for (int i = (n - 1) / 2 ; i < n ; i++) {
        to_defuse.push(bomb[i]);
    }
    
    int time = 0;
    while (!to_defuse.empty()) {
        Bomb next = to_defuse.top();
        to_defuse.pop();
        
        if (next.t <= time) {
            // This bomb already exploded
            cout << "no" << endl;
            return;
        }
        
        // We mark this child as defused in the parent bomb
        int parent = parent_of(next);
        if (parent != 0) {
            if (next.i % 2 == 0) {
                bomb[parent].right_defused = true;
            } else {
                bomb[parent].left_defused = true;
            }
            
            if (bomb[parent].right_defused && bomb[parent].left_defused) {
                to_defuse.push(bomb[parent]);
            }
        }
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
