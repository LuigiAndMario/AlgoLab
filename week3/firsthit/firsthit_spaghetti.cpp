#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

double floor_to_double(const K::FT& x) {
    // Copied from the slides
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

Point end(Point a, Point b, Point x, Point y) {
    if (collinear_are_ordered_along_line(a, x, y)) {
        return x;
    } else if (collinear_are_ordered_along_line(a, y, x)) {
        return y;
    } else return object_cast<Point>(intersection(Segment(a, b), Segment(x, y)));
}

void adjust(Segment &trajectory, CGAL::Object overlap) {
    if (const Point *point = object_cast<Point>(&overlap)) {
        trajectory = Segment(trajectory.source(), *point);
    } else if (const Segment *segment = object_cast<Segment>(&overlap)) {
        trajectory = Segment(trajectory.source(),
                             collinear_are_ordered_along_line(trajectory.source(), segment->source(), segment->target())
                             ? segment->source() : segment->target());
    }
}

int ray_collides(Point a, Point b, Point x, Point y) {
    // Supposing that a, b are on the laser and x, y designate the obstacle
    // There is no collision only if the obstacle is completely left, or completely right of the trajectory
    // WARNING: This applies ONLY to the `laser` ray, no to the ``trajectory` segment
    if ((left_turn(a, b, x) && left_turn(a, b, y)) || (right_turn(a, b, x) && right_turn(a, b, y))) {
        // There is no collision only if the obstacle is completely left, or completely right of the trajectory
        return 0; // No collision
    }
    if (collinear_are_ordered_along_line(a, x, y)) {
        return 1; // a -> x -> y
    }
    if (collinear_are_ordered_along_line(a, y, x)) {
        return 2; // a -> y -> x
    }
    return 3; // "Simple" collision
}

bool segment_collides(Point a, Point b, Point x, Point y) {
    // Supposing that a, b are on the trajectory and x, y designate the obstacle
    // There is no collision only if the obstacle is completely left (resp. right) of the trajectory,
    // AND ALSO completely left (resp. right) of the inverse trajectory
    return (
            ((left_turn(a, b, x) && !left_turn(a, b, y)) && (left_turn(b, a, x) && !left_turn(b, a, y))) ||
            ((!left_turn(a, b, x) && left_turn(a, b, y)) && (!left_turn(b, a, x) && left_turn(b, a, y)))
            );
}

void testcase(int n) {
    double x; cin >> x;
    double y; cin >> y;
    double a; cin >> a;
    double b; cin >> b;
    Point first(x, y);
    Point second(a, b);
    
    Ray laser = Ray(first, second);
    Segment trajectory(first, first); // This is going to change as we progress. We start at `first`
    Point to;
    
    int i = 0;
    bool collision = false;
    while (!collision && i < n) {
        double r; cin >> r;
        double s; cin >> s;
        double t; cin >> t;
        double u; cin >> u;
        Point ox(r, s);
        Point oy(t, u);
        
        int collision_type = ray_collides(first, second, ox, oy);
        if (collision_type == 0) {
            // No collision
            i++;
        } else {
            collision = true;
            if (collision_type == 1) {
                // Colinear
                to = ox;
            } else if (collision_type == 2) {
                to = oy;
            } else {
                // "Simple" collision
                to = object_cast<Point>(intersection(Ray(first, second), Segment(ox, oy)));
            }
            // adjust(trajectory, intersection(laser, Segment(ox, oy)));
            // to = end_ray(first, second, ox, oy);
            trajectory = Segment(first, to);
        }
        i++;
    }
    
    if (collision) {
        while (i < n) {
            double r; cin >> r;
            double s; cin >> s;
            double t; cin >> t;
            double u; cin >> u;
            Point ox(r, s);
            Point oy(t, u);
            
            // if (segment_collides(first, to, ox, oy)) {
            if (do_intersect(trajectory, Segment(ox, oy))) {
                // We check here for the intersections with the current trajectory
                collision = true;
                adjust(trajectory, intersection(laser, Segment(ox, oy)));
                // to = end(first, to, ox, oy);
                // trajectory = Segment(first, to);
            }
            i++;
        }
        
        cout << floor_to_double(trajectory.target().x()) << " " << floor_to_double(trajectory.target().y()) << endl;
    } else {
        cout << "no" << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout << setiosflags(ios::fixed) << setprecision(0); // This rounds up to the next integer
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
    return 0;
}
