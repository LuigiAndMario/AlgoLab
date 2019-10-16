#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/enum.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

typedef vector<double> vecd;
typedef vector<tuple<int, Ray> > vecir;
typedef vector<bool> vecb;
typedef vector<int> veci;

bool asc_y0(tuple<int, Ray> biker1, tuple<int, Ray> biker2) {
    return get<1>(biker1).source().y() < get<1>(biker2).source().y();
}

void testcase() {
    int n; cin >> n; // bikers
    
    vecir bikers = vecir(n); // index, trajectory
    for (int i = 0 ; i < n ; i++) {
        double y0; cin >> y0;
        double x1; cin >> x1;
        double y1; cin >> y1;
        
        bikers[i] = make_tuple(i, Ray(Point(0.0, y0), Point(x1, y1)));
    }
    
    sort(bikers.begin(), bikers.end(), asc_y0);
    
    vecb finishes(n, true);
    for (int i = 0 ; i < n ; i++) {
        int bi = get<0>(bikers[i]);
        if (finishes[bi]) {
            
            Ray ti = get<1>(bikers[i]);
            for (int j = i + 1 ; j < n ; j++) {
                int bj = get<0>(bikers[j]);
                if (finishes[bj]) {
                    
                    Ray tj = get<1>(bikers[j]);
                    if (do_intersect(ti, tj)) {
                        // 3 possible cases
                        // Case 1: Biker i is here before biker j -> biker j stops at this point
                        // Case 2: Biker i is here after biker j -> biker i stops at this point
                        // Case 3: Biker i and j arrive here at the same time -> the biker that started on the left (i.e. up) stops
                        
                        CGAL::Object overlap = intersection(ti, tj);
                        const Point *inter = object_cast<Point>(&overlap); // Cast is safe, since all y0s are different
                        
                        CGAL::Comparison_result distance_for_i = compare_distance_to_point(*inter, ti.source(), tj.source());
                        if (distance_for_i == CGAL::SMALLER) {
                            // Case 1
                            finishes[bj] = false;
                        } else if (distance_for_i == CGAL::LARGER) {
                            // Case 2
                            finishes[bi] = false;
                            break; // cause bi don't do shit no more
                        } else {
                            // Case 3
                            int who_stops = ti.source().y() < tj.source().y() ? bj : bi;
                            finishes[who_stops] = false;
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0 ; i < n ; i++) {
        if (finishes[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
