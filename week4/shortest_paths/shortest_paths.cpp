// STL includes
#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

#define VISUALISATION (false)

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        boost::no_property, // no vertex property
        boost::property<boost::edge_weight_t, int> // interior edge weight property
    > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
    int n; cin >> n;
    int m; cin >> m;
    int q; cin >> q;
    
    for (int i = 0 ; i < n ; i++) {
        int lon; cin >> lon;
        int lat; cin >> lat;
    }
    
    weighted_graph g(n);
    weight_map weights = boost::get(boost::edge_weight, g);
    edge_desc ed;
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a;
        int b; cin >> b;
        int c; cin >> c;
        
        ed = boost::add_edge(a, b, g).first;
        weights[ed] = c;
    }
    
    for (int i = 0 ; i < q ; i++) {
        int s; cin >> s;
        int t; cin >> t;
        
        std::vector<int> dist_map(n);
        std::vector<vertex_desc> pred_map(n);
        
        boost::dijkstra_shortest_paths(g, s,
            boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g)))
            .predecessor_map(boost::make_iterator_property_map( pred_map.begin(), boost::get(boost::vertex_index, g))));
        
        if (dist_map[t] < INT_MAX) {
            cout << dist_map[t] << endl;
            
            // Extras added for the visualisation
            if (VISUALISATION) {
                int cur = t;
                vector<int> path;
                path.push_back(cur);
                while (cur != s) {
                    cur = pred_map[cur];
                    path.push_back(cur);
                }
                reverse(path.begin(), path.end());
                
                cout << path.size() << " ";
                for (auto it = path.begin() ; it != path.end() ; it++) {
                    cout << *it << " ";
                }
                cout << endl;
            }
        } else {
            cout << "unreachable" << endl;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    testcase();
    return 0;
}
