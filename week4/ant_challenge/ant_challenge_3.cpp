// STL includes
#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, // no vertex property
        boost::property<boost::edge_weight_t, int> // interior edge weight property
    > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef vector<int> vec;

void testcase() {
    int n; cin >> n; // trees
    int e; cin >> e; // edges
    int s; cin >> s; // species
    int a; cin >> a; // from
    int b; cin >> b; // to
    
    // Third test assumptions: s = 1
    // assert(s == 1);
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    edge_desc ed;
    for (int i = 0 ; i < e ; i++) {
        int t1; std::cin >> t1;
        int t2; std::cin >> t2;
        vec w(s);
        int w_min = INT_MAX;
        for (int i = 0 ; i < s ; i++) {
            std::cin >> w[i];
            w_min = min(w_min, w[i]);
        }
        
        ed = boost::add_edge(t1, t2, G).first;
        weights[ed] = w_min;
    }
    
    vec h(s);
    for (int i = 0 ; i < s ; i++) {
        cin >> h[i];
    }
    
    vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    weighted_graph new_G(n);
    weight_map new_weights = boost::get(boost::edge_weight, G);
    for (auto it = mst.begin() ; it != mst.end() ; it++) {
        ed = boost::add_edge(boost::source(*it, G), boost::target(*it, G), new_G).first;
        new_weights[ed] = weights[*it];
    }
    
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);
    
    boost::dijkstra_shortest_paths(new_G, b,
        boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G)))
        .predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, G))));
    
    cout << dist_map[a] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
