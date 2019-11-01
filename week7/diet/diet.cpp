#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

using namespace std;

double ceil_to_double(const SolT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

double floor_to_double(const SolT &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int n, int m) {
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    
    for (int i = 0 ; i < n ; i++) {
        IT mini; cin >> mini;
        IT maxi; cin >> maxi;
        
        lp.set_b(i, maxi); // ... <= maxi
        lp.set_b(n + i, -mini); // ... <= -mini
    }
    
    for (int j = 0 ; j < m ; j++) {
        IT pj; cin >> pj;
        
        lp.set_c(j, pj);
        for (int i = 0 ; i < n ; i++) {
            IT cji; cin >> cji;
            
            lp.set_a(j, i, cji); // ... + cji * xj + ...
            lp.set_a(j, n + i, -cji); // ... - cji * xj - ...
        }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (!s.is_optimal() || s.is_unbounded()) {
        std::cout << "No such diet." << std::endl;
    } else {
        cout << floor_to_double(s.objective_value()) << endl;
    }
}

int main() {
    int n; cin >> n;
    int m; cin >> m;
    while (n != 0 && m != 0) {
        testcase(n, m);
        cin >> n;
        cin >> m;
    }
    return 0;
}
