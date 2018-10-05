#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel EE;

int main() {
    int n; std::cin >> n;
    while (n != 0) {
        double x; std::cin >> x;
        double y; std::cin >> y;
        EE::Point_2 start(x, y);

        double a; std::cin >> a;
        double b; std::cin >> b;
        EE::Point_2 another(a, b);

        EE::Ray_2 laser_ray(start, another);

        bool collision = false;
        for (int i = 0 ; i < n ; i++) {
            double r; std::cin >> r;
            double s; std::cin >> s;
            EE::Point_2 p_1(r, s);

            double t; std::cin >> t;
            double u; std::cin >> u;
            EE::Point_2 p_2(t, u);

            if (!collision) {
                EE::Segment_2 obstacle(p_1, p_2);
                collision = CGAL::do_intersect(laser_ray, obstacle);
            }
        }

        std::cout << (collision ? "yes" : "no") << std::endl;
        std::cin >> n;
    }
}
