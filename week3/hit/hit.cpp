#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

void testcase(int n) {
    double x; cin >> x;
    double y; cin >> y;
    double a; cin >> a;
    double b; cin >> b;
    Ray laser = Ray(Point(x, y), Point(a, b));
    
    bool collision = false;
    while (n--) {
        double r; cin >> r;
        double s; cin >> s;
        double t; cin >> t;
        double u; cin >> u;
        if (!collision && do_intersect(laser, Segment(Point(r, s), Point(t, u)))) {
            collision = true;
        }
    }
    
    cout << (collision ? "yes" : "no") << endl;
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
