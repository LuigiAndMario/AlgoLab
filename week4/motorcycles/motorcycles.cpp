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

vector<int> indices;
vector<Ray> trajectories;
vector<K::FT> slopes;
vector<double> y0s;

bool asc_y0(int i, int j) {
    return y0s[i] < y0s[j];
}

void testcase() {
    int n; cin >> n; // bikers
    
    indices = vector<int>(n);
    trajectories = vector<Ray>(n);
    slopes = vector<K::FT>(n);
    y0s = vector<double>(n);
    for (int i = 0 ; i < n ; i++) {
        double y0; cin >> y0;
        double x1; cin >> x1;
        double y1; cin >> y1;
        
        y0s[i] = y0;
        Ray r = Ray(Point(0.0, y0), Point(x1, y1));
        indices[i] = i;
        trajectories[i] = r;
        slopes[i] = r.direction().dy() / r.direction().dx();
    }
    
    sort(indices.begin(), indices.end(), asc_y0);
    
    // IDEA: Keep a stack of riding bikers - just remove the ones below that stop
    vector<int> riding(0);
    riding.push_back(indices[0]);
    int i = 1;
    while (i < n) {
        int bi = indices[i];
        
        bool ride = true;
        while (!riding.empty() && ride) {
            // As long as there are bikers to take care of, we continue
            int bj = riding.back();
            if (do_intersect(trajectories[bi], trajectories[bj])) {
                if (slopes[bi] + slopes[bj] > 0) {
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
    
    sort(riding.begin(), riding.end());
    for (auto it = riding.begin() ; it < riding.end() ; it++) {
        cout << *it << " ";
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
