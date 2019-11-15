#include <iostream>
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;

typedef K::Point_2 Point;

using namespace std;

double ceil_to_double(K::FT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
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
    map<Face, int> visited; // keeps track of the faces that have already been visited
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        visited[it] = -1;
    }
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        Point p(x, y);
        K::FT d; cin >> d;
        
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
            // If i starts out too close to an infected person, they can't escape
            cout << "n";
            continue;
        }
        
        bool possible = false;
        vector<Face> to_visit(0);
        to_visit.push_back(t.locate(p));
        while (to_visit.size() != 0) {
            // Getting a face to visit
            Face f = to_visit.back();
            to_visit.pop_back();
            
            // If the face is infinite, then we escaped
            if (t.is_infinite(f)) {
                possible = true;
                break;
            }
            
            // Marking the face as visited
            visited[f] = i;
            
            // Finding all adjacent faces
            // Neighbour 0 has vertices 1 and 2 in common
            // Neighbour 1 has vertices 2 and 0 in common
            // Neighbour 2 has vertices 0 and 1 in common
            if (CGAL::squared_distance(f->vertex(1)->point(), f->vertex(2)->point()) >= (4 * d)) {
                Face neighbour = f->neighbor(0);
                if (visited[neighbour] < i) {
                    to_visit.push_back(neighbour);
                }
            }
            if (CGAL::squared_distance(f->vertex(2)->point(), f->vertex(0)->point()) >= (4 * d)) {
                Face neighbour = f->neighbor(1);
                if (visited[neighbour] < i) {
                    to_visit.push_back(neighbour);
                }
            }
            if (CGAL::squared_distance(f->vertex(0)->point(), f->vertex(1)->point()) >= (4 * d)) {
                Face neighbour = f->neighbor(2);
                if (visited[neighbour] < i) {
                    to_visit.push_back(neighbour);
                }
            }
        }
        
        cout << (possible ? "y" : "n");
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
