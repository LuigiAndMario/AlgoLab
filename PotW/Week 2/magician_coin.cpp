#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

bool sort_by_first(std::pair<double, int> a, std::pair<double, int> b) {
    return a.first >= b.first;
}

void testcase() {
    int n; std::cin >> n; // Number of days
    int k; std::cin >> k; // Initial wealth
    int m; std::cin >> m; // Target wealth

    std::vector<double> p(n); // Probabilities of winning, per day
    for (int i = 0 ; i < n ; i++) {
        std::cin >> p[i];
    }

    // BEST STRATEGIES
    // Sorting the days by decreasing winning probability
    std::vector<std::pair<double, int> > prob_day(n);
    for (int i = 0 ; i < n ; i++) {
        prob_day[i] = std::make_pair(p[i], i);
    }
    std::sort(prob_day.begin(), prob_day.end(), sort_by_first);
    
    // Getting the best strategies.
    int potential_gain = k;
    int i = 1;
    while (potential_gain < m) {
        potential_gain *= 2;
        i++;
    }

    // TRY & RECOVER
    // TODO
    

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "lol" << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }

    return 0;
}
