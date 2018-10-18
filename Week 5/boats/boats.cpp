#include <iostream>
#include <vector>
#include <algorithm>

bool ring_closer(std::pair<int, int> boat_1, std::pair<int, int> boat_2) {
    return boat_1.second < boat_2.second;
}

void testcase() {
    int n; std::cin >> n; // # of wizards
    std::vector<std::pair<int, int> > boats(n); // boats[i] = <length of boat, assigned ring> for wizard `i`
    for (int i = 0 ; i < n ; i++) {
        int l_i; std::cin >> l_i;
        int p_i; std::cin >> p_i;
        boats[i] = std::make_pair(l_i, p_i);
    }

    // The idea is to see the boats as tasks that need to be scheduled, with deadline p_i + l_i, arrival time p_i - l_i and execution time l_i
    // Only there is no precedence, andwe want to minimise the number of deadline misses  instead of theri total apmlitude
    std::random_shuffle(boats.begin(), boats.end());
    std::sort(boats.begin(), boats.end(), ring_closer);

    int boats_placed = 0;
    int end_of_line = boats[0].second - boats[0].first;
    int end_before_previous_boat = end_of_line;
    for (int i = 0 ; i < n ; i++) {
        int l_i = boats[i].first; // length of the boat
        int r_i = boats[i].second; // position of the ring

        if (r_i >= end_of_line) {
            // The line does not end after to ring, so the current boat can still be placed
            end_before_previous_boat = end_of_line;
            boats_placed++;
            end_of_line = std::max(r_i, end_of_line + l_i);
        } else {
            // What if we replaced the previous boat with this one ?
            int potential_alternative = std::max(r_i, l_i + end_before_previous_boat);
            if (potential_alternative < end_of_line) {
                // If the line ends earlier with this boat than with the previous one, we swap them
                end_of_line = potential_alternative;
            }
        }
    }

    std::cout << boats_placed << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
