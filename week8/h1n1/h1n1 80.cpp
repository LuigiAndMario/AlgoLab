#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

using namespace std;

char escape(Triangulation &t, Face &start, K::FT &d, int i) {
    vector<Face> to_visit(0);
    to_visit.push_back(start);
    
    while (!to_visit.empty()) {
        Face f = to_visit.back();
        to_visit.pop_back();
        
        if (f->info() < i) {
            // Face not visited yet
            f->info() = i;
            
            if (t.is_infinite(f)) {
                // Succesfully escaped
                return 'y';
            }
            
            for (int j = 0 ; j < 3 ; j++) {
                Segment edge = t.segment(make_pair(f, j));
                if (edge.squared_length() >= 4 * d) {
                    to_visit.push_back(f->neighbor(j));
                }
            }
        }
    }
    
    return 'n';
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
    
    // Setting up DFS
    for (auto it = t.all_faces_begin() ; it != t.all_faces_end() ; it++) {
        it->info() = -1;
    }
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        K::FT x; cin >> x;
        K::FT y; cin >> y;
        K::FT d; cin >> d;
        
        Point p(x, y);
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
            cout << "n";
        } else {
            Face start = t.locate(p);
            cout << escape(t, start, d, i);
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
