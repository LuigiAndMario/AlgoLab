#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

double solve(int day, int wealth, int n, int goal, vector<double> &p) {
    if (wealth >= goal) {
        // Already won
        return 1;
    }
    if (wealth == 0 || day == n) {
        // Broke, or the magician left
        return 0;
    }
    
    double prob = 0;
    for (int i = 0 ; i <= wealth ; i++) { // i = the sum of money we bet
        double win = p[day] * solve(day + 1, wealth + i, n, goal, p);
        double loss = (1 - p[day]) * solve(day + 1, wealth - i, n, goal, p);
        
        prob = max(prob, win + loss);
    }
    
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
