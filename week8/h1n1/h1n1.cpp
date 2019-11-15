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

map<Face, K::FT> max_allowed;
map<Face, bool> visited;

void dfs(Triangulation &t, Face &current) {
    // Marking this face as visited
    visited[current] = true;
    if (t.is_infinite(current)) {
        // Escaped succesfully
        return;
    }
    
    K::FT val = INT_MIN;
    for (int i = 0 ; i < 3 ; i++) {
        auto di = t.segment(current, i).squared_length();
        Face ni = current->neighbor(i);
        dfs(t, ni);
        
        if (visited[ni]) {
            val = max(val, min(di, max_allowed[ni]));
        } else {
            val = max(val, di);
        }
    }
    
    visited[current] = val;
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
    
    max_allowed = map<Face, K::FT>(); // Maximum distance allowed to escape from that face
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        max_allowed[it] = INT_MIN;
    }
    
    visited = map<Face, bool>(); // Keeping track of what vertices are already visited
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        // When on an infinite face, we already escaped
        visited[it] = false;
    }
    
    Face f = t.finite_faces_begin();
    dfs(t, f);
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        K::FT d; cin >> d;
        Point p(x, y);
        
        Face start = t.locate(p);
        
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
            // If i starts out too close to an infected person, they can't escape
            cout << "n";
        } else {
            cout << (max_allowed[start] >= (4 * d) ? "y" : "n");
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
