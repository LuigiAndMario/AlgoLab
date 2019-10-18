#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> b;
vector<vector<int> > on;
vector<vector<int> > off;

int solve(int current_switch, vector<int> on_in_room, int flicks, int n, int m) {
    if (current_switch == n) {
        for (int i = 0 ; i < m ; i++) {
            if (on_in_room[i] != b[i]) {
                return INT_MAX;
            }
        }
        return flicks;
    }
    
    int no_flicking = solve(current_switch + 1, on_in_room, flicks, n, m);
    for (int j = 0 ; j < m ; j++) {
        // Counterintuitive, but given that flicking a switch reverses the pattern, we remove the lights that it would turn on, and add those that it would turn off
        on_in_room[j] = max(0, on_in_room[j] - on[current_switch][j]);
        on_in_room[j] += off[current_switch][j];
    }
    int flicking = solve(current_switch + 1, on_in_room, flicks + 1, n, m);
    
    return min(flicking, no_flicking);
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
    
    // First testset assumption: m, n <= 15
    // i.e. be as slow as you want
    int flicks = solve(0, on_in_room, 0, n, m);
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
