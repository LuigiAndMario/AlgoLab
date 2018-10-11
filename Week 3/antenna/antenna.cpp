#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Min_circle_2.h>
#include <iostream>
#include <vector>
#include <cmath>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EEsqrt;

typedef EEsqrt::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<EEsqrt> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

int main() {
    int n; std::cin >> n;
    while (n != 0) {
        std::vector<Point> homes_vector(n);
        for (int i = 0 ; i < n ; i++) {
            double x_i; std::cin >> x_i;
            double y_i; std::cin >> y_i;
            homes_vector[i] = Point(x_i, y_i);
        }

        std::random_shuffle(homes_vector.begin(), homes_vector.end());
        Point homes[n];
        for (int i = 0 ; i < n ; i++) {
            homes[i] = homes_vector[i];
        }

        Traits::Circle coverage_zone = Min_circle(homes, homes + n).circle();
        std::cout << ceil(sqrt(coverage_zone.squared_radius())) << std::endl;

        std::cin >> n;
    }
}
