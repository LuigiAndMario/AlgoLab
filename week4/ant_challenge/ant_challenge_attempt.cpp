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
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef vector<int> vec;

void testcase() {
    int n; cin >> n; // trees
    int e; cin >> e; // edges
    int s; cin >> s; // species
    int a; cin >> a; // from
    int b; cin >> b; // to
    
    vector<weighted_graph> G(s, weighted_graph(n));
    vector<weight_map> weights(s);
    for (int i = 0 ; i < s ; i++) {
        weights[i] = boost::get(boost::edge_weight, G[i]);
    }
    edge_desc ed;
    for (int i = 0 ; i < e ; i++) {
        int t1; std::cin >> t1;
        int t2; std::cin >> t2;
        for (int j = 0 ; j < s ; j++) {
            ed = boost::add_edge(t1, t2, G[j]).first;
            std::cin >> weights[j][ed];
        }
    }
    
    vec h(s);
    for (int i = 0 ; i < s ; i++) {
        cin >> h[i];
    }
    
    weighted_graph G_best(n);
    weight_map weights_best = boost::get(boost::edge_weight, G_best);
    for (int i = 0 ; i < s ; i++) {
        vector<edge_desc> mst;
        boost::kruskal_minimum_spanning_tree(G[i], std::back_inserter(mst));
        
        for (auto it = mst.begin() ; it != mst.end() ; it++) {
            ed = boost::add_edge(source(*it, G[i]), target(*it, G[i]), G_best).first;
            weights_best[ed] = weights[i][*it];
        }
    }
//    vector<vector<edge_desc> > msts(s);
//    for (int i = 0 ; i < s ; i++) {
//        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(msts[i]));
//    }
//
//    weighted_graph new_G(n);
//    weight_map new_weights = boost::get(boost::edge_weight, new_G);
//    map<edge_desc, bool> is_set;
//    for (int i = 0 ; i < s ; i++) {
//        for (auto it = msts[i].begin() ; it != msts[i].end() ; it++) {
//            ed = boost::add_edge(boost::source(*it, G), boost::target(*it, G), new_G).first;
//            if (!is_set[ed]) {
//                new_weights[ed] = weights[*it];
//                is_set[ed] = true;
//            } else {
//                new_weights[ed] = min(new_weights[ed], weights[*it]);
//            }
//        }
//    }
//
//    vector<edge_desc> mst;
//    boost::kruskal_minimum_spanning_tree(new_G, std::back_inserter(mst));
    
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);
    
    boost::dijkstra_shortest_paths(G_best, b,
        boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G_best)))
        .predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, G_best))));
    
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
