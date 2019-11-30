// Includes
// ========
// STL includes
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
    int c; cin >> c; // # cities (nodes)
    int g; cin >> g; // # guides (edges)
    int b; cin >> b; // Budget
    int k; cin >> k; // Start
    int a; cin >> a; // End
    
    graph G(c);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    for (int i = 0 ; i < g ; i++) {
        int x; cin >> x; // From
        int y; cin >> y; // To
        int d; cin >> d; // Cost
        int e; cin >> e; // Capacity
        
        adder.add_edge(x, y, e, d);
    }
    
    // Second testsets assumption - we can pay for everything
    
    long flow = boost::push_relabel_max_flow(G, k, a);
    boost::successive_shortest_path_nonnegative_weights(G, k, a);
    int cost = boost::find_flow_cost(G);
    
    cout << (cost <= b ? flow : 0) << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
