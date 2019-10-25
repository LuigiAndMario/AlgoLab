#include <iostream>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

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
    int n; cin >> n; // # players
    int m; cin >> m; // # rounds
    
    graph G(n);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    for (int i = 0 ; i < m ; i++) {
        int a; cin >> a; // Player a
        int b; cin >> b; // Player b
        int c; cin >> c; // Outcome (0 = unknown, i = player i won)
        
        if (c == 0) {
            adder.add_edge(v_source, a, 1); // We assume player a won,
            adder.add_edge(a, b, 1);        // but we can "transfer" that win to player b
        } else if (c == 1) {
            adder.add_edge(v_source, a, 1);
        } else {
            adder.add_edge(v_source, b, 1);
        }
    }
    
    const vertex_desc v_target = boost::add_vertex(G);
    int total = 0;
    for (int i = 0 ; i < n ; i++) {
        int si; cin >> si;
        adder.add_edge(i, v_target, si);
        total += si;
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    int goal = max(total, m); // Because if we have more rounds than the total value (which is technically possible), then the flow will be superior to the total value
    
    cout << (flow == goal ? "yes" : "no") << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
