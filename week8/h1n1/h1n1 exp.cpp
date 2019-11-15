#include <iostream>
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Face_handle Face;

typedef K::Point_2 Point;

using namespace std;

map<Face, int> visited;

bool can_escape(Triangulation &t, K::FT d, Face &current, int candidate) {
    // Marking this face as visited
    visited[current] = candidate;
    if (t.is_infinite(current)) {
        // Escaped succesfully
        return true;
    }
    
    for (int i = 0 ; i < 3 ; i++) {
        auto edge = t.segment(current, i);
        if (edge.squared_length() >= (4 * d)) { // is candidate can go through here
            Face f = current->neighbor(i);
            if (visited[f] < candidate && can_escape(t, d, f, candidate)) {
                // If we have not visited this face
                return true;
            }
        }
    }
    
    return false;
}

void testcase(int n) {
    vector<Point> infected(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        infected[i] = Point(x, y);
    }
    
    Triangulation t;
    t.insert(infected.begin(), infected.end());
    visited = map<Face, int>(); // keeps track of the faces that have already been visited
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        visited[it] = -1;
    }
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        K::FT d; cin >> d;
        Point p(x, y);
        
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
            // If i starts out too close to an infected person, they can't escape
            cout << "n";
        } else {
            Face start = t.locate(p);
            if (can_escape(t, d, start, i)) {
                cout << "y";
            } else {
                cout << "n";
            }
        }
        
    }
    
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
    return 0;
}
