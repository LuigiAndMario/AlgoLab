#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

int tail_rec(std::deque<int> &coins, bool my_turn, int left_dent, int right_dent, std::vector<std::vector<int> > &values) {
    if (left_dent > right_dent || right_dent < 0) {
        return 0;
    }
    if (left_dent == right_dent) {
        return coins[left_dent];
    }
    if (values[left_dent][right_dent] != -1) {
        return values[left_dent][right_dent];
    }

    int take_left = coins[left_dent] + std::min(
            tail_rec(coins, !my_turn, left_dent + 2, right_dent, values),
            tail_rec(coins, !my_turn, left_dent + 1, right_dent - 1, values)
            );
    int take_right = coins[right_dent] + std::min(
            tail_rec(coins, !my_turn, left_dent, right_dent - 2, values),
            tail_rec(coins, !my_turn, left_dent + 1, right_dent - 1, values)
            );

    values[left_dent][right_dent] = std::max(take_left, take_right);
    return values[left_dent][right_dent];
}

void testcase() {
    int n; std::cin >> n;
    std::deque<int> coins(n);
    for (int i = 0 ; i < n ; i++) {
        std::cin >> coins[i];
    }

    std::vector<std::vector<int> > values(n, std::vector<int>(n, -1));
    std::cout << tail_rec(coins, true, 0, n - 1, values) << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }

    return 0;
}
