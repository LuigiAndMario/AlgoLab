#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel_with_sqrt K;

typedef K::Point_2 Point;
typedef  CGAL::Min_circle_2_traits_2<K> Traits;
typedef  CGAL::Min_circle_2<Traits> Min_circle;

void testcase(int n) {
    Point homes[n];
    for (int i = 0 ; i < n ; i++) {
        double xi; cin >> xi;
        double yi; cin >> yi;
        homes[i] = Point(xi, yi);
    }
    
    Traits::Circle coverage = Min_circle(homes, homes + n, true).circle();
    cout << ceil(sqrt(coverage.squared_radius())) << endl;
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
