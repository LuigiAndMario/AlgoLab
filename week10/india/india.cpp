// Includes
// ========
// STL includes
#include <iostream>
#include <climits>
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

typedef struct guide {
    int x;
    int y;
    int e;
    int d;
} guide;

using namespace std;

long flow_with_capacity(vector<guide> &guides, long capacity, int c, int b, int k, int a) {
    graph G(c + 1);
    edge_adder adder(G);
    const int src = c;
    for (auto it = guides.begin() ; it != guides.end() ; it++) {
        adder.add_edge(it->x, it->y, it->e, it->d);
    }
    adder.add_edge(src, k, capacity, 0);
    
    boost::successive_shortest_path_nonnegative_weights(G, src, a);
    long cost = boost::find_flow_cost(G);
    
    return cost <= b ? boost::push_relabel_max_flow(G, src, a) : -1;
}

void testcase() {
    int c; cin >> c; // # cities (nodes)
    int g; cin >> g; // # guides (edges)
    int b; cin >> b; // Budget
    int k; cin >> k; // Start
    int a; cin >> a; // End
    
    graph G(c);
    edge_adder adder(G);
    
    vector<guide> guides;
    for (int i = 0 ; i < g ; i++) {
        int x; cin >> x; // From
        int y; cin >> y; // To
        int d; cin >> d; // Cost
        int e; cin >> e; // Capacity
        
        adder.add_edge(x, y, e, d);
        guides.push_back({x, y, e, d});
    }
    
    // IDEA: limit the flow, get the max possible flow with cost <= b
    // Find this with binary search (bot = 0, top = max flow)
    long bot = 0; // Lower bound
    long top = boost::push_relabel_max_flow(G, k, a); // Upper bound
    
    // This clears testset 2
    boost::successive_shortest_path_nonnegative_weights(G, k, a);
    long cost = boost::find_flow_cost(G);
    if (cost <= b) {
        cout << top << endl;
        return;
    }
    
    long best = -1;
    while (true) {
        // End of the searc
        if (bot == top) {
            // Litterally a tie
            best = flow_with_capacity(guides, bot, c, b, k, a);
            break;
        }
        if (bot + 1 == top) {
            // Kind of a tie
            best = max(flow_with_capacity(guides, bot, c, b, k, a),
                       flow_with_capacity(guides, top, c, b, k, a));
            break;
        }
        
        // The search continues
        long mid = (bot + top) / 2;
        long res = flow_with_capacity(guides, mid, c, b, k, a);
        
        if (res == -1) {
            // Mid is too high, we need to go lower
            top = mid - 1;
        } else {
            // Mid might be too low, we can go higher
            bot = mid;
        }
    }
    
    cout << best << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
