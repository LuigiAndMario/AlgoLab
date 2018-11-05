#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<
    boost::edge_capacity_t, int, boost::property<boost::edge_residual_capacity_t, int, boost::property<
        boost::edge_reverse_t, Traits::edge_descriptor> > > > Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type	 EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type	ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor Edge;

// Copied from the slides
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap	&capacitymap;
    ReverseEdgeMap	&revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void testcase() {
    int n; std::cin >> n; // # players
    int m; std::cin >> m; // # rounds

    Graph graph(n);
    EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, graph);
    ReverseEdgeMap reverse_edge_map = boost::get(boost::edge_reverse, graph);
    EdgeAdder edge_adder(graph, capacity_map, reverse_edge_map);

    Vertex s = boost::add_vertex(graph);
    Vertex t = boost::add_vertex(graph);

    int nodes_in_flow_network = n + 2;
    for (int i = 0 ; i < m ; i++) {
        int a; std::cin >> a;
        int b; std::cin >> b;
        int c; std::cin >> c;

        edge_adder.addEdge(s, nodes_in_flow_network, 1);

        if (c == 0 || c == 1) {
            // a won, or unsure
            edge_adder.addEdge(nodes_in_flow_network, a, 1);
        }
        if (c == 0 || c == 2) {
            // b won, or unsure
            edge_adder.addEdge(nodes_in_flow_network, b, 1);
        }
        
        nodes_in_flow_network++;
    }

    int sum = 0;
    for (int i = 0 ; i < n ; i++) {
        int s_i; std::cin >> s_i;
        edge_adder.addEdge(i, t, s_i);
        sum += s_i;
    }

    std::cout << (boost::push_relabel_max_flow(graph, s, t) == std::max(sum, m) ? "yes" : "no") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
