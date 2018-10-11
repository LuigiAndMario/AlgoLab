#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::vector<int> > memo;

int get_strategy_tr(std::vector<int> &v, std::vector<std::pair<int, int> > &mappings, int last_def_assigned, int remaining_atks, int k) {
    if (remaining_atks == 0) {
        // No more attackers left - we simply return the value of the strategy
        int sum = 0;
        for (int i = 0 ; i < mappings.size() ; i++) {
            sum += mappings[i].second - mappings[i].first + 1;
        }
        return sum;
    }

    if (remaining_atks > v.size() - last_def_assigned || last_def_assigned == v.size() - 1) {
        // More attackers than defenders OR the last assigned defender was the last one
        return -1;
    }

    int a = last_def_assigned + 1;
    int b = a;
    int defense_value = v[a];
    while (defense_value < k) {
        if (b == v.size() - 1) {
            // We reached the end without finding a viable strategy
            return -1;
        }

        // We can still attack -> expand the window and add to the defense_value
        defense_value += v[++b];
    }

    if (defense_value == k) {
        // Viable strategy - we get the max between that, and, well, not that
        std::vector<std::pair<int, int> > mappings_with(mappings.size());
        for (int i = 0 ; i < mappings.size() ; i++) {
            mappings_with[i] = (i == mappings.size() - remaining_atks ? std::make_pair(a, b) : mappings[i]);
        }

        int with = std::max(memo[remaining_atks - 1][b], get_strategy_tr(v, mappings_with, b, remaining_atks - 1, k));
        int without = std::max(memo[remaining_atks][last_def_assigned + 1], get_strategy_tr(v, mappings, last_def_assigned + 1, remaining_atks, k));
        int best = std::max(with, without);

        memo[remaining_atks - 1][b] = best;
        memo[remaining_atks][last_def_assigned + 1] = best;

        return best;
    } else {
        // Impossible strategy - we skip the first attacker of [a, b] and try again
        memo[remaining_atks][last_def_assigned + 1] = get_strategy_tr(v, mappings, last_def_assigned + 1, remaining_atks, k); 
        return memo[remaining_atks][last_def_assigned + 1];
    }
}

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;

    std::vector<int> v(n);
    for (int i = 0 ; i < n ; i++) {
        std::cin >> v[i];
    }

    // element i is [x, y] -> ith attacker attacks players from x to y
    std::vector<std::pair<int, int> > mappings(m, std::make_pair(0, 0));
    memo = std::vector<std::vector<int> >(m + 1, std::vector<int>(n + 1, -2));

    int strategy = get_strategy_tr(v, mappings, -1, m, k);
    if (strategy <= 0) {
        std:: cout << "fail" << std::endl;
    } else {
        std:: cout << strategy << std::endl;
    }
}

int main () {
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
        return 0;
}
