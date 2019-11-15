// Includes
// ========
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor,
boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph &G;
    
public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

using namespace std;

void testcase() {
    int n; cin >> n; // # days
    
    graph G(n + 2);
    edge_adder adder(G);
    // auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    // auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const int src = n;
    const int sink = src + 1;
    
    for (int i = 0 ; i < n ; i++) {
        int ai; cin >> ai; // # menus that can be produced for day i
        int ci; cin >> ci; // # production cost per menu
        
        adder.add_edge(src, i, ai, ci);
    }
    
    int hungry_students = 0;
    for (int i = 0 ; i < n ; i++) {
        int si; cin >> si; // # students that want to eat on day i
        int pi; cin >> pi; // # price of a menu
        
        adder.add_edge(i, sink, si, -pi); // -pi because the price students pay reduce the cost by that much
        // Only works for cycle_cancelling -> not for all testsets
        hungry_students += si;
    }
    
    for (int i = 1 ; i < n ; i++) {
        int vi; cin >> vi; // # menus that can be frozen from day i - 1 to day i
        int ei; cin >> ei; // Freezing cost per menu
        
        adder.add_edge(i - 1, i, vi, ei);
    }
    
    int flow = boost::push_relabel_max_flow(G, src, sink);
    boost::cycle_canceling(G);
    int cost = boost::find_flow_cost(G);
    
    cout << (flow == hungry_students ? "possible" : "impossible") << " " << flow << " " << -cost << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
