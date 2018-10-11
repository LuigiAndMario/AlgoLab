#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

void testcase() {
    int n; std::cin >> n; // Number of parasols
    std::vector<int> x(n); // Parasol positions
    for (int i = 0 ; i < n ; i++) {
        std::cin >> x[i];
    }
    
    std::random_shuffle(x.begin(), x.end()); // Precaution against potential adversarial input
    std::sort(x.begin(), x.end()); // Sorting the parasols by increasing positions

    int max_parasols = 0;
    int sum_parasols = 0;
    std::vector<std::pair<int, int> > optimal_ranges;

    int start = 0;
    int end = 0;

    for (int i = 0 ; i < n ; i++) {
        while (x[i] - x[start] > 200) { 
            // Moving the start index to the right as long as we are out of range.
            start++;
            sum_parasols--;
        }
        end = i;
        sum_parasols++;

        if (sum_parasols >= max_parasols) {
            if (sum_parasols > max_parasols) {
                max_parasols = sum_parasols;
                // Flushing the previous ranges - a better one was found
                optimal_ranges = std::vector<std::pair<int, int> >();
            }

            optimal_ranges.push_back(std::make_pair(start, end));
        }
    }

    std::vector<int> bars;
    int d_min = (x[optimal_ranges[0].second] - x[optimal_ranges[0].first] + 1) / 2;
    for (int i = 0 ; i < optimal_ranges.size() ; i++) {
        int a = x[optimal_ranges[i].first]; // [a, b] is the interval in which the bar is
        int b = x[optimal_ranges[i].second];
        
        int range = (b - a + 1) / 2;
        if (range <= d_min) {
            if (range < d_min) {
                d_min = range;
                bars = std::vector<int>();
            }

            if ((a + b) % 2 == 0) {
                bars.push_back((a + b) / 2);
            } else {
                bars.push_back((a + b - 1) / 2);
                bars.push_back((a + b - 1) / 2 + 1);
            }
        }
    }

    std::sort(bars.begin(), bars.end());

    std::cout << max_parasols << " " << d_min << std::endl;
    for (int i = 0 ; i < bars.size() ; i++) {
        std::cout << bars[i] << (i == bars.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}





hbsenia
niop








wnadiw
