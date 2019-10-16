#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> x;
vector<pair<int, int> > ranges;

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        x = vector<int>(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> x[i];
        }
        sort(x.begin(), x.end());

        //
        // Sliding window to find all the optimal bar locations
        //
        ranges = vector<pair<int, int> >();
        int a = 0;
        int b = 0;
        int this_p = 0; // number of parasols in this window
        int best_p = 0; // best number of parasols

        for (int i = 0 ; i < n ; i++) { // i = current end of window
            while (x[i] - x[a] > 200) {
                // Too big a distance
                a++;
                this_p--;
            }

            b = i;
            this_p++;

            if (this_p > best_p) {
                // Everything we had then is trash, this here is the real deal
                best_p = this_p;

                ranges = vector<pair<int, int> >();
                ranges.push_back(make_pair(a, b));
            } else if (this_p == best_p) {
                // Just another optimal location
                ranges.push_back(make_pair(a, b));
            }
        }

        //
        // Finding the shortest distance, and sorting by best to worst
        //
        a = x[ranges[0].first];
        b = x[ranges[0].second];
        int size = b - a + 1;
        int d_max_min = size / 2; // Minimum of the longest distance (so we want to MINimise it)
        // size / 2 it's the length to the farthest parasols in the interval
        vector<int> x_bar(0); // Only the optimum locations
        for (int i = 0 ; i < ranges.size(); i++) {
            a = x[ranges[i].first];
            b = x[ranges[i].second];
            size = (b - a + 1) / 2;

            if (size < d_max_min) {
                // Everything we had then is trash, this here is the real deal
                d_max_min = size;
                x_bar = vector<int>(0);

                if ((a + b) % 2 == 0) {
                    x_bar.push_back((a + b) / 2);
                } else {
                    x_bar.push_back((a + b - 1) / 2);
                    x_bar.push_back((a + b - 1) / 2 + 1);
                }
            } else if (size == d_max_min) {
                // Just another optimal location
                if ((a + b) % 2 == 0) {
                    x_bar.push_back((a + b) / 2);
                } else {
                    x_bar.push_back((a + b - 1) / 2);
                    x_bar.push_back((a + b - 1) / 2 + 1);
                }
            }
        }

        sort(x_bar.begin(), x_bar.end());


        cout << best_p << " " << d_max_min << endl;
        for (int i = 0 ; i < x_bar.size() ; i++) {
            cout << x_bar[i] << " ";
        }
        cout << endl;
        
    }

    return 0;
}