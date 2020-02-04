// Includes
// ========
#include <iostream>
#include <vector>
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
    int n; cin >> n;
    int m; cin >> m;
    
    const int MAX_WEIGHT = 128;
    
    vector<int> c(n - 1);
    for (int i = 0 ; i < n - 1 ; i++) {
        cin >> c[i];
    }
    
    graph G(n + 2);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const int src = n;
    const int sink = n + 1;
    edge_adder adder(G);
    
    int c_prev = 0;
    for (int i = 0 ; i < n ; i++) {
        int ci = 0;
        if (i < n - 1) {
            ci = c[i];
            adder.add_edge(i, i + 1, ci, MAX_WEIGHT);
            
            if (ci > c_prev) {
                // Outgoing flow is larger than incoming - we need to pull from src to compensate
                adder.add_edge(src, i, ci - c_prev, MAX_WEIGHT * i);
            }
        }
        
        if (i > 0 && ci < c_prev) {
            // There is an incoming flow, and it is larger than the outgoing flow - we send to sink to compensate
            adder.add_edge(i, sink, c_prev - ci, MAX_WEIGHT * (n - 1 - i));
        }
        

        c_prev = ci;
    }
    
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a;
        int b; cin >> b;
        int d; cin >> d;
        
        int cost = (MAX_WEIGHT * (b - a)) - d;
        
        // Getting rid of parallel edges
        out_edge_it e, eend;
        bool can_add = true;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(a, G), G); e != eend; ++e) {
            if (boost::target(*e, G) == b && w_map[*e] == cost) {
                c_map[*e]++;
                can_add = false;
                break;
            }
        }
        
        if (can_add) {
            adder.add_edge(a, b, 1, cost);
        }
    }
    
    int flow = boost::push_relabel_max_flow(G, src, sink);
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);
    
    cout << ((n - 1) * MAX_WEIGHT * flow) - cost << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
