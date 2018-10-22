#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

std::vector<std::vector<double> > memo;

double non_rec(std::vector<double> &p, int k, int m, int n) {
    if (k >= m) {
        return 1;
    }

    memo = std::vector<std::vector<double> >(n, std::vector<double>(m, -1)); // [day, money]
    
    for (int i = 0 ; i < m ; i++) {
        memo[0][i] = k; // On the first day, we start with k
    }
    for (int i = 0 ; i < n ; i++) {
        memo[i][0] = k; // If we keep betting 0 after the start of the game, we keep having k
    }

    int current_fortune = k;

    for (int i = 1 ; i < n ; i++) {
        for (int j = 1 ; j <= current_fortune ; j++) {
            // TODO
        }
    }

    return 0;
}

double dee_pee(std::vector<double> &p, int day, int k_today, int m, int n) {
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
    if (memo[day][0] == -1) {
        memo[day][0] = dee_pee(p, tomorrow, k_today, m, n);
    }
    double max = memo[day][0];

    for (int i = 1 ; i <= k_today ; i++) {
        if (memo[day][i] == -1) {
            memo[day][i] = p_today * dee_pee(p, tomorrow, k_today + i, m, n) +
                (1 - p_today) * dee_pee(p, tomorrow, k_today - i, m, n);
        }
        max = std::max(max, memo[day][i]);
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

    memo = std::vector<std::vector<double> >(n, std::vector<double>(m, -1));

    std::cout << std::fixed << std::setprecision(5);
    std::cout << dee_pee(p, 0, k, m, n) << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase() ;
    }

    return 0;
}
