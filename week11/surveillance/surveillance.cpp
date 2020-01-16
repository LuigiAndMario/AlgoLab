#include <iostream>
#include <vector>
#include <climits>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

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

void testcase() {
    int n; cin >> n; // # intersections
    int m; cin >> m; // # streets
    int k; cin >> k; // # stations
    int l; cin >> l; // # photographs
    
    vector<int> stations(k);
    for (int i = 0 ; i < k ; i++) {
        cin >> stations[i];
    }
    
    vector<int> photos(l);
    for (int i = 0 ; i < l ; i++) {
        cin >> photos[i];
    }
    
    graph g((2 * n) + 2);
    const int src = 2 * n;
    const int sink = src + 1;
    edge_adder adder(g);
    
    for (int i = 0 ; i < m ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        
        adder.add_edge(x, y, INT_MAX); // Any policeman without carrying a photo
        adder.add_edge(n + x, n + y, 1); // At most one policeman carrying a photo
    }
    
    for (int i = 0 ; i < l ; i++) {
        adder.add_edge(photos[i], n + photos[i], 1);
    }
    
    for (int i = 0 ; i < k ; i++) {
        adder.add_edge(src, stations[i], 1);
        adder.add_edge(n + stations[i], sink, 1);
    }
    
    cout << boost::push_relabel_max_flow(g, src, sink) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
