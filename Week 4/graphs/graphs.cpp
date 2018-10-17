#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Edge_iter;
typedef boost::property_map<Graph, boost::edge_weight_t>::type Weight_map;

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;

    Graph graph(n);
    Weight_map weights = get(boost::edge_weight, graph);
    for (int i = 0 ; i < m ; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        int w; std::cin >> w;

        Edge e;
        bool done;
        tie(e, done) = add_edge(u, v, graph);
        weights[e] = w;
    }

    std::vector<Edge> mst;
    boost::kruskal_minimum_spanning_tree(graph, back_inserter(mst));

    int w = 0; // To output (sum of weights)
    for (std::vector<Edge>::iterator iter = mst.begin() ; iter != mst.end() ; iter++) {
        w += boost::get(boost::edge_weight_t(), graph, *iter);
    }

    std::vector<int> dists(n);
    std::vector<Vertex> prevs(n);

    dijkstra_shortest_paths(graph, 0,
            predecessor_map(make_iterator_property_map(prevs.begin(), get(boost::vertex_index, graph))).
                distance_map(make_iterator_property_map(dists.begin(), get(boost::vertex_index, graph))));
    int d = INT_MIN;
    for (int i = 0 ; i < n ; i++) {
        if (dists[i] > d && dists[i] < INT_MAX)
            d = dists[i];
    }

    std::cout << w << " " << d << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
