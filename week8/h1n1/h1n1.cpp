#include <iostream>
#include <vector>
#include <map>
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

typedef boost::disjoint_sets_with_storage<> ds; // disjoint set

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
    
    int num_faces = 0;
    vector<Face> to_visit;
    for (auto it = t.finite_faces_begin() ; it != t.finite_faces_end() ; it++) {
        // We start by doing the trivial case - if a face has an infinite neighbour, it can escape there at no cost
        int  max_allowed = 0;
        for (int i = 0 ; i < 3 ; i++) {
            if (t.is_infinite(it->neighbor(i))) {
                int allowed = CGAL::squared_distance(it->vertex((i + 1) % 3)->point(), it->vertex((i + 2) % 3)->point());
                max_allowed = max(max_allowed, allowed);
            }
        }
        
        if (max_allowed != 0) {
            it->info() = max_allowed;
            to_visit.push_back(it);
        }
        
        num_faces++;
    }
    
    ds united(num_faces);
    // TODO: Start from every face next to an infinite face
    
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
