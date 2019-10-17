#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct Boat {
    int l;
    int p;
} Boat;

bool ascending_p(Boat b1, Boat b2) {
    return b1.p < b2.p;
}

void testcase() {
    int n; cin >> n;
    
    vector<Boat> boat(n);
    for (int i = 0 ; i < n ; i++) {
        int li; cin >> li;
        int pi; cin >> pi;
        boat[i] = { li, pi };
    }
    
    sort(boat.begin(), boat.end(), ascending_p);
    
    int current_end = boat[0].p - boat[0].l;
    int previous_end = current_end;
    int boats = 0;
    for (int i = 0 ; i < n ; i++) {
        int li = boat[i].l;
        int pi = boat[i].p;
        
        if (current_end <= pi) {
            // There is enough space between the previous one and the this boat's ring to place it
            boats++;
            previous_end = current_end;
            current_end = max(current_end + li, pi);
        } else {
            // As things are now, we cannot place this boat - but replacing the previous one with this one may yield better results
            // Note that here, "better results" only mean more space for the next boat(s)
            int alternative_end = max(previous_end + li, pi);
            current_end = min(alternative_end, current_end);
        }
    }
    
    cout << boats << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
