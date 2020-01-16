#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Face_handle Face;

typedef K::Point_2 Point;

using namespace std;

void testcase(int n) {
    vector<Point> infected(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        infected[i] = Point(x, y);
    }
    
    Triangulation t;
    t.insert(infected.begin(), infected.end());
    
    map<Face, K::FT> visited; // Keeps track of the faces that have already been visited
    priority_queue<pair<K::FT, Face>> where_to_next;
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        if (t.is_infinite(it)) {
            where_to_next.push(make_pair(INT_MAX, it));
        }
    }
    
    // Getting the max allowed distance for each face
    while (!where_to_next.empty()) {
        auto cur = where_to_next.top();
        where_to_next.pop();
        
        if (visited.count(cur.second) == 0) {
            visited[cur.second] = cur.first;
            for (int i = 0 ; i < 3 ; i++) {
                Face neighbor_i = cur.second->neighbor(i);
                if (visited.count(neighbor_i) == 0) {
                    K::FT allowance = CGAL::squared_distance(
                                                             cur.second->vertex((i + 1) % 3)->point(),
                                                             cur.second->vertex((i + 2) % 3)->point());
                    
                    where_to_next.push(make_pair(min(cur.first, allowance), neighbor_i));
                }
            }
        }
    }
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        K::FT d; cin >> d;
        Point p(x, y);
        
        Face f = t.locate(p);
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d || visited[f] < 4 * d) {
            cout << "n";
        } else {
            cout << "y";
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
