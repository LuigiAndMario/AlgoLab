#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel EE;
typedef EE::Point_2 Point;
typedef EE::Segment_2 Segment;
typedef EE::Ray_2 Ray;

double my_floor(const EE::FT x) {
    double res = std::floor(CGAL::to_double(x));
    while (res > x) {
        res--;
    }
    while (res + 1 <= x) {
        res++;
    }
    return res;
}

void check_and_expand(Segment &trajectory, CGAL::Object UFO) {
    if (const Point *i_cant_believe = CGAL::object_cast<Point>(&UFO)) {
        trajectory = Segment(trajectory.source(), *i_cant_believe);
    } else if (const Segment *its_not_butter = CGAL::object_cast<Segment>(&UFO)) {
        trajectory = Segment(trajectory.source(), 
                CGAL::collinear_are_ordered_along_line(trajectory.source(), its_not_butter->source(), its_not_butter->target())
                    ? its_not_butter->source()
                    : its_not_butter->target());
    }
}

int main() {
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

    int n; std::cin >> n;
    while (n != 0) {
        double x; std::cin >> x;
        double y; std::cin >> y;
        Point start(x, y);

        double a; std::cin >> a;
        double b; std::cin >> b;
        Point another(a, b);

        Ray laser_ray(start, another);

        std::vector<Segment> obstacle_course(n);
        for (int i = 0 ; i < n ; i++) {
            double r; std::cin >> r;
            double s; std::cin >> s;
            Point p_1(r, s);

            double t; std::cin >> t;
            double u; std::cin >> u;
            Point p_2(t, u);

            obstacle_course.at(i) = Segment(p_1, p_2);
        }

        std::random_shuffle(obstacle_course.begin(), obstacle_course.end());
        bool collision = false;
        Segment trajectory(start, start);
        int i = 0;

        while (!collision && i < n) {
            if (do_intersect(laser_ray, obstacle_course[i])) {
                collision = true;
                check_and_expand(trajectory, intersection(laser_ray, obstacle_course[i]));
            } else {
                i++;
            }
        }

        if (collision) {
            while(i < n) {
                if (do_intersect(trajectory, obstacle_course[i])) {
                    check_and_expand(trajectory, intersection(laser_ray, obstacle_course[i]));
                }
                i++;
            }

            std::cout << my_floor(trajectory.target().x()) << " " << my_floor(trajectory.target().y()) << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }

        std::cin >> n;
    }
}
