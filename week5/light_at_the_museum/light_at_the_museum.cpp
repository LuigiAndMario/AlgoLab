#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <tuple>

using namespace std;

vector<int> b;
// Counterintuitive, but the pattern of (on_i, off_i) is inverted each time the switch i is flipped
vector<vector<int> > on;
vector<vector<int> > off;
vector<int> missing_in_room; // Lights that still need to be switched in room i

vector<int> missing_lights(vector<int> currently_missing, int combination) {
    // Return the lights that still need to be turned on if we flick all the switches in the combination
    vector<int> now_missing = currently_missing;
    
    for (int i = 0 ; i < (int) on.size() ; i++) {
        if ((combination & (1 << i)) != 0) {
            // If i is in the combination of switches we're trying out
            for (int j = 0 ; j < (int) b.size() ; j++) {
                // Again, counterintuitive, but we invert the pattern
                now_missing[j] += on[i][j];
                now_missing[j] -= off[i][j];
            }
        }
    }
    
    return now_missing;
}

vector<int> missing_lights(int combination) {
    // Return the lights that still need to be turned on if we flick all the switches in the combination
    int split = on.size() / 2;
    vector<int> now_missing = vector<int>(b.size(), 0);
    
    for (int i = split ; i < (int) on.size() ; i++) {
        if ((combination & (1 << (i - split))) != 0) {
            // If i is in the combination of switches we're trying out
            for (int j = 0 ; j < (int) b.size() ; j++) {
                // Again, counterintuitive, but we invert the pattern
                now_missing[j] += off[i][j];
                now_missing[j] -= on[i][j];
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
    
    missing_in_room = vector<int>(m, 0);
    for (int i = 0 ; i < m ; i++) {
        missing_in_room[i] = b[i] - on_in_room[i];
    }
    
    // IDEA: Do split and list
    int flicks = INT_MAX;
    int split = n / 2;
    vector<tuple<vector<int>, int> > S1(1 << split); // combination -> lights left to turn on, number of flicks
    
    for (int i = 0 ; i < (1 << split) ; i++) {
        // i = every combination of switches
        int flicks_for_i = bits_set(i);
        vector<int> still_needed = missing_lights(missing_in_room, i);
        S1[i] = make_tuple(still_needed, flicks_for_i);
    }
    sort(S1.begin(), S1.end()); // This sorts by lights still needed AND by flicks
    
    // We don't actually use S2, we simply set it to the first values of S1 (i.e. the lights that are missing in every room)
    vector<vector<int> > S2(1 << split); // combination -> lights left to turn on
    for (int i = 0 ; i < (1 << split) ; i++) {
        S2[i] = get<0>(S1[i]);
    }
    
    for (int i = 0 ; i < (1 << (n - split)) ; i++) {
        int flicks_for_i = bits_set(i);
        vector<int> still_needed = missing_lights(i);
        
        // I was about to do all of that manually, but then found out that you can just let c++ compare the two vectors element by element - ain't that nifty!
        
        // We search for a combination (in S2, which is essentially the same as S1) that would require >= lights to be turned on than this one
        auto fewest_lights_needed = lower_bound(S2.begin(), S2.end(), still_needed);
        if (fewest_lights_needed != S2.end() && *fewest_lights_needed == still_needed) {
            // If there is none (i.e. we get end()), we don't do anything
            // If the one we have is the same as this one, then it is matching (both halves balance out, i.e. we split-and-listed the shit outta this problem)
            // If it is anything else, then it doesn't balance out (we sorted S1 so we know we won't have anything better)
            int flicks_from_S1 = get<1>(S1[fewest_lights_needed - S2.begin()]);
            flicks = min(flicks, flicks_for_i + flicks_from_S1);
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
