#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/enum.h>
#include <CGAL/Direction_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Direction_2 Direction;

typedef tuple<int, Ray, Direction> biker;

typedef vector<biker> vecird;
typedef vector<bool> vecb;

K::RT absolute(K::RT x) {
    return x > 0 ? x : -x;
}

bool asc_y0(const biker &biker1, const biker &biker2) {
    return get<1>(biker1).source().y() < get<1>(biker2).source().y();
}

bool original_order(const biker &biker1, const biker &biker2) {
    return get<0>(biker1) < get<0>(biker2);
}

bool asc_slope(const biker &biker1, const biker &biker2) {
    // Returns true if the angle of biker1 is closer to 90 than the angle of biker2
    K::RT dy1 = get<2>(biker1).dy();
    K::RT dx1 = get<2>(biker1).dx();
    K::RT dy2 = get<2>(biker2).dy();
    K::RT dx2 = get<2>(biker2).dx();
    return absolute(dy1 * dx2) < absolute(dy2 * dx1);
}

bool same_slope(const biker &biker1, const biker &biker2) {
    // Returns true if biker 1 and biker 2 have the same slope
    K::RT dy1 = get<2>(biker1).dy();
    K::RT dx1 = get<2>(biker1).dx();
    K::RT dy2 = get<2>(biker2).dx();
    K::RT dx2 = get<2>(biker2).dx();
    return absolute(dy1 * dx2) == absolute(dy2 * dx1);
}

void testcase() {
    int n; cin >> n; // bikers
    
    vecird bikers = vecird(n); // index, trajectory, direction
    for (int i = 0 ; i < n ; i++) {
        double y0; cin >> y0;
        double x1; cin >> x1;
        double y1; cin >> y1;
        
        Ray r = Ray(Point(0.0, y0), Point(x1, y1));
        bikers[i] = make_tuple(i, r, r.direction());
    }
    
    sort(bikers.begin(), bikers.end(), asc_y0);
    
    
    
    // NOTES
    /*
     Keep a stack of riding bikers - just remove the ones below that stop
     */
    vecird riding(0);
    riding.push_back(bikers[0]);
    int i = 1;
    while (i < n) {
        biker bi = bikers[i];
        
        bool ride = true;
        while (!riding.empty() && ride) {
            // As long as there are bikers to take care of, we continue
            biker bj = riding[riding.size() - 1];
            if (do_intersect(get<1>(bi), get<1>(bj))) {
                if (asc_slope(bi, bj)) {
                    // Biker i has a smaller slope than biker j -> biker i is closer to a straight line -> biker i was here first
                    riding.pop_back();
                } else {
                    // Either the slopes are the same -> biker i came from above (i.e. left) and therefore stops,
                    // or biker i has a bigger slope -> biker j was here first
                    ride = false;
                }
            } else {
                break;
            }
        }
        if (ride) {
            riding.push_back(bi);
        }
        
        i++;
    }
    
    sort(riding.begin(), riding.end(), original_order);
    for (auto it = riding.begin() ; it < riding.end() ; it++) {
        cout << get<0>(*it) << " ";
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
