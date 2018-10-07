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
        std::vector<Point> homes(n);
        for (int i = 0 ; i < n ; i++) {
            double x_i; std::cin >> x_i;
            double y_i; std::cin >> y_i;
            homes[i] = Point(x_i, y_i);
        }

        // Coverage zone for all homes
        Min_circle coverage_zone(homes.begin(), homes.end(), true);
        EEsqrt::FT current_radius = coverage_zone.circle().squared_radius();

        // Coverage zone for all but one home
        for (auto iter = coverage_zone.support_points_begin() ; iter < coverage_zone.support_points_end() ; iter++) {
            int i = 0;
            bool safe = true;
            while (safe && i < n) {
                if (homes[i] == *iter) {
                    homes[i] = homes[i + (i == 0 ? 1 : -1)];

                    Min_circle candidate_zone(homes.begin(), homes.end(), true);
                    EEsqrt::FT candidate_radius = candidate_zone.circle().squared_radius();

                    if (candidate_radius < current_radius) {
                        current_radius = candidate_radius;
                    }

                    homes[i] = *iter;
                    safe = false;
                }

                i++;
            }
        }

        std::cout << ceil(sqrt(current_radius)) << std::endl;

        std::cin >> n;
    }
}
