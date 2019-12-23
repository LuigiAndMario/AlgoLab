#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

vector<vector<double>> memo;

double solve(int day, int wealth, int n, int goal, vector<double> &p) {
    // Corner cases
    if (wealth >= goal) {
        // Already won
        return 1;
    }
    if (wealth == 0 || day == n) {
        // Broke, or the magician left
        return 0;
    }
    
    // Precomputed
    if (memo[wealth][day] != -1) {
        return memo[wealth][day];
    }
    
    // Actual algorithm
    double prob = 0;
    for (int i = 0 ; i <= wealth ; i++) { // i = the sum of money we bet
        double win = p[day] * solve(day + 1, wealth + i, n, goal, p);
        double loss = (1 - p[day]) * solve(day + 1, wealth - i, n, goal, p);
        
        prob = max(prob, win + loss);
    }
    
    memo[wealth][day] = prob;
    return prob;
}

void testcase() {
    int n; cin >> n; // # days
    int k; cin >> k; // Starting wealth
    int m; cin >> m; // Target wealth
    
    vector<double> p(n); // Probabilities of having heads (i.e. winning the bet)
    for (int i = 0 ; i < n ; i++) {
        cin >> p[i];
    }
    
    memo = vector<vector<double>>(2 * m, vector<double>(n, -1)); // wealth x day -> prob
    double prob = solve(0, k, n, m, p);
    
    // Setting the precision to 5 decimals
    std::cout << std::fixed << std::setprecision(5);
    cout << prob << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
