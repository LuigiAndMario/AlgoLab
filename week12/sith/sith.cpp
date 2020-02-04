#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef K::Point_2 Point;

typedef boost::disjoint_sets_with_storage<> Uf;

using namespace std;

vector<pair<Point, int>> planets;

void testcase() {
    int n; cin >> n;
    K::FT r; cin >> r;
    K::FT r_sq = r * r;
    
    planets = vector<pair<Point, int>>(n); // point, blob index
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        Point p(x, y);
        
        planets[i] = make_pair(p, i);
    }
    
    int bottom = 0;
    int top = n - 1;
    while (bottom <= top) {
        int mid = (bottom + top) / 2;
        
        Triangulation tri;
        tri.insert(planets.begin() + mid, planets.end());
        
        Uf blobs(n);
        for (auto it = tri.finite_edges_begin() ; it != tri.finite_edges_end() ; it++) {
            Triangulation::Edge e = *it;
            Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);
            Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);
            
            if (tri.segment(e).squared_length() <= r_sq) {
                blobs.union_set(v1->info(), v2->info());
            }
        }
        
        vector<int> blob_sizes(n, 0);
        for (int j = 0 ; j < n ; j++) {
            blob_sizes[blobs.find_set(j)]++;
        }
        
        int largest_blob = *max_element(blob_sizes.begin(), blob_sizes.end());
        int empire = mid; // We start at planet i, so the empire has planets 0, ..., i - 1, so i planets in total
        if (largest_blob < empire) {
            top = mid - 1;
        } else {
            bottom = mid + 1;
        }
    }
    
    cout << bottom - 1 << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
