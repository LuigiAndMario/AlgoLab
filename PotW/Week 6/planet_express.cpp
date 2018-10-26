#include <iostream>
#include <vector>
#include <set>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define MAX_MILLISECONDS 1000000
#define OMICRON_PERSEI_8 (n - 1)

using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIterator;

typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase() {
    int n; std::cin >> n; // # planets
    int m; std::cin >> m; // # edges
    int k; std::cin >> k; // # warehouses (vertices 0, ..., k - 1)
    int T; std::cin >> T; // # planets in teleportation network
    
    std::vector <int> teleportation_network(T);
    for (int i = 0 ; i < T ; i++) {
        std::cin >> teleportation_network[i];
    }
    
    Graph galaxy(n);
    WeightMap weights = get(edge_weight, galaxy);
    for (int i = 0 ; i < m ; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        int c; std::cin >> c;
        
        Edge e;
        bool success;
        tie(e, success) = add_edge(v, u, galaxy);
        weights[e] = c;
    }
    
    std::vector<int> component(num_vertices(galaxy));
    int nb_scc = strong_components(galaxy, make_iterator_property_map(component.begin(), get(vertex_index, galaxy)));
    
    std::vector<std::set<int> > eligible_for_teleportation(nb_scc); // SCC -> elements of the SCC
    for (int i = 0 ; i < T ; i++) {
        eligible_for_teleportation[component[teleportation_network[i]]].insert(teleportation_network[i]);
    }
    
    for (int i = 0 ; i < nb_scc ; i++) {
        int weight = eligible_for_teleportation[i].size() - 1;
        Vertex mass_relay = add_vertex(galaxy);
        for (std::set<int>::iterator it = eligible_for_teleportation[i].begin() ; it != eligible_for_teleportation[i].end() ; it++) {
            Edge e;
            bool success;
            tie(e, success) = add_edge(mass_relay, *it, galaxy);
            weights[e] = weight;
            tie(e, success) = add_edge(*it, mass_relay, galaxy);
            weights[e] = 0;
        }
        
//        Correct, but can run in up to 21 seconds for test3
//        for (std::set<int>::iterator it_1 = eligible_for_teleportation[i].begin() ; it_1 != eligible_for_teleportation[i].end() ; it_1++) {
//            for (std::set<int>::iterator it_2 = eligible_for_teleportation[i].begin() ; it_2 != eligible_for_teleportation[i].end() ; it_2++) {
//                if (it_1 != it_2) {
//                    // We add an edge between every two distinct of this SCC
//                    Edge e;
//                    bool success;
//                    tie(e, success) = add_edge(*it_2, *it_1, galaxy);
//                    weights[e] = weight;
//                }
//            }
//        }
    }
    
    // We get the shortest paths from Omicron Persei 8
    std::vector<Vertex> predmap(n + nb_scc);
    std::vector<int> distmap(n + nb_scc);
    dijkstra_shortest_paths(galaxy, OMICRON_PERSEI_8,
                            predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, galaxy))).
                            distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, galaxy))));
    
    // And find the best warehouse to ship from
    int shortest = INT_MAX;
    for (int i = 0 ; i < k ; i++) {
        shortest = std::min(shortest, distmap[i]);
    }
    
    if (shortest <= MAX_MILLISECONDS) {
        std::cout << shortest << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
