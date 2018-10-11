#include <iostream>
#include <vector>

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;

    std::vector<int> v(n);
    std::vector<int> v_sum(n);
    std::cin >> v[0];
    v_sum[0] = v[0];
    for (int i = 1 ; i < n ; i++) {
        std::cin >> v[i];
        v_sum[i] = v_sum[i - 1] + v[i];
    }

    std::vector<int> interval_values(n, 0);
    int a = 0;
    int b = 0;
    while (b < n) {
        int current = v_sum[b] - v_sum[a] + v[a];

        if (current == k) {
            interval_values[b] = b - a + 1;
        }
        if (current <= k) {
            b++;
        }
        if (current >= k) {
            a++;
        }
    }

    std::vector<std::vector<int> > memo(m + 1, std::vector<int>(n, 0)); // memo[i][j] = value if the ith attacker attacks up to the jth defender
    // memo[0][j] = 0 - the 0th player cannot be assigned to an interval
    // memo[i][0] = interval_values[0] - player i attacks defender 0 if they can
    for (int i = 1 ; i < m + 1 ; i++) {
        memo[i][0] = interval_values[0];
    }

    for (int i = 1 ; i < m + 1 ; i++) {
        for (int j = 1 ; j < n ; j++) {
            int value_if_attack_here = interval_values[j]; // We take the best of that and, well, not that
            memo[i][j] = std::max(value_if_attack_here + memo[i - 1][j - value_if_attack_here], memo[i][j - 1]);
        }
    }

    if (memo[m - 1][n - 1] == memo[m][n - 1]) {
        // We have the same value if the last attacker attacks the last defender or not -> fail
        std::cout << "fail" << std::endl; 
    } else {
        std::cout << memo[m][n - 1] << std::endl;
    }

}



int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
