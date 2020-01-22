#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;

int index_of(int i, int j, int k) {
    // Returns the index of the vertex vij (given k)
    if (i <= 0 || i > k || j <= 0 || j > i) {
        return -1; // Out of bounds
    }
    
    return j + (((i - 1) * i) / 2) - 1;
}

void testcase() {
    int k; cin >> k; // # layers
    int n = (k * (k + 1)) / 2;
    
    weighted_graph G(2 * n);
    weight_map weights_m = boost::get(boost::edge_weight, G);
    vector<int> weights(n);
    
    for (int i = 1 ; i <= k ; i++) {
        for (int j = 1 ; j <= i ; j++) {
            int v = index_of(i, j, k);
            int v_in = 2 * v;
            int v_out = (2 * v) + 1;
            
            int vij; cin >> vij;
            weights[v] = vij;
            
            edge_desc ed = boost::add_edge(v_in, v_out, G).first;
            weights_m[ed] = vij;
            
            vector<int> neighbours(6);
            neighbours[0] = index_of(i - 1, j - 1, k);
            neighbours[1] = index_of(i - 1, j, k);
            neighbours[2] = index_of(i, j - 1, k);
            neighbours[3] = index_of(i, j + 1, k);
            neighbours[4] = index_of(i + 1, j, k);
            neighbours[5] = index_of(i + 1, j + 1, k);
            
            for (auto neighbour : neighbours) {
                if (neighbour != -1) {
                    ed = boost::add_edge(v_out, 2 * neighbour, G).first;
                    weights_m[ed] = 0;
                }
            }
        }
    }
    
    // Idea: for all the vertices, sum the shortest paths to v11, vk1, and vkk, and add its weight
    
    const int v11_in = 2 * index_of(1, 1, k);
    const int vk1_in = 2 * index_of(k, 1, k);
    const int vkk_in = 2 * index_of(k, k, k);
    
    std::vector<int> dist_map_v11(2 * n);
    boost::dijkstra_shortest_paths(G, v11_in, boost::distance_map(boost::make_iterator_property_map(dist_map_v11.begin(), boost::get(boost::vertex_index, G))));
    
    std::vector<int> dist_map_vk1(2 * n);
    boost::dijkstra_shortest_paths(G, vk1_in, boost::distance_map(boost::make_iterator_property_map(dist_map_vk1.begin(), boost::get(boost::vertex_index, G))));
    
    std::vector<int> dist_map_vkk(2 * n);
    boost::dijkstra_shortest_paths(G, vkk_in, boost::distance_map(boost::make_iterator_property_map(dist_map_vkk.begin(), boost::get(boost::vertex_index, G))));
    
    int best = INT_MAX;
    for (int i = 1 ; i <= k ; i++) {
        for (int j = 1 ; j <= i ; j++) {
            int v = index_of(i, j, k);
            
            best = min(best, dist_map_v11[2 * v] + dist_map_vk1[2 * v] + dist_map_vkk[2 * v] + weights[v]);
        }
    }
    
    cout << best << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
