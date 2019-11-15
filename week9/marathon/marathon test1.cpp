// Includes
// ========
#include <iostream>
#include <vector>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
    int n; cin >> n; // # intersections
    int m; cin >> m; // # streets
    int s; cin >> s; // Start index -> source
    int f; cin >> f; // Finishing index -> sink
    
    graph G(n);
    edge_adder adder(G);
    // auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    // auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a; // From
        int b; cin >> b; // To
        int c; cin >> c; // Width (how many runners can run here) -> capacity
        int d; cin >> d; // Length -> cost
        
        if (a != b) {
            adder.add_edge(a, b, c, d);
        }
    }
    
    // Assumption for testset 1: every street is on a shortest path (i.e. every street can be taken)
    int flow = boost::push_relabel_max_flow(G, s, f);
    boost::successive_shortest_path_nonnegative_weights(G, s, f);
    int length = boost::find_flow_cost(G); // length of the race
    
    cout << flow << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
