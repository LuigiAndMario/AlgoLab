#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<
    boost::edge_capacity_t, int, boost::property<boost::edge_residual_capacity_t, int, boost::property<
        boost::edge_reverse_t, Traits::edge_descriptor> > > > Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
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

void tescase() {
    int n; std::cin >> n; // # vertices (intersections)
    int m; std::cin >> m; // # edges (streets)
    int s; std::cin >> s; // # stores

    Graph city(n);
    EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, city);
    ReverseEdgeMap reverse_edge_map = boost::get(boost::edge_reverse, city);
    EdgeAdder edge_adder(city, capacity_map, reverse_edge_map);

    Vertex src = boost::add_vertex(city);
    Vertex tgt = boost::add_vertex(city);
    edge_adder.addEdge(src, 0, s); // View this edge as the path from Rick's front door to his flat, through which he can go s times (or more, but there are only s stores to visit)

    std::vector<int> stores(s);
    for (int i = 0 ; i < s ; i++) {
        int store; std::cin >> store;
        edge_adder.addEdge(store, tgt, 1); // Each store only needs to be visited once
    }

    for (int i = 0 ; i < m ; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        edge_adder.addEdge(u, v, 1); // Only one pass allowed because Rick is a fucking freak who can't walk twice on the same street
        edge_adder.addEdge(v, u, 1); // wtf Rick, get ahold of yourself, you weirdo
    }

    std::cout << (boost::push_relabel_max_flow(city, src, tgt) == s ? "yes" : "no") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) tescase();
    return 0;
}
