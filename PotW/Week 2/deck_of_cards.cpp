#include <iostream>
#include <vector>
#include <cmath>

int main() {
    int t; std::cin >> t;
    for (int tc = 0 ; tc < t ; tc ++) {
        int n; std::cin >> n;
        int k; std::cin >> k;

        std::vector<int> deck(n);
        for (int i = 0 ; i < n ; i++) {
            std::cin >> deck[i];
        }

        int first = 0;
        int second = 0;
        int sum = deck.at(first);
        int diff = std::abs(k - sum);
        int first_current = first;
        int second_current = second;
        bool safe = true;
        while (sum != k && safe) {
            if (sum < k) {
                second++;
                if (second >= n) {
                    // Impossible.
                    safe = false;
                } else {
                    sum += deck.at(second);
                }
            } else if (sum > k) {
                sum -= deck.at(first);
                first++;
            }

            int diff_current = std::abs(k - sum);
            if (diff_current < diff) {
                // Closer
                diff = diff_current;
                first_current = first;
                second_current = second;
            }
        }

        if (sum == k) {
            first_current = first;
            second_current = second;
        }
        
        std::cout << first_current << " " << second_current << std::endl;

    }

    return 0;
}
