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

void adjust(Segment &trajectory, CGAL::Object overlap) {
    if (const Point *point = object_cast<Point>(&overlap)) {
        trajectory = Segment(trajectory.source(), *point);
    } else if (const Segment *segment = object_cast<Segment>(&overlap)) {
        trajectory = Segment(trajectory.source(),
                             collinear_are_ordered_along_line(trajectory.source(), segment->source(), segment->target())
                             ? segment->source() : segment->target());
    }
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
    
    int i = 0;
    bool collision = false;
    while (!collision && i < n) {
        double r; cin >> r;
        double s; cin >> s;
        double t; cin >> t;
        double u; cin >> u;
        Segment obstacle(Point(r, s), Point(t, u));
        
        if (do_intersect(laser, obstacle)) {
            collision = true;
            adjust(trajectory, intersection(laser, obstacle));
        }
        i++;
    }
    
    random_shuffle(
    
    if (collision) {
        while (i < n) {
            double r; cin >> r;
            double s; cin >> s;
            double t; cin >> t;
            double u; cin >> u;
            Segment obstacle(Point(r, s), Point(t, u));
            
            if (do_intersect(trajectory, obstacle)) {
                // We check here for the intersections with the current trajectory
                collision = true;
                adjust(trajectory, intersection(laser, obstacle));
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
