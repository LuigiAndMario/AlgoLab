// Includes
// ========
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor,
boost::property <boost::edge_weight_t, long> > > > > undirected_graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<undirected_graph>::edge_descriptor  undirected_edge_desc;
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

double find(vector<int> &a, vector<int> &b, vector<int> &c, vector<int> &d, vector<int> &dist_map, int mid, int s, int f, int n, int m) {
    graph G(n + 1);
    const int src = n;
    edge_adder adder(G);
    for (int i = 0 ; i < m ; i++) {
        if (dist_map[a[i]] < dist_map[b[i]]) {
            adder.add_edge(a[i], b[i], c[i], d[i]);
        } else {
            adder.add_edge(b[i], a[i], c[i], d[i]);
        }
    }
    
    adder.add_edge(src, s, mid, 0);
    
    double flow = boost::push_relabel_max_flow(G, src, f);
    boost::successive_shortest_path_nonnegative_weights(G, src, f);
    double cost = boost::find_flow_cost(G);
    
    return cost / flow > dist_map[f] ? -1 : cost / flow;
}

void testcase() {
    int n; cin >> n; // # nodes
    int m; cin >> m; // # edges
    int s; cin >> s; // start
    int f; cin >> f; // end
    
    vector<int> a(m), b(m), c(m), d(m);
    for (int i = 0 ; i < m ; i++) {
        cin >> a[i] >> b[i] >> c[i] >> d[i];
    }
    
    undirected_graph Gu(n);
    auto weights = boost::get(boost::edge_weight, Gu);
    undirected_edge_desc ed;
    for (int i = 0 ; i < m ; i++) {
        ed = boost::add_edge(a[i], b[i], Gu).first;
        weights[ed] = d[i];
    }
    
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(Gu, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                                                                         dist_map.begin(), boost::get(boost::vertex_index, Gu))));
    
    graph Gmax(n);
    edge_adder adder(Gmax);
    for (int i = 0 ; i < m ; i++) {
        if (dist_map[a[i]] < dist_map[b[i]]) {
            adder.add_edge(a[i], b[i], c[i], d[i]);
        } else {
            adder.add_edge(b[i], a[i], c[i], d[i]);
        }
    }
    
    int bottom = 0;
    int top = boost::push_relabel_max_flow(Gmax, s, f);
    int best;
    while (true) {
        if (bottom == top) {
            best = bottom;
            break;
        }
        if (bottom == top - 1) {
            int with_bottom = find(a, b, c, d, dist_map, bottom, s, f, n, m);
            int with_top = find(a, b, c, d, dist_map, top, s, f, n, m);
            best = max(with_bottom, with_top) == with_top ? top : bottom;
            break;
        }
        
        int mid = (bottom + top) / 2;
        double res = find(a, b, c, d, dist_map, mid, s, f, n, m);
        if (res == -1) {
            top = mid - 1;
        } else {
            bottom = mid;
        }
    }
    
    cout << best << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
