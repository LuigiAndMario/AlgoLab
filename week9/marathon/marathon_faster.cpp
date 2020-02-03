// Includes
// ========
#include <iostream>
#include <vector>
#include <climits>
#include <map>
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


typedef boost::property_map<graph, boost::edge_weight_t>::type  weight_map;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;

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
    
    // Building the *graph* of the city
    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    map<edge_desc, int> capacity;
    
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a; // From
        int b; cin >> b; // To
        int c; cin >> c; // Width (how many runners can run here) -> capacity
        int d; cin >> d; // Length -> cost
        
        if (a != b) {
            edge_desc e;
            e = boost::add_edge(a, b, G).first;
            capacity[e] = c;
            weights[e] = d;
        }
    }
    
    // Assumption for testset 2: The shortest path is unique
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);
    boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))) /* dot! */ .predecessor_map(boost::make_iterator_property_map( pred_map.begin(), boost::get(boost::vertex_index, G))));
    
    // Building the *flow network* of the city
    graph G_flow(n);
    edge_adder adder(G_flow);
    
    // Adding all the edges in the shortest path to the flow network
    for (int i = 0 ; i < pred_map.size() ; i++) {
        int prev = pred_map[i];
        if (i != prev) {
            edge_desc e = boost::edge(prev, i, G).first;
            adder.add_edge(prev, i, capacity[e], weights[e]);
            adder.add_edge(i, prev, capacity[e], weights[e]); // Because every street is 2-way
        }
    }
    
    int flow = boost::push_relabel_max_flow(G_flow, s, f);
    boost::successive_shortest_path_nonnegative_weights(G_flow, s, f);
    int cost = boost::find_flow_cost(G_flow);
    
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
