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
    int w; cin >> w; // Width
    int n; cin >> n; // # bricks
    
    graph G(2 * w); // Vertices at 0, 1, 2, ..., w - 1, w - but we split every vertex into 2 (to impose a maximum flow per vertex)
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);
    
    int offset = w;
    for (int i = 1 ; i < w ; i++) {
        adder.add_edge(i, offset + i, 1);
    }
    
    for (int i = 0 ; i < n ; i++) {
        int start; cin >> start;
        int end; cin >> end;
        
        // Making sure the start is before the end
        if (start > end) {
            int tmp = start;
            start = end;
            end = tmp;
        }
        
        if (start == 0) {
            // Vertex 0's twin is unused, so we can't just add the offset here
            adder.add_edge(v_source, start, 1);
            adder.add_edge(start, end, 1);
        } else {
            adder.add_edge(offset + start, end, 1);
        }
        
        if (end == w) {
            adder.add_edge(end, v_target, 1);
        }
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    cout << flow << endl;
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
