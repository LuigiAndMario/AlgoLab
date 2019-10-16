// STL includes
#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, // no vertex property
        boost::property<boost::edge_weight_t, int> // interior edge weight property
    > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    edge_desc e;
    for (int i = 0 ; i < m ; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        int w; std::cin >> w;
        
        e = boost::add_edge(u, v, G).first;
        weights[e] = w;
    }
    
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    // Weights
    int w = 0;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); it++) {
//        w += weights[*it];
        w += boost::get(boost::edge_weight_t(), G, *it);
    }
    
    // Distance
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);
    
    
    boost::dijkstra_shortest_paths(G, 0, boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G)))
            /* dot! */ .predecessor_map(boost::make_iterator_property_map( pred_map.begin(), boost::get(boost::vertex_index, G))));

    int d = INT_MIN;
    for (int i = 0 ; i < n ; i++) {
        if (dist_map[i] > d && dist_map[i] < INT_MAX) {
            d = dist_map[i];
        }
    }
    
    std::cout << w << " " << d << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
