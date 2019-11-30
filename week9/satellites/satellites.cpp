// Includes
// ========
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
    int g; cin >> g; // # ground stations
    int s; cin >> s; // # satellites
    int l; cin >> l; // # links
    
    graph G(g + s + 2);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    int src = g + s;
    int sink = src + 1;
    
    for (int i = 0 ; i < g ; i++) {
        adder.add_edge(src, i, 1);
    }
    
    for (int i = g ; i < g + s ; i++) {
        adder.add_edge(i, sink, 1);
    }
    
    for (int i = 0 ; i < l ; i++) {
        int ground; cin >> ground;
        int sat; cin >> sat;
        
        adder.add_edge(ground, g + sat, 1);
    }
    
    // Find a min cut via maxflow
    int flow = boost::push_relabel_max_flow(G, src, sink);
    
    // BFS to find vertex set S
    std::vector<int> vis(g + s + 2, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[src] = true; // Mark the source as visited
    Q.push(src);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        out_edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rc_map[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
        }
    }
    
    vector<int> ground;
    for (int i = 0 ; i < g ; i++) {
        if (!vis[i]) {
            ground.push_back(i);
        }
    }
    
    vector<int> satellites;
    for (int i = 0 ; i < s ; i++) {
        if (vis[g + i]) {
            satellites.push_back(i);
        }
    }
    
    cout << ground.size() << " " << satellites.size() << endl;
    for (auto it = ground.begin() ; it != ground.end() ; it++) {
        cout << *it << " ";
    }
    for (auto it = satellites.begin() ; it != satellites.end() ; it++) {
        cout << *it << " ";
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