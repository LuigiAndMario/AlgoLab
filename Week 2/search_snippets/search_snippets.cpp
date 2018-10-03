#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void testcase() {
    int n; std::cin >> n; // Number of words
    std::vector<int> m(n); // m[i] = number of times word `i` occurs
    for (int i = 0 ; i < n ; i++) {
        std::cin >> m[i];
    }

    std::set<std::pair<int, int> > p_set;
    for (int i = 0 ; i < n ; i++) {
        int m_i = m[i];
        for (int j = 0 ; j < m_i ; j++) {
            int p_i_j; std::cin >> p_i_j;
            p_set.insert(std::make_pair(p_i_j, i));
        }
    }
    std::vector<std::pair<int, int> > p(p_set.begin(), p_set.end()); // p[i, j] = `j`th occurence of `i`th word

    std::vector<int> sliding_window(n);
    int a = 0; // [a, b] is the interval
    int b = 0;
    int minimal_interval = p.back().first - p.front().first + 1; // minimal_interval is initialised at p[last] - p[first] + 1
    sliding_window[p[0].second] = 1;

    int included = 1;
    bool safe = true;
    while (b < p.size() && safe) {
        if (included < n) {
            if (++b == p.size()) {
                // reached the end
                safe = false;
            } else {
                // We expand the sliding window on the right.
                int after = sliding_window[p[b].second]++;
                if (after == 0) {
                    included++;
                }
            }
        } else {
            // All the words are included. New candidate for the minimal interval.
            int res = p[b].first - p[a].first + 1;
            if (res < minimal_interval) {
                minimal_interval = res;
            }
            // We shrink the sliding window from the left.
            int before = --sliding_window[p[a].second];
            if (before == 0) {
                included--;
            }
            a++;
        }
    }
    std::cout << minimal_interval << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }

    return 0;
}
