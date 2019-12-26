#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

// Basic geometry
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
// Triangulation
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
// LP
typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    int a; cin >> a; // # asteroids
    int s; cin >> s; // # shots
    int b; cin >> b; // # bounty hunters
    
    int e; cin >> e; // Available energy
    
    vector<pair<Point, int>> asteroids(a);
    for (int i = 0 ; i < a ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        int d; cin >> d;
        
        asteroids[i] = make_pair(Point(x, y), d);
    }
    
    vector<Point> shots(s);
    for (int i = 0 ; i < s ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        shots[i] = Point(x, y);
    }
    
    vector<Point> bounty_hunters(b);
    for (int i = 0 ; i < b ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        bounty_hunters[i] = Point(x, y);
    }
    Triangulation t;
    t.insert(bounty_hunters.begin(), bounty_hunters.end());
    
    // LP
    Program lp(CGAL::LARGER, true, 0, false, 0); // >= 0, < inf
    
    for (int i = 0 ; i < a ; i++) {
        for (int j = 0 ; j < s ; j++) {
            K::FT dj = CGAL::squared_distance(shots[j], asteroids[i].first); // distance between the shot and the asteroid
            ET den = 1 < dj ? dj : 1;
            lp.set_a(j, i, 1 / den); // xj / max(1, dj^2)
        }
        lp.set_b(i, asteroids[i].second);
    }
        
    // First testset assumption: b = 0
    for (int i = 0 ; i < s ; i++) {
        lp.set_c(i, 1);
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    
    if (sol.objective_value() > e) {
        cout << "n";
    } else {
        cout << "y";
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
