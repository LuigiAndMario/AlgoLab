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

void testcase(int p) {
    IT a; cin >> a;
    IT b; cin >> b;
    
    if (p == 1) {
        Program lp (CGAL::SMALLER, true, 0, false, 0);
        const int X = 0;
        const int Y = 1;
        
        lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
        lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); // 4x + 2y <= ab
        lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1
        
        // Keep in mind: this will minimise the solution, so to get the maximum solution, we get the minimum of the negative version, then multiply it by -1
        lp.set_c(Y, -b); // by
        lp.set_c(X, a); // -ax
        
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program(lp));
        
        if (!s.is_optimal()) {
            std::cout << "no" << std::endl;
        } else if (s.is_unbounded()) {
            std::cout << "unbounded" << std::endl;
        } else {
            cout << floor_to_double(-s.objective_value()) << endl;
        }
    } else {
        Program lp (CGAL::LARGER, false, 0, true, 0);
        const int X = 0;
        const int Y = 1;
        const int Z = 2;
        
        lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_a(Z, 0, 0); lp.set_b(0, -4); // x + y >= -4
        lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b); // 4x + 2y + z >= -ab
        lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_a(Z, 2, 0); lp.set_b(2, -1); // -x + y >= -1
        
        lp.set_c(X, a); // ax
        lp.set_c(Y, b); // by
        lp.set_c(Z, 1); // z
        
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program(lp));
        
        if (!s.is_optimal()) {
            std::cout << "no" << std::endl;
        } else if (s.is_unbounded()) {
            std::cout << "unbounded" << std::endl;
        } else {
            cout << ceil_to_double(s.objective_value()) << endl;
        }
    }
}

int main() {
    int p; cin >> p;
    while (p) {
        testcase(p);
        cin >> p;
    }
    return 0;
}
