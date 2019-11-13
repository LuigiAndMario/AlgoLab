#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef double IT;
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

void testcase() {
    int n; cin >> n; // # posters to be added
    int m; cin >> m; // # posters already there
    int h; cin >> h;
    int w; cin >> w;
    
    // First testset assumption: y = 0, m = 0
    vector<pair<IT, IT> > new_nails(n);
    for (int i = 0 ; i < n ; i++) {
        IT x; cin >> x;
        IT y; cin >> y;
        
        new_nails[i] = make_pair(x, y);
    }
    
    vector<pair<IT, IT> > old_nails(m);
    for (int i = 0 ; i < m ; i++) {
        IT x; cin >> x;
        IT y; cin >> y;
        
        old_nails[i] = make_pair(x, y);
    }
    
    Program lp (CGAL::SMALLER, true, 1, false, 0); // Because forall i: ai >= 1
    int eqn = 0; // Index of what equation we're currently at
    
    for (int i = 0 ; i < n ; i++) {
        for (int j = i + 1; j < n ; j++) {
            IT dw = 2 * abs(new_nails[i].first - new_nails[j].first); // 2|xi - xj|
            // Assumption: dh = 0 (because y = 0)
            
            lp.set_a(i, eqn, w); // ai * w
            lp.set_a(j, eqn, w); // + aj * w
            lp.set_b(eqn, dw); //   <= 2|xi - xj|
            
            eqn++;
        }
    }
    
    // Assumption: m = 0 (no old posters)
    
    for (int i = 0 ; i < n ; i++) {
        lp.set_c(i, -2 * (w + h)); // We want to maximise it, so we minimise its inverse
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    std::cout << setprecision(0) << fixed << ceil_to_double(-s.objective_value()) << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
