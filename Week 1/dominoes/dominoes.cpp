#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> h(n);
    for (int i = 0 ; i < n ; i++) {
        std::cin >> h[i];
    }

    std::deque<int> reaches;
    reaches.push_back(h[0]);
    int just_pushed = 1; // start_index
    int height_index = 1; // data_index
    int height = h[0]; // end_index
    int range = 0;
    while (((unsigned int) height) > reaches.size()) {
        range = just_pushed + height - 1;

        for (std::vector<int>::iterator it = h.begin() + height_index ; it != h.begin() + range ; it++) {
            reaches.push_back(*it);
        }

        // We now look for the biggest elements in the 'reaches' deque, and get them by decreasing order.
        std::deque<int> ranked(reaches); // ranked[i] = reaches[i] + i.
        for (int i = 0 ; ((unsigned int) i) < ranked.size() ; i++) {
            ranked[i] += i;
        }
        // We get the index of the biggest element.
        std::deque<int>::iterator it_at_max = max_element(ranked.begin(), ranked.end());
        int max_index = it_at_max - ranked.begin();

        height_index = just_pushed + ranked.size() - 1;
        just_pushed += max_index;
        height = *it_at_max - max_index;
        for (int i = 0 ; i < max_index ; i++) {
            reaches.pop_front();
        }

        range = just_pushed + height - 1;
        if (range > n) {
            height = n + 1 - just_pushed;
            break;
        }
     }

    std::cout << just_pushed + height - 1 << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0 ; i < t ; i++) {
        testcase();
    }

    return 0;
}
