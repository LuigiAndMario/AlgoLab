#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

typedef boost::disjoint_sets_with_storage<> union_fuck;

using namespace std;

bool can_do(int cur, bool colour, vector<vector<int>> &graph, vector<int> &colouring) {
    colouring[cur] = colour;
    for (int i : graph[cur]) {
        if (colouring[i] == -1) {
            // Not coloured yet - try colouring it
            if (!can_do(i, !colour, graph, colouring)) {
                return false;
            }
            if (colouring[cur] == colouring[i]) {
                // Same colour - not possible
                return false;
            }
        }
    }
    
    return true;
}

bool two_colour(vector<vector<int>> &graph, int n) {
    vector<int> colouring(n, -1);
    for (int i = 0 ; i < n ; i++) {
        if (colouring[i] == -1) {
            if (!can_do(0, false, graph, colouring)) {
                return false;
            }
        }
    }
    
    return true;
}

void testcase() {
    int n; cin >> n; // # stations
    int m; cin >> m; // # clues
    int r; cin >> r; // Range of operation
    int r2 = r * r;
    
    vector<pair<Point, int>> s(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        s[i] = make_pair(Point(x, y), i); // Position of the ith station
    }
    
    Triangulation t;
    t.insert(s.begin(), s.end());
    
    vector<vector<int>> graph(n);
    union_fuck uf(n);
    for (auto it = t.finite_edges_begin() ; it != t.finite_edges_end() ; it++) {
        if (t.segment(*it).squared_length() <= r2) {
            int i1 = it->first->vertex((it->second + 1) % 3)->info();
            int i2 = it->first->vertex((it->second + 2) % 3)->info();
            
            graph[i1].push_back(i2);
            graph[i2].push_back(i1);
            
            uf.union_set(i1, i2);
        }
    }
    
    bool interference = !two_colour(graph, n);
    
    for (int i = 0 ; i < m ; i++) {
        int xa; cin >> xa;
        int ya; cin >> ya;
        Point a(xa, ya);
        int xb; cin >> xb;
        int yb; cin >> yb;
        Point b(xb, yb);
        
        if (interference) {
            // No two colouring possible
            cout << "n";
        } else if (CGAL::squared_distance(a, b) <= r2) {
            // Already in range
            cout << "y";
        } else {
            auto station_a = t.nearest_vertex(a);
            auto station_b = t.nearest_vertex(b);
            
            if (uf.find_set(station_a->info()) != uf.find_set(station_b->info())) {
                // Different components
                cout << "n";
            } else if (CGAL::squared_distance(station_a->point(), a) > r2 || CGAL::squared_distance(station_b->point(), b) > r2) {
                // One of them is not in range of the closest station
                cout << "n";
            } else {
                cout << "y";
            }
        }
    }
    
    cout << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
