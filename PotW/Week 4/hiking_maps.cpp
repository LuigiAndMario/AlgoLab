#include <iostream>
#include <vector>
#include <climits>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EI;
typedef EI::Point_2 Point;

struct Segment {
    Point p_1;
    Point p_2;
};

struct Triangle {
    Segment s_1;
    Segment s_2;
    Segment s_3;
};

std::vector<Segment> trail;
std::vector<Triangle> map_parts;
std::vector<std::vector<int> > coverage; // coverage[i] = x <-> the ith triangle covers all the path segments in x
std::vector<int> covered_segments;

bool shoot_the_sheriff(Triangle t, Point p) {
    return !right_turn(t.s_1.p_1, t.s_1.p_2, p)
        && !right_turn(t.s_2.p_1, t.s_2.p_2, p)
        && !right_turn(t.s_3.p_1, t.s_3.p_2, p);
}

bool contains(Triangle t, Segment s) {
    return shoot_the_sheriff(t, s.p_1) && shoot_the_sheriff(t, s.p_2);
}

void testcase() {
    int m; std::cin >> m;
    int n; std::cin >> n;

    trail = std::vector<Segment>(m - 1);
    int x; std::cin >> x;
    int y; std::cin >> y;
    Point prev(x, y);
    Point cur;
    for (int i = 0 ; i < m - 1 ; i++) {
        int x; std::cin >> x;
        int y; std::cin >> y;
        cur = Point(x, y);
        trail.at(i).p_1 = prev;
        trail.at(i).p_2 = cur;
        prev = cur;
    }

    map_parts = std::vector<Triangle>(n);
    coverage = std::vector<std::vector<int> >(n);
    for (int i = 0 ; i < n ; i++) {
        std::vector<Point> points(6);
        for (int j = 0 ; j < 6 ; j++) {
            int x_1; std::cin >> x_1;
            int x_2; std::cin >> x_2;
            points[j] = Point(x_1, x_2);
        }

        std::vector<Segment> segments(3);
        segments[0].p_1 = points[0];
        segments[0].p_2 = points[1];
        segments[1].p_1 = points[2];
        segments[1].p_2 = points[3];
        segments[2].p_1 = points[4];
        segments[2].p_2 = points[5];

        map_parts[i].s_1 = segments[0];
        map_parts[i].s_2 = segments[1];
        map_parts[i].s_3 = segments[2];

        // Reversing the segment if they are not on the right order
        if (!left_turn(map_parts[i].s_1.p_1, map_parts[i].s_1.p_2, map_parts[i].s_2.p_1)) {
            Point tmp = map_parts[i].s_1.p_1;
            map_parts[i].s_1.p_1 = map_parts[i].s_1.p_2;
            map_parts[i].s_1.p_2 = tmp;
        }
        if (!left_turn(map_parts[i].s_2.p_1, map_parts[i].s_2.p_2, map_parts[i].s_3.p_1)) {
            Point tmp = map_parts[i].s_2.p_1;
            map_parts[i].s_2.p_1 = map_parts[i].s_2.p_2;
            map_parts[i].s_2.p_2 = tmp;
        }
        if (!left_turn(map_parts[i].s_3.p_1, map_parts[i].s_3.p_2, map_parts[i].s_1.p_1)) {
            Point tmp = map_parts[i].s_3.p_1;
            map_parts[i].s_3.p_1 = map_parts[i].s_3.p_2;
            map_parts[i].s_3.p_2 = tmp;
        }

        // Writing down what sections are covered by the current triangle
        for (int j = 0 ; j < m - 1 ; j++) {
            if (contains(map_parts[i], trail[j])) {
                coverage[i].push_back(j);
            }
        }
    }

    covered_segments = std::vector<int>(m - 1, 0);

    int best_value = INT_MAX;
    int left = 0;
    int right = -1;
    bool expand_right = true; // If false, then we remove triangles on the left
    bool safe = true;
    int covered_by_window = 0;

    while (safe) {
        if (right >= n || left >= n) {
            safe = false;
        } else {
            if (left > right) {
                // The window is now nonexistent, so we start expanding
                expand_right = true;
            }

            if (expand_right) {
                if (right >= n - 1) {
                    expand_right = false;
                    continue;
                }

                right++;
                int how_much_it_covers = coverage[right].size();
                for (int i = 0 ; i < how_much_it_covers ; i++) {
                    int current_section = coverage[right][i];
                    if (covered_segments[current_section] == 0) {
                        covered_by_window++;
                    }
                    covered_segments[current_section]++;
                }

            } else {
                int how_much_it_covers = coverage[left].size();
                for (int i = 0 ; i < how_much_it_covers ; i++) {
                    int current_section = coverage[left][i];
                    covered_segments[current_section]--;
                    if (covered_segments[current_section] == 0) {
                        covered_by_window--;
                    }
                }
                left++;
            }

            if (covered_by_window == m - 1) {
                expand_right = false;
                best_value = std::min(best_value, right - left + 1);
            } else {
                expand_right = true;
            }
        }
    }

    std::cout << best_value << std::endl;
}

int main() {
    int c; std::cin >> c;
    while (c--) testcase();
    return 0;
}
