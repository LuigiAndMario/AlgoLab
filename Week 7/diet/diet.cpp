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

typedef CGAL::Quotient<ET> SolT;

double floor_to_double (const SolT &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a--;
    while (a - 1 <= x) a++;
    return a;
}
// \end{from the slides}

void testcase(int n, int m) {
    Program lp(CGAL::LARGER, true, 0, false, 0);
    
    // Each nutrient is simply two constraints
    for (int i = 0 ; i < n ; i++) {
        int min_i; std::cin >> min_i;
        int max_i; std::cin >> max_i;
        
        lp.set_b(i, min_i); // ... >= min_i
        lp.set_b(i + n, -max_i); // ... >= -max_i
    }
    // Currently, we have 2n elements in the linear program
    
    // Each food is an n-dimensional line
    for (int i = 0 ; i < m ; i++) {
        int cost_i; std::cin >> cost_i;
        lp.set_c(i, cost_i); // Minimising the costs
        
        for (int j = 0 ; j < n ; j++) {
            int quantity_i_j; std::cin >> quantity_i_j; // quantity of jth nutrient in ith food
            lp.set_a(i, j, quantity_i_j); // ... + x_i*quantity_i_j + ... >= min_j
            lp.set_a(i, j + n, -quantity_i_j); // ... - x_i*quantity_i_j - ... >= -max_j
        }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    std::cout << std::setprecision(12);
    if (s.is_infeasible()) {
        std::cout << "No such diet.";
    } else {
        std::cout << floor_to_double(s.objective_value()) - 2;
    }
    std::cout << std::endl;
}

int main() {
    int n; std::cin >> n; // nutrients
    int m; std::cin >> m; // foods
    while (n != 0 && m != 0) {
        testcase(n, m);
        std::cin >> n;
        std::cin >> m;
    }
    
    return 0;
}
