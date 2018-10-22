#include <iostream>
#include <vector>
#include <climits>

#define DEBUG false
#define DEBUG_PRINT(ARG) if (DEBUG) std::cerr << ARG << std::endl

void testcase() {
    int n; std::cin >> n; // # drinks
    int k; std::cin >> k; // # people - also, k is the minimal quantity of drinks that we want.

    std::vector<std::pair<int, int> > drinks(n);
    int max_volume = INT_MIN;
    for (int i = 0 ; i < n ; i++) {
        int c; std::cin >> c; // cost
        int v; std::cin >> v; // volume
        drinks[i] = std::make_pair(c, v);
        max_volume = std::max(max_volume, v);
    }
    
    DEBUG_PRINT(max_volume);
    
    int max = k + max_volume;
    std::vector<std::vector<std::pair<int, int> > > memo(n, std::vector<std::pair<int, int> >(max + 1)); // [drink][volume] = <cost, nb drinks>
    
    memo[0][0] = std::make_pair(0, 0);
    for (int i = 1 ; i < max + 1 ; i++) {
        int cost = drinks[0].first;
        int volume = drinks[0].second;
        memo[0][i] = std::make_pair(((i + volume - 1)/volume) * cost, 1);
    }
    
    for (int i = 1 ; i < n ; i++) {
        memo[i][0] = memo[i - 1][0];
        
        for (int j = 1 ; j < max + 1 ; j++) {
            int min_cost = INT_MAX;
            int max_drinks = INT_MIN;
            int cost = drinks[i].first;
            int volume = drinks[i].second;
            
            std::pair<int, int> take_it = memo[i][j - volume];
            std::pair<int, int> leave_it = memo[i - 1][j];
            
            if (j < volume) {
                // The volume of the current bottle is more than what we have, so we take it instead
                min_cost = cost;
                max_drinks = 1;
            } else {
                // We take a bottle of this drink
                min_cost = take_it.first + cost;
                max_drinks = take_it.second + (take_it == memo[i - 1][j - volume] ? 1 : 0); // If we take another bottle of the same drink, we don't increment the counter
            }
            
            if (leave_it.first < min_cost) {
                // This bottle is even cheaper than what we have, so we take it instead
                min_cost = leave_it.first;
                max_drinks = leave_it.second;
            } else if (min_cost == leave_it.first) {
                // Equal costs - we want to take the solution with the most drinks
                max_drinks = std::max(max_drinks, leave_it.second);
            }
            
            memo[i][j] = std::make_pair(min_cost, max_drinks);
        }
    }
    
    std::cout << memo[n - 1][k].first << " " << memo[n - 1][k].second << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
