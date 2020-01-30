// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef K::Point_2 Point;

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
    
    vector<Point> all(n + m);
    
    // Warehouses
    vector<Point> warehouses(n);
    vector<int> s(n);
    vector<int> a(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        warehouses[i] = Point(x, y);
        all[i] = warehouses[i];
        cin >> s[i]; // Supply
        cin >> a[i]; // Alcohol
    }
    
    // Stadiums
    vector<Point> stadiums(m);
    vector<int> d(m);
    vector<int> u(m);
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        stadiums[i] = Point(x, y);
        all[n + i] = stadiums[i];
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
    
    Triangulation t;
    t.insert(all.begin(), all.end());
    
    // Contour lines
    vector<pair<Point, K::FT>> lines;
    for (int i = 0 ; i < c ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        int r; cin >> r;
        
        Point centre(x, y);
        K::FT r_sq = K::FT(r) * K::FT(r);
        // We add this line only if there is at least *something* inside of it
        if (CGAL::squared_distance(t.nearest_vertex(centre)->point(), centre) <= r_sq) {
            lines.push_back(make_pair(centre, r_sq));
        }
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
            int lines_crossed = 0;
            for (auto &line : lines) {
                bool starts_in = CGAL::squared_distance(warehouses[i], line.first) <= line.second;
                bool ends_in = CGAL::squared_distance(stadiums[j], line.first) <= line.second;
                
                if (starts_in != ends_in) {
                    lines_crossed++;
                }
            }
            
            lp.set_c(n * i + j, -(100 * r[i][j] - lines_crossed)); // -xij * (100 * rij - lines_crossed) (i the warehouse, j the stadium)
        }
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    
    if (sol.is_infeasible()) {
        cout << "RIOT!" << endl;
    } else {
        cout << (long) floor_to_double(-sol.objective_value() / 100) << endl;
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
