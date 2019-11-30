#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <string>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;

typedef K::Point_2 Point;

typedef boost::disjoint_sets_with_storage<> ds; // disjoint set

using namespace std;

struct edge { // The indices of the two endpoints, and the squared length
    int from;
    int to;
    K::FT sq_length;
};

bool asc_dist(const edge &e1, const edge &e2) {
    return e1.sq_length < e2.sq_length;
}

void testcase() {
    int n; cin >> n; // # jammers
    int m; cin >> m; // # missions
    long p; cin >> p; // Initial power consumption
    
    vector<pair<Point, int> > jammers_with_idx(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        jammers_with_idx[i] = make_pair(Point(x, y), i);
        
    }
    
    vector<pair<Point, Point> > missions(m);
    for (int i = 0 ; i < m ; i++) {
        int x0; cin >> x0;
        int y0; cin >> y0;
        int x1; cin >> x1;
        int y1; cin >> y1;
        
        missions[i] = make_pair(Point(x0, y0), Point(x1, y1));
    }
    
    Triangulation t;
    t.insert(jammers_with_idx.begin(), jammers_with_idx.end());
    
    vector<edge> edges;
    for (auto it = t.finite_edges_begin() ; it != t.finite_edges_end() ; it++) {
        edge ed;
        ed.from = it->first->vertex((it->second + 1) % 3)->info();
        ed.to = it->first->vertex((it->second + 2) % 3)->info();
        ed.sq_length = t.segment(it).squared_length();
        edges.push_back(ed);
    }
    sort(edges.begin(), edges.end(), asc_dist);
    
    // 1. Checking whether or not missions are doable as-is
    ds zones(n); // zones within which it is safe to operate (given a power consumption of p)
    // We only store the indices of every jammer
    for (auto it = edges.begin() ; it != edges.end() ; it++) {
        if (it->sq_length > p) {
            // Given that we sorted by ascending p, everything coming next is above p as well
            break;
        }
        // Otherwise, both ends of this edge are in the same zone
        zones.union_set(it->from, it->to);
    }
    
    string can_do;
    K::FT a = 0; // smallest power consumption required for ALL missions (i.e. biggest power requirement of any mission)
    auto ita = edges.begin();
    ds zones_a(n); // zones, but for a instead of p
    
    K::FT b = 0; // smallest power consumption such that we can do the same missions as we can with p
    // i.e. if we can do something with p, then we need to be able to do it with b. Also, b must be minimal, and not above p.
    auto itb = edges.begin();
    ds zones_b(n); // zones, but for b instead of p
    
    for (auto it = missions.begin() ; it != missions.end() ; it++) {
        Point p0 = it->first;
        Vertex v0 = t.nearest_vertex(p0);
        K::FT d0 = CGAL::squared_distance(p0, v0->point());
        
        Point p1 = it->second;
        Vertex v1 = t.nearest_vertex(p1);
        K::FT d1 = CGAL::squared_distance(p1, v1->point());
        
        K::FT p_required = 4 * max(d0, d1);
        
        if (p_required <= p && zones.find_set(v0->info()) == zones.find_set(v1->info())) {
            cout << "y";
            
            b = max(b, p_required);
            // While it is still not possible to go from p0 to p1 in the zones_b, we put zones together
            while (itb != edges.end() && zones_b.find_set(v0->info()) != zones_b.find_set(v1->info())) {
                zones_b.union_set(itb->from, itb->to);
                itb++;
            }
        } else {
            cout << "n";
        }
        
        a = max(a, p_required);
        // While it is still not possible to go from p0 to p1 in the zones_a, we put zones together
        while (ita != edges.end() && zones_a.find_set(v0->info()) != zones_a.find_set(v1->info())) {
            zones_a.union_set(ita->from, ita->to);
            ita++;
        }
    }
    
    // If we expanded a's zones, then we must take the new value (squared distance) if it is larger
    if (ita != edges.begin()) {
        a = max(a, (ita - 1)->sq_length);
    }
    // If we expanded b's zones, then we must take the new value (squared distance) if it is larger
    if (itb != edges.begin()) {
        b = max(b, (itb - 1)->sq_length);
    }
    
    cout << endl;
    cout << a << endl;
    cout << b << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}