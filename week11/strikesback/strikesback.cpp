#include <iostream>
#include <vector>
#include <climits>

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
    
    vector<pair<Point, int>> asteroids(a); // Position, density
    for (int i = 0 ; i < a ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        int d; cin >> d;
        
        asteroids[i] = make_pair(Point(x, y), d);
    }
    
    vector<pair<Point, double>> shots(s); // Position, distance to nearest bounty hunter
    for (int i = 0 ; i < s ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        shots[i] = make_pair(Point(x, y), INT_MAX);
    }
    
    vector<Point> bounty_hunters(b); // Position
    for (int i = 0 ; i < b ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        bounty_hunters[i] = Point(x, y);
    }
    
    // LP
    Program lp(CGAL::LARGER, true, 0, false, 0); // >= 0, < inf
    
    // Adjusting the distances to the bounty hunters
    if (b != 0) {
        Triangulation t;
        t.insert(bounty_hunters.begin(), bounty_hunters.end());
        
        for (int i = 0 ; i < s ; i++) {
            Point si = shots[i].first;
            shots[i].second = CGAL::squared_distance(si, t.nearest_vertex(si)->point());
        }
    }
    
    // Actual equations
    for (int i = 0 ; i < a ; i++) {
        for (int j = 0 ; j < s ; j++) {
            K::FT dj = CGAL::squared_distance(shots[j].first, asteroids[i].first); // distance between the shot and the asteroid
            ET den = 1 < dj ? dj : 1;
            if (dj < shots[j].second) {
                // We only add it if the distance to the asteroid is smaller than the distance to a bounty hunter
                lp.set_a(j, i, 1 / den); // ... xj / max(1, dj^2) ...
            }
        }
        lp.set_b(i, asteroids[i].second); // ... <= di
    }
    
    // Objective function, sum(j = 0 to s) xj <= e
    for (int i = 0 ; i < s ; i++) {
        lp.set_c(i, 1);
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    
    if (sol.objective_value() > e || sol.is_infeasible()) {
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
