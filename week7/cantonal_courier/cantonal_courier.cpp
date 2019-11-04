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
    int z; cin >> z; // # zones
    int j; cin >> j; // # jobs
    
    graph G(z + j);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);

    // IDEA: Use the flow to compute the cost of the best set of jobs we take
    // Then, we simply remove that from the total profit we'd have if every ticket was 0 (i.e. the sum of pis
    
    for (int i = 0 ; i < z ; i++) {
        int ci; cin >> ci;
        adder.add_edge(v_source, i, ci); // Vertex from the source to the zone i, for ci
    }
    
    int gains = 0; // Total gains from all the jobs
    for (int i = 0 ; i < j ; i++) {
        int pi; cin >> pi;
        adder.add_edge(z + i, v_target, pi); // Vertex from job i to target, for pi
        gains += pi;
    }
    
    for (int i = 0 ; i < j ; i++) {
        int ni; cin >> ni;
        
        for (int k = 0 ; k < ni ; k++) {
            int zk; cin >> zk;
            adder.add_edge(zk, z + i, INT_MAX); // Vertex from the zone zk to the job i, max capacity
        }
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    long profit = gains - flow;
    cout << profit << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
