#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator               edge_it;

using namespace std;

void testcase() {
    int n; cin >> n; // # planets
    int i; cin >> i; // Tatooine (irrelevant tho)
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    vector<vector<int>> costs(n, vector<int>(n)); // [j][k] = cost for link from j to k
    for (int j = 1 ; j <= n - 1 ; j++) {
        for (int k = 1 ; k <= n - j ; k++) {
            int cost; cin >> cost;
            costs[j][k] = cost;
            
            edge_desc e = boost::add_edge(j, j + k, G).first;
            weights[e] = cost;
        }
    }
    
    std::vector<edge_desc> leia;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(leia));
    
    int cost = 0;
    for (auto e: leia) {
        cost += weights[e];
    }
    
    int arturito_cost = INT_MAX;
    for (auto e: leia) {
        weighted_graph Ga(n);
        weight_map weightsa = boost::get(boost::edge_weight, Ga);
        
        edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
            if (*ebeg != e) {
                int from = boost::source(*ebeg, G);
                int to = boost::target(*ebeg, G);
                edge_desc ea = boost::add_edge(from, to, Ga).first;
                weightsa[ea] = weights[*ebeg];
            }
        }
        
        std::vector<edge_desc> arturito;
        boost::kruskal_minimum_spanning_tree(Ga, std::back_inserter(arturito));
        
        if (arturito.size() != leia.size()) {
            continue;
        }
        
        int new_cost = 0;
        for (auto ea: arturito) {
            new_cost += weightsa[ea];
        }
        arturito_cost = min(arturito_cost, new_cost);
    }
    
    cout << arturito_cost << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
