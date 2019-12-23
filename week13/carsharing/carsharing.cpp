// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

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

using namespace std;

const long MAX_PROFIT = 100;
const long MAX_CARS = 100;

void testcase() {
    int N; cin >> N; // # bookings
    int S; cin >> S; // # stations
    
    vector<int> l(S);
    for (int i = 0 ; i < S ; i++) {
        cin >> l[i];
    }
    
    vector<int> s(N); // From
    vector<int> t(N); // To
    vector<int> d(N); // Departure time
    vector<int> a(N); // Arrival time
    vector<int> p(N); // Profit
    vector<set<int>> useful_times_for(S);
    int latest_arrival = -1;
    for (int i = 0 ; i < N ; i++) {
        cin >> s[i]; s[i]--;
        cin >> t[i]; t[i]--;
        cin >> d[i];
        cin >> a[i];
        cin >> p[i];
        
        useful_times_for[s[i]].insert(d[i]); // Need a node fot the departure time at the departure station
        useful_times_for[t[i]].insert(a[i]); // Need a node fot the arrival time at the arrival station
        
        latest_arrival = max(latest_arrival, a[i]);
    }
    
    // First 3 testsets assumptions
    // t <= 10'000, time % 30 = 0 -> total of 10'000 / 30 = 334 timesteps
    int timesteps = 334;
    int nodes = timesteps * S;
    graph G(nodes + 2);
    edge_adder adder(G);
    
    const int src = nodes;
    const int sink = nodes + 1;
    
    // Source to the first timeslot
    for (int i = 0 ; i < S ; i++) {
        adder.add_edge(src, i, l[i], 0);
    }
    
    // Previous to next timeslot
    for (int i = 0 ; i < S ; i++) {
        for (int j = 1 ; j < timesteps ; j++) {
            int offset_prev = ((j - 1) * S);
            int offset_cur = j * S;
            adder.add_edge(offset_prev + i, offset_cur + i, MAX_CARS, 0);
        }
    }
    
    // Actual bookings
    for (int i = 0 ; i < N ; i++) {
        int from = ((d[i] / 30) * S) + s[i];
        int to = ((a[i] / 30) * S) + t[i];
        adder.add_edge(from, to, 1, -p[i]);
    }
    
    // Last timeslot to sink
    for (int i = 0 ; i < S ; i++) {
        adder.add_edge(((timesteps - 1) * S) + i, sink, MAX_CARS, 0);
    }
    
    long flow = boost::push_relabel_max_flow(G, src, sink);
    boost::cycle_canceling(G);
    long cost = boost::find_flow_cost(G);
    cout << -cost << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
