#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

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
    int n; cin >> n; // # intersections (vertices)
    int m; cin >> m; // # streets (edges)
    int s; cin >> s; // # stores to visit
    
    graph G(n);
    edge_adder adder(G);
    
    const vertex_desc v_target = boost::add_vertex(G);
    // Given that we can reuse the same streets to go back home, we simply transfer 1 to the target for every visited store
    for (int i = 0 ; i < s ; i++) {
        int si; cin >> si;
        adder.add_edge(si, v_target, 1);
    }
    
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a;
        int b; cin >> b;
        adder.add_edge(a, b, 1); // Current street connects a to b,
        adder.add_edge(b, a, 1); // and it does so in both ways
    }
    
    const vertex_desc v_source = boost::add_vertex(G);
    adder.add_edge(v_source, 0, s); // As per the description, the source is Rick's home, which is at 0
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    
    cout << (flow == s ? "yes" : "no") << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
