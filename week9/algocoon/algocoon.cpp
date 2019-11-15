// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >    graph;
// Interior Property Maps
typedef    boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef    boost::graph_traits<graph>::out_edge_iterator            out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
    graph &G;
    
public:
    explicit edge_adder(graph &G) : G(G) {}
    
    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

using namespace std;

void testcase() {
    int n; cin >> n; // # figures
    int m; cin >> m; // # limbs
    
    // Create graph, edge adder class and propery maps
    graph G(n);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a;
        int b; cin >> b;
        int c; cin >> c;
        
        adder.add_edge(a, b, c);
    }
    
    // Iterating over all the possible statues that we can take
    // There are necessarily two statues with indices i, i+1 that don't have the same status (taken, not taken)
    int src = n - 1;
    int sink = 0;
    int min_flow = boost::push_relabel_max_flow(G, src, sink);
    int src_best = src;
    int sink_best = sink;
    for (int i = 1 ; i < n ; i++) {
        src = i - 1;
        sink = i;
        int flow = boost::push_relabel_max_flow(G, src, sink);
        if (flow < min_flow) {
            min_flow = flow;
            src_best = src;
            sink_best = sink;
        }
    }
    
    // We need to recompute the best flow here (oherwise, the flow with src = n - 1, sink = n - 1 is taken into account)
    int flow = boost::push_relabel_max_flow(G, src_best, sink_best);
    cout << flow << endl;
    
    // BFS to find vertex set S
    std::vector<int> vis(n, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[src_best] = true; // Mark the source as visited
    int figures = 1;
    Q.push(src_best);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        out_edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rc_map[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            figures++;
            Q.push(v);
        }
    }
    
    // Output S
    cout << figures << " ";
    for (int i = 0; i < n; ++i) {
        if (vis[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
