#include <iostream>
#include <vector>
#include <tuple>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef double IT; // has to be double instead of int (cause of potential decimals)
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET; // Gmpq is much slower than Gmpz, but both give a correct result

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

using namespace std;

typedef tuple<IT, IT, IT> point;

const int D_MAX = 30;
const int X = 0;
const int Y = 1;
const int Z = 2;
vector<point> healthy;
vector<point> tumor;

vector<IT> combinations(IT x, IT y, IT z, int d) {
    // x^d y^0 z^0, x^{d-1} (y^0 z^0 | y^1 z^0 | y^0 z^1), ...
    // Returns a vector of all possible combinations of x^i y^j z^k s.t. i + j + k <= d
    // (Inspired from slide 35 onwards, not quite sure why it should work though)
    vector<IT> terms(0);
    for (int i = 0 ; i <= d ; i++) {
        for (int j = 0 ; j <= d - i ; j++) {
            for (int k = 0 ; k <= d - i - j ; k++) {
                terms.push_back(pow(x, i) * pow(y, j) * pow(z, k));
            }
        }
    }
    
    return terms;
}

bool sufficient(int d) {
    // Returns true iff d is a sufficient value for the number of degrees
    int h = healthy.size();
    int t = tumor.size();
    
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    
    // Again, inspired by slide 35 onwards (still not sure why it even works)
    // The idea is two have all the healthy cells on one side of the curve and the tumors on the other
    for (int i = 0 ; i < h ; i++) {
        vector<IT> terms = combinations(get<X>(healthy[i]), get<Y>(healthy[i]), get<Z>(healthy[i]), d);
        
        for (int j = 0 ; j < (int) terms.size() ; j++) {
            lp.set_a(j, i, terms[j]); // ... + xi * (xhi^_ yhi^_ zhi^_) + ...
        }
        
        lp.set_b(i, -1); // <= -1
    }
    
    for (int i = 0 ; i < t ; i++) {
        vector<IT> terms = combinations(get<X>(tumor[i]), get<Y>(tumor[i]), get<Z>(tumor[i]), d);
        
        for (int j = 0 ; j < (int) terms.size() ; j++) {
            lp.set_a(j, h + i, terms[j]); // ... + xi * (xhi^_ yhi^_ zhi^_) + ...
        }
        
        lp.set_r(h + i, CGAL::LARGER);
        lp.set_b(h + i, 1); // >= 1
    }
    
    // Copied from the slides
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    return !s.is_infeasible();
}

void testcase() {
    int h; cin >> h;
    int t; cin >> t;
    
    healthy = vector<point>(h);
    for (int i = 0 ; i < h ; i++) {
        IT x; cin >> x;
        IT y; cin >> y;
        IT z; cin >> z;
        
        healthy[i] = make_tuple(x, y, z);
    }
    
    tumor = vector<point>(t);
    for (int i = 0 ; i < t ; i++) {
        IT x; cin >> x;
        IT y; cin >> y;
        IT z; cin >> z;
        
        tumor[i] = make_tuple(x, y, z);
    }
    
    if (h == 0 || t == 0) {
        // If one of these sets is empty, then it is trivial; we don't even need a polynomial to separate them
        cout << 0 << endl;
        return;
    }
    
    int lower = 1;
    int upper = 2; // [lower, upper] = the window of possible ds
    while (upper <= D_MAX && !sufficient(upper)) {
        // Setting the window from the lowest sufficient d, to the maximum possible d
        upper = min(2 * upper, D_MAX + 1);
    }
    
    // Now we do the binary search part
    while (lower != upper) {
        int d = (lower + upper) / 2;
        
        if (sufficient(d)) {
            upper = d;
        } else {
            lower = d + 1;
        }
    }
    
    if (lower <= D_MAX) {
        cout << lower << endl;
    } else {
        cout << "Impossible!" << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int n; cin >> n;
    while (n--) {
        testcase();
    }
    return 0;
}
