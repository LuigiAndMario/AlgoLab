// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
    graph &G;
    
public:
    explicit edge_adder(graph &G) : G(G) {}
    
    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

using namespace std;

void testcase() {
    int n; cin >> n; // # vertices
    int m; cin >> m; // # edges
    int s; cin >> s; // # start locations
    int d; cin >> d; // # locations
    
    graph G(2 * n + 2);
    const int src = 2 * n;
    const int sink = 2 * n + 1;
    const int IN = 0;
    const int OUT = n;
    edge_adder adder(G);
    
    // i_in -> i_out
    for (int i = 0 ; i < n ; i++) {
        adder.add_edge(IN + i, OUT + i, 1);
    }
    
    // Actual edges
    for (int i = 0 ; i < m ; i++) {
        int from; cin >> from;
        int to; cin >> to;
        
        adder.add_edge(OUT + from, IN + to, 1);
    }
    
    // src -> starts
    for (int i = 0 ; i < s ; i++) {
        int v; cin >> v;
        adder.add_edge(src, IN + v, 1);
    }
    
    // dests -> sink
    for (int i = 0 ; i < d ; i++) {
        int v; cin >> v;
        adder.add_edge(OUT + v, sink, 1);
    }
    
    cout << boost::push_relabel_max_flow(G, src, sink) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
