// Includes
// ========
#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <set>
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

typedef struct booking {
    int s, t, d, a, p;
} booking;


using namespace std;

void testcase() {
    int n; cin >> n;
    int s; cin >> s;
    
    const int MAX_TIME = 100000;
    const int MAX_PROFIT = 100;
    
    vector<int> l(s);
    for (int i = 0 ; i < s ; i++) {
        cin >> l[i];
    }
    
    
    vector<set<int>> times(s);
    for (int i = 0 ; i < s ; i++) {
        times[i].insert(0);
        times[i].insert(MAX_TIME);
    }
    
    vector<booking> bookings(n);
    for (int i = 0 ; i < n ; i++) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--;
        t--;
        times[s].insert(d);
        times[t].insert(a);
        
        bookings[i].s = s;
        bookings[i].t = t;
        bookings[i].d = d;
        bookings[i].a = a;
        bookings[i].p = p;
    }
    
    // Mapping every station to its relevant times
    vector<map<int, int>> times_at(s);
    vector<int> offset_to(s + 1, 0);
    for (int i = 0 ; i < s ; i++) {
        int last_added = 0;
        for (int t: times[i]) {
            times_at[i][t] = last_added++;
        }
        
        offset_to[i + 1] = offset_to[i] + last_added;
    }
    
    int num_vertices = offset_to[s];
    
    graph G(num_vertices + 2);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const int src = num_vertices;
    const int sink = num_vertices + 1;
    
    for (int i = 0 ; i < s ; i++) {
        adder.add_edge(src, offset_to[i], l[i], 0);
        adder.add_edge(offset_to[i + 1] - 1, sink, INT_MAX, 0);
        
        int cur = 0;
        int prev_time;
        for (auto t: times[i]) {
            if (cur != 0) {
                adder.add_edge(offset_to[i] + cur - 1, offset_to[i] + cur, s * 100, MAX_PROFIT * (t - prev_time));
            }
            cur++;
            prev_time = t;
        }
    }
    
    for (int i = 0 ; i < n ; i++) {
        // Compiler is fucking stupid and won't accept anything without these extra variables
        int from = offset_to[bookings[i].s] + times_at[bookings[i].s][bookings[i].d];
        int to = offset_to[bookings[i].t] + times_at[bookings[i].t][bookings[i].a];
        
        adder.add_edge(from, to, 1, MAX_PROFIT * (bookings[i].a - bookings[i].d) - bookings[i].p);
    }
    
    // int flow = boost::push_relabel_max_flow(G, src, sink);
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];
    }
    
    cout << MAX_PROFIT * MAX_TIME * flow - cost << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
