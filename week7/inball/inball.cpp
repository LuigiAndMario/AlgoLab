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

void testcase(int n) {
    int d; cin >> d;
    
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    
    // We want to maximise the diameter of the ball - so we minimise the negative version of all that
    lp.set_c(d, -1);
    
    for (int i = 0 ; i < n ; i++) {
        int r = 0;
        for (int j = 0 ; j < d ; j++) {
            int aij; cin >> aij;
            r += (aij * aij);
            
            lp.set_a(j, i, aij); // ... + aij * xj + ...
        }
        
        r = sqrt(r);
        lp.set_a(d, i, r); // ... + r * xd
        
        int bi; cin >> bi;
        lp.set_b(i, bi); // ... <= bi
    }
    
    lp.set_l(d, true, 0); // Because the radius needs to be >= 0
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (s.is_unbounded()) {
        cout << "inf" << endl;
    } else if (!s.is_optimal()) {
        cout << "none" << endl;
    } else {
        cout << floor_to_double(-s.objective_value()) << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
    return 0;
}
