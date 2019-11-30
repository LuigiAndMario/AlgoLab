#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;

typedef struct player {
    Point p; // Position
    double r; // Thiccness
} player;


void testcase() {
    int m; cin >> m; // # players
    int n; cin >> n; // # lamps
    
    vector<player> p(m);
    for (int i = 0 ; i < m ; i++) {
        double x; cin >> x;
        double y; cin >> y;
        double r; cin >> r;
    
        p[i] = {Point(x, y), r};
    }
    
    int h; cin >> h; // Height of the lamps
    
    vector<pair<Point, double> > l(n);
    for (int i = 0 ; i < n ; i++) {
        double x; cin >> x;
        double y; cin >> y;
        
        l[i] = make_pair(Point(x, y), i);
    }
    
    Triangulation t;
    t.insert(l.begin(), l.end());
    
    vector<int> hit_player(m, INT_MAX); // player -> lamp that hit him
    
    for (int i = 0 ; i < m ; i++) {
        Point pi = p[i].p; // Position
        double r = p[i].r; // Thiccness
        
        Vertex lamp = t.nearest_vertex(pi);
        
        double d_min = (h + r) * (h + r); // d_min to stay in the game is h + r - but squared because no sqrt
        double d_ptl = CGAL::squared_distance(pi, lamp->point()); // distance from player to lamp
        
        if (d_ptl < d_min) {
            // Player i just got rekt by this lamp, but maybe they got rekt by a lamp that was turned on before ?
            hit_player[i] = lamp->info();
            for (int j = 0 ; j < lamp->info() ; j++) {
                double d_ptj = CGAL::squared_distance(pi, l[j].first);
                
                if (d_ptj < d_min) {
                    hit_player[i] = j;
                    break;
                }
            }
        }
    }
    
    int last_hit_at = -1;
    for (int i = 0 ; i < m ; i++) {
        last_hit_at = max(last_hit_at, hit_player[i]);
        if (hit_player[i] == INT_MAX) {
            // Player i survived till the end
            cout << i << " ";
        }
    }
    
    if (last_hit_at == INT_MAX) {
        // At least one player stayed in the game till the end, so we're done
        cout << endl;
    } else {
        // No one stayed in the game, so we have to lower our expectation
        for (int i = 0 ; i < m ; i++) {
            if (hit_player[i] == last_hit_at) {
                cout << i << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
