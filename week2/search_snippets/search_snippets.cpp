#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> m(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> m[i];
        }

        set<pair<int, int> > p_set;
        for (int i = 0 ; i < n ; i++) {
            for (int j = 0 ; j < m[i] ; j++) {
                int p_ij; cin >> p_ij; // Position of the j^th occurence of word i
                p_set.insert(make_pair(p_ij, i)); // At position p_ij, we have word i
            }
        }
        vector<pair<int, int> > p(p_set.begin(), p_set.end()); // Now the vector p is orderd by ascending p_ij, which is nifty

        int a = 0;
        int b = 0;
        int size = p.back().first - p.front().first + 1; // i.e. the maximal size
        vector<int> window(n, 0); // No word occured so far
        window[p[0].second] = 1; // Except word 0, which occured once
        int words_in = 1;

        while (b < p.size()) {
            if (words_in < n) {
                // We still need to add more words, since we don't have enough of them
                if (++b == p.size()) {
                    // Reached the end
                    break;
                }
                // We expand the sliding window
                if (window[p[b].second]++ == 0) {
                    // We now have more than 0 occurences of word b - one more word in the count
                    words_in++;
                }
            } else {
                // Enough words - we shrink to see if we can do better
                int this_size = p[b].first - p[a].first + 1;
                if (this_size < size) {
                    // We just did better
                    size = this_size;
                }
                // We shrink the sliding window
                if (--window[p[a++].second] == 0) {
                    // We now have 0 occurences of word a - one fewer word in the count
                    words_in--;
                }
            }
        }

        cout << size << endl;
    }

    return 0;
}
