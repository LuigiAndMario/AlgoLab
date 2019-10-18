#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> b;
// Counterintuitive, but the pattern of (on_i, off_i) is inverted each time the switch i is flipped
vector<vector<int> > on;
vector<vector<int> > off;
vector<int> missing_in_room; // Lights that still need to be switched in room i

vector<int> missing_lights(vector<int> currently_missing, int combination) {
    // Return the lights that still need to be turned on if we flick all the switches in the combination
    vector<int> now_missing = vector<int>(currently_missing);
    for (int i = 0 ; i < on.size() ; i++) {
        if ((combination & (1 << i)) != 0) {
            // If i is in the combination of switches we're trying out
            for (int j = 0 ; j < b.size() ; j++) {
                // Again, counterintuitive, but we invert the pattern
                now_missing[j] += on[i][j];
                now_missing[j] -= off[i][j];
            }
        }
    }
    
    return now_missing;
}

int bits_set(int i) {
    int count = 0;
    while (i != 0) {
        count += (i & 1);
        i >>= 1;
    }
    
    return count;
}

bool all_zeroes(vector<int> &vec) {
    for (auto it = vec.begin() ; it != vec.end() ; it++) {
        if (*it) {
            return false;
        }
    }
    return true;
}

void testcase() {
    int n; cin >> n; // Switches
    int m; cin >> m; // Rooms
    
    b = vector<int>(m);
    for (int i = 0 ; i < m ; i++) {
        cin >> b[i];
    }
    
    vector<int> on_in_room(m, 0);
    on = vector<vector<int> >(n, vector<int>(m));
    off = vector<vector<int> >(n, vector<int>(m));
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            cin >> on[i][j];
            cin >> off[i][j];
            
            on_in_room[j] += on[i][j];
        }
    }
    
    missing_in_room = vector<int>(b);
    for (int i = 0 ; i < m ; i++) {
        missing_in_room[i] -= on_in_room[i];
    }
    
    int flicks = INT_MAX;
    for (int i = 0 ; i < (1 << n) ; i++) {
        // i = every combination of switches
        int flicks_for_i = bits_set(i);
        vector<int> after_flicking = missing_lights(missing_in_room, i);
        if (all_zeroes(after_flicking)) {
            flicks = min(flicks, flicks_for_i);
        }
    }
    
    if (flicks != INT_MAX) {
        cout << flicks << endl;
    } else {
        cout << "impossible" << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}
