#include <iostream>
#include <vector>
#include<algorithm>
#include<climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::allow_parallel_edge_tag, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Edge_iterator;
typedef boost::property_map<Graph, boost::edge_weight_t>::type Weight_map;

void testcase() {
    int n; std::cin >> n; // Trees (vertices)
    int e; std::cin >> e; // Edges
    int s; std::cin >> s; // Species
    int a; std::cin >> a; // Start
    int b; std::cin >> b; // Finish

    Graph species_networks[s + 1];
    Weight_map weights[s + 1];

    for (int i = 0 ; i < s ; i++) {
        species_networks[i] = Graph(n);
        weights[i] = get(boost::edge_weight, species_networks[i]);
    }

    for (int i = 0 ; i < e ; i++) {
        int t_1; std::cin >> t_1;
        int t_2; std::cin >> t_2;

        Edge ed;
        bool done;
        for (int j = 0 ; j < s ; j++) {
            tie(ed, done) = boost::add_edge(t_1, t_2, species_networks[j]);
            std::cin >> weights[j][ed];
        }
    }
    
    int null;
    for(int i = 0 ; i < s ; i++){
        std::cin >> null; // We use kruskal instead
    }

    for (int i = 0 ; i < s ; i++) {
        std::vector<Edge> mst;
        kruskal_minimum_spanning_tree(species_networks[i], back_inserter(mst));
    
        for (std::vector<Edge>::iterator iter = mst.begin() ; iter != mst.end() ; iter++) {
            Edge ed;
            bool done;
            tie(ed, done) = boost::add_edge(source(*iter, species_networks[i]), target(*iter, species_networks[i]), species_networks[s]);
            weights[s][ed] = get(boost::edge_weight_t(), species_networks[i], *iter);
        }
    }

    std::vector<int> dists(n);
    std::vector<Vertex> prevs(n);

    dijkstra_shortest_paths(species_networks[s], a,
            predecessor_map(make_iterator_property_map(prevs.begin(), get(boost::vertex_index, species_networks[s]))).
                distance_map(make_iterator_property_map(dists.begin(), get(boost::vertex_index, species_networks[s]))));

    std::cout << dists[b] << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
