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

Point end_ray(Point a, Point b, Point x, Point y) {
    if (collinear_are_ordered_along_line(a, x, y)) {
        return x;
    } else if (collinear_are_ordered_along_line(a, y, x)) {
        return y;
    } else return object_cast<Point>(intersection(Ray(a, b), Segment(x, y)));
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
    // There is a collision only if x or y belong to the line, or x and y are on two different sides AND not behind the ray
    if (collinear_are_ordered_along_line(a, x, y)) {
        return 1; // a -> x -> y
    }
    if (collinear_are_ordered_along_line(a, y, x)) {
        return 2; // a -> y -> x
    }
    if (left_turn(a, b, x) && left_turn(a, b, y)) {
        return 0;
    }
    if (right_turn(a, b, x) && right_turn(a, b, y)) {
        return 0;
    }
    if (left_turn(a, b, x) && right_turn(a, b, y) && angle(b, a, x) == CGAL::OBTUSE && angle(a, b, y) == CGAL::OBTUSE) {
        // Behind and x is on the left
        return 0;
    }
    if (right_turn(a, b, x) && left_turn(a, b, y) && angle(b, a, x) == CGAL::OBTUSE && angle(a, b, y) == CGAL::OBTUSE) {
        // Behind and x is on the right
        return 0;
    }
    
    return 3; // "Simple" collision
}

bool collides(Point a, Point b, Point x, Point y) {
    if (left_turn(a, b, x) && left_turn(a, b, y)) {
        // Completely left
        return false;
    }
    if (right_turn(a, b, x) && right_turn(a, b, y)) {
        // Completely right
        return false;
    }
    if (!right_turn(a, b, x) && !left_turn(a, b, y) && angle(b, a, x) == CGAL::OBTUSE && angle(b, a, y) == CGAL::OBTUSE) {
        // Fully behind, x left (or on the line)
        return false;
    }
    if (!left_turn(a, b, x) && !right_turn(a, b, y) && angle(b, a, y) == CGAL::OBTUSE && angle(b, a, x) == CGAL::OBTUSE) {
        // Fully behind, x right (or on the line)
        return false;
    }
    if (!right_turn(a, b, x) && !left_turn(a, b, y) && angle(b, a, x) == CGAL::OBTUSE && angle(b, a, y) == CGAL::ACUTE) {
        // x left (or on the line) and behind, y right (or on the line) and in front
        return false;
    }
    if (!right_turn(a, b, x) && !left_turn(a, b, y) && angle(b, a, x) == CGAL::ACUTE && angle(b, a, y) == CGAL::OBTUSE) {
        // x left (or on the line) and in front, y right (or on the line) and behind
        return false;
    }
    if (!left_turn(a, b, x) && !right_turn(a, b, y) && angle(b, a, x) == CGAL::OBTUSE && angle(b, a, y) == CGAL::ACUTE) {
        // x right (or on the line) and behind, y left (or on the line) and in front
        return false;
    }
    if (!left_turn(a, b, x) && !right_turn(a, b, y) && angle(b, a, x) == CGAL::ACUTE && angle(b, a, y) == CGAL::OBTUSE) {
        // x right (or on the line) and in front, y left (or on the line) and behind
        return false;
    }
    
    
    if (collinear_are_ordered_along_line(a, x, y)) {
        return true; // a -> x -> y
    }
    if (collinear_are_ordered_along_line(a, y, x)) {
        return true; // a -> y -> x
    }
    
    return true; // "Simple" collision
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
        Segment obstacle(Point(r, s), Point(t, u));
        
        if (collides(first, second, ox, oy)) {
            collision = true;
            // adjust(trajectory, intersection(laser, obstacle));
            to = end_ray(first, second, ox, oy);
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
            
            if (do_intersect(trajectory, Segment(ox, oy))) {
                // We check here for the intersections with the current trajectory
                adjust(trajectory, intersection(laser, Segment(ox, oy)));
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
