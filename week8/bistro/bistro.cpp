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
    vector<Point> pos_old(n); // Existing locations
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        pos_old[i] = Point(x, y);
    }
    
    Triangulation t;
    t.insert(pos_old.begin(), pos_old.end());
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        Point p(x, y);
        
        cout << CGAL::squared_distance(p, t.nearest_vertex(p)->point()) << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
    return 0;
}
