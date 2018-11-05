#include <iostream>

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

void testcase() {
    int l; std::cin >> l; // # locations
    int p; std::cin >> p; // # paths

    Graph kingdom(l);
    Vertex src = boost::add_vertex(kingdom);
    Vertex tgt = boost::add_vertex(kingdom);

    EdgeCapacityMap capacity_map = get(boost::edge_capacity, kingdom);
    ReverseEdgeMap rev_edge_map = get(boost::edge_reverse, kingdom);
    EdgeAdder edge_adder(kingdom, capacity_map, rev_edge_map);

    int soldiers_needed = 0;
    for (int i = 0 ; i < l ; i++) {
        int g_i; std::cin >> g_i; // What we have
        int d_i; std::cin >> d_i; // What we need

        edge_adder.addEdge(src, i, g_i); // g_i soldiers "go there" at first (src -(g_i)-> i)
        edge_adder.addEdge(i, tgt, d_i); // d_i soldiers must be there in the end (i -(d_i)-> tgt)

        soldiers_needed += d_i;
    }

    int minimum_path_traversals = 0;
    for (int i = 0 ; i < p ; i++) {
        int f_i; std::cin >> f_i; // from
        int t_i; std::cin >> t_i; // to
        int c_i; std::cin >> c_i; // min # traversals
        int C_i; std::cin >> C_i; // max # traversals

        edge_adder.addEdge(src, t_i, c_i); // We must have at least c_i traversals on this path (src -(c_i)-> t_i),
        edge_adder.addEdge(f_i, tgt, c_i); // but also (f_i -(c_i)-> tgt) - that way, we ensure that at least c_i
        // traversals happen (when computing the max_flow)

        edge_adder.addEdge(f_i, t_i, C_i - c_i); // All the extra traversals we are can have on this path

        minimum_path_traversals += c_i;
    }

    std::cout << (boost::push_relabel_max_flow(kingdom, src, tgt) >= (soldiers_needed + minimum_path_traversals) ? "yes" : "no") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
