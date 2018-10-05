#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

double dee_pee(std::vector<double> &p, int day, int k_today, int m, int n, std::vector<std::vector<double> > *memo) {
    if (k_today >= m) {
        // You done, fam!
        return 1;
    }
    if (k_today == 0 || day == n) {
        // You ruined, fam!
        return 0;
    }

    double p_today = p[day];
    int tomorrow = day + 1;
    if (memo->at(day)[0] == -1) {
        memo->at(day)[0] = dee_pee(p, tomorrow, k_today, m, n, memo);
    }
    double max = memo->at(day)[0];

    for (int i = 1 ; i <= k_today ; i++) {
        if (memo->at(day)[i] == -1) {
            memo->at(day)[i] = p_today * dee_pee(p, tomorrow, k_today + i, m, n, memo) +
                (1 - p_today) * dee_pee(p, tomorrow, k_today - i, m, n, memo);
        }
        max = std::max(max, memo->at(day)[i]);
    }

    return max;
}

// Dis shit cuz dis ain't dee-pee, fam!
double do_the_thing(std::vector<double> &p, int day, int k_today, int m, int n) {
    if (k_today >= m) {
        // You done, fam!
        return 1;
    }
    if (k_today == 0 || day == n) {
        // You ruined, fam!
        return 0;
    }

    double p_today = p[day];
    int tomorrow = day + 1;
    double max = do_the_thing(p, tomorrow, k_today, m, n);

    for (int i = 1 ; i <= k_today ; i++) {
        max = std::max(max,
                p_today * do_the_thing(p, tomorrow, k_today + i, m, n) +
                (1 - p_today) * do_the_thing(p, tomorrow, k_today - i, m, n)
                );
    }

    return max;
}

void testcase() {
    int n; std::cin >> n; // Number of days
    int k; std::cin >> k; // Initial wealth
    int m; std::cin >> m; // Target wealth

    std::vector<double> p(n);
    for (int i = 0 ; i < n ; i++) {
        std::cin >> p[i];
    }

    std::vector<std::vector<double> > memo(n, std::vector<double>(m, -1));

    std::cout << std::fixed << std::setprecision(5);
    std::cout << dee_pee(p, 0, k, m, n, &memo) << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase() ;
    }

    return 0;
}
