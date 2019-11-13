#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef K::Point_2 Point;

using namespace std;

double ceil_to_double(K::FT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase(int n) {
    // General assumptions:
    // - every (x, y) is unique
    // - every speed is the same (1 m/s)
    // - if two graypes are the closest ones, run to the lexicographically smallest one (smallest x, or equal x and smallest y)
    vector<Point> pos(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        pos[i] = Point(x, y);
    }
    
    Triangulation t;
    t.insert(pos.begin(), pos.end());
    
    // Assuming there is only one single closest grapye:
    // If graype g1 runs towards graype g2, then g2 is the closest one to g1
    // Therefore, g1 is the closest one to g2 (symmetry)
    // Therefore, g2 runs towards g1 (definition)
    // Therefore, the time it takes for g1 and g2 to reach each-other is d / 2 seconds
    // BUT, this only holds if the number of closest graypes is 1
    // Otherwise, we could have something like g0 - g1 - g2, where g2 -> g1 but g1 -> g0
    
    K::FT dmin = INT_MAX; // The minimal distance to be traveled between any two graypes
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        K::FT d = t.segment(e).squared_length();
        dmin = min(dmin, d);
    }
    
    dmin = sqrt(dmin); // Cause the distance is squared
    dmin /= 2.0; // Cause both graypes run towards each other
    dmin *= 100; // Cause we want the result in hundredths of second
    
    cout << ceil_to_double(dmin) << endl;
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
