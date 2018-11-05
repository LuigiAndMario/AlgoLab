// Never have I seen, in this course, such a poorly formulated exercise - it is simply insulting.

// \begin{from the slides}
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
// \end{from the slides}

// Variables of the QP
#define COSTS 0
#define RETURNS 1

void testcase(int n, int m) {
    Program qp(CGAL::LARGER, true, 0, false, 0);
    
    for (int i = 0 ; i < n ; i++) {
        int cost_i; std::cin >> cost_i;
        int return_i; std::cin >> return_i;
        
        qp.set_a(i, COSTS, return_i);
        qp.set_a(i, RETURNS, -cost_i);
    }
    
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < n ; j++) {
            int covariance; std::cin >> covariance;
            if (i >= j) {
                qp.set_d(i, j, 2 * covariance);
            }
        }
    }
    
    for (int i = 0 ; i < m ; i++) {
        int C; std::cin >> C; // max cost
        int R; std::cin >> R; // min return
        int V; std::cin >> V; // max variance
        
        qp.set_b(COSTS, R);
        qp.set_b(RETURNS, -C);
        
        assert(qp.is_nonnegative());
        Solution s = CGAL::solve_quadratic_program(qp, ET());
        assert(s.solves_quadratic_program(qp));
        
        if (s.is_optimal()) {
            std::cout << ((s.objective_value().numerator() / s.objective_value().denominator()) <= V ? "Yes" : "No");
        } else {
            std::cout << "No";
        }
        std::cout << "." << std::endl;
    }
}

int main() {
    int n; std::cin >> n; // # assets
    int m; std::cin >> m; // # beggars
    while (n != 0 && m != 0) {
        testcase(n, m);
        std::cin >> n;
        std::cin >> m;
    }
    
    return 0;
}
