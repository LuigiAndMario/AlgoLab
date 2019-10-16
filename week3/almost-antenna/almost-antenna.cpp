#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel_with_sqrt K;

typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

void testcase(int n) {
    vector<Point> homes(n);
    for (int i = 0 ; i < n ; i++) {
        double xi; cin >> xi;
        double yi; cin >> yi;
        homes[i] = Point(xi, yi);
    }
    
    Min_circle min_coverage(homes.begin(), homes.end(), true);
    K::FT min_radius = min_coverage.circle().squared_radius();
    for (auto it = min_coverage.support_points_begin() ; it != min_coverage.support_points_end() ; it++) {
        int i = 0;
        bool safe = true;
        while (safe && i < n) {
            if (homes[i] == *it) {
                // Current home under non-consideration - we replace it by the previous (or next if it is the first one)
                if (i == 0) {
                    homes[i] = homes[i + 1];
                } else {
                    homes[i] = homes[i - 1];
                }
                
                Min_circle coverage(homes.begin(), homes.end(), true);
                K::FT radius = coverage.circle().squared_radius();
                
                if (radius < min_radius) {
                    min_radius = radius;
                }
                
                homes[i] = *it; // Putting the home back in the list
                safe = false;
            }
            i++;
        }
    }
    
    cout << ceil(sqrt(min_radius)) << endl;
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
