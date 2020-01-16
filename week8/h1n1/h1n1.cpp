#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property<boost::edge_weight_t, long> weight_prop;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;

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
    
    long cur = 0;
    int num_faces = 0;
    for (auto it = t.finite_faces_begin() ; it != t.finite_faces_end() ; it++) {
        it->info() = cur++;
        num_faces++;
    }
    
    // Modelling the triangulation as a graph ; the vertices (in the graph) are faces, and the edges are passages between one face to another
    weighted_graph g(num_faces + 1); // + 1 for the infinite vertex
    const int INF_VERTEX = num_faces;
    for (auto it = t.finite_faces_begin() ; it != t.finite_faces_end() ; it++) {
        for (int i = 0 ; i < 3 ; i++) {
            Face ni = it->neighbor(i);
            
            Vertex v1 = it->vertex((i + 1) % 3);
            Vertex v2 = it->vertex((i + 2) % 3);
            long d_allowed = CGAL::squared_distance(v1->point(), v2->point());
            
            if (t.is_infinite(ni)) {
                boost::add_edge(it->info(), INF_VERTEX, weight_prop(LONG_MAX - d_allowed), g);
            } else if (ni->info() >= it->info()) {
                // Not yet visited
                boost::add_edge(it->info(), ni->info(), weight_prop(LONG_MAX - d_allowed), g);
            }
        }
    }
    
    vector<long> dist_map(num_faces + 1);
    vector<vertex_desc> pred_map(num_faces + 1);
    
    boost::prim_minimum_spanning_tree(g, boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, g)), boost::root_vertex(INF_VERTEX).distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g))));
    
    
    int m; cin >> m;
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        K::FT d; cin >> d;
        
        Point p(x, y);
        Face f = t.locate(p);
        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
            cout << "n";
            continue;
        }
        
        if (t.is_infinite(f)) {
            cout << "y";
            continue;
        }
        
        vertex_desc cur = f->info();
        while (cur != INF_VERTEX && LONG_MAX - dist_map[cur] >= 4 * d) {
            cur = pred_map[cur];
        }
        
        cout << (cur == INF_VERTEX ? "y" : "n");
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
