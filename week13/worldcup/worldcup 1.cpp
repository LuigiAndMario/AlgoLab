// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase() {
    int n; cin >> n; // # warehouses
    int m; cin >> m; // # stadiums
    int c; cin >> c; // # contour lines
    
    // Warehouses
    vector<int> x_w(n);
    vector<int> y_w(n);
    vector<int> s(n);
    vector<int> a(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> x_w[i];
        cin >> y_w[i];
        cin >> s[i]; // Supply
        cin >> a[i]; // Alcohol
    }
    
    // Stadiums
    vector<int> x_s(m);
    vector<int> y_s(m);
    vector<int> d(m);
    vector<int> u(m);
    for (int i = 0 ; i < m ; i++) {
        cin >> x_s[i];
        cin >> y_s[i];
        cin >> d[i]; // demand
        cin >> u[i]; // Max alcohol
    }
    
    // rws
    vector<vector<int>> r(n, vector<int>(m));
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            cin >> r[i][j];
        }
    }
    
    // Contour lines
    vector<int> x_c(c);
    vector<int> y_c(c);
    vector<int> r_c(c);
    for (int i = 0 ; i < c ; i++) {
        cin >> x_c[i];
        cin >> y_c[i];
        cin >> r_c[i]; // Radius
        
        // Not for the first two testsets
    }
    
    // Second testset: c = 0
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    int cur = 0; // Current equation
    
    // For every warehouse i, the sum of beer sent is <= s[i]
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            lp.set_a(n * i + j, cur, 1); // ... + xij + ...
        }
        lp.set_b(cur++, s[i]); // ... <= si
    }
    
    // For every stadium j, the sum of alcohol received is <= u[j]
    for (int j = 0 ; j < m ; j++) {
        for (int i = 0 ; i < n ; i++) {
            lp.set_a(n * i + j, cur, a[i]); // ... + xij * ai + ...
        }
        lp.set_b(cur++, u[j] * 100); // ... <= uj
    }
    
    // For every stadium j, the quantity of beer received is = d (so <= d and >= d)
    for (int j = 0 ; j < m ; j++) {
        for (int i = 0 ; i < n ; i++) {
            lp.set_a(n * i + j, cur, 1); // ... + xij + ... (sum of all the liters)
            lp.set_a(n * i + j, cur + 1, -1); // ... - xij - ... (negative sum of all contents, for equality)
        }
        
        lp.set_b(cur, d[j]); // ... <= di (no less than di liters)
        lp.set_b(cur + 1, -d[j]); // ... <= -di (no more than di liters)
        cur += 2;
    }
    
    // Objective function
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            lp.set_c(n * i + j, -r[i][j]); // rij * xij (i the warehouse, j the stadium)
        }
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    
    if (sol.is_infeasible()) {
        cout << "RIOT!" << endl;
    } else {
        cout << (long) floor_to_double(-sol.objective_value()) << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
