// STL includes
#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator                       edge_it;

void testcase() {
	int n; cin >> n;
	int m; cin >> m;
	int k; cin >> k;
	int T; cin >> T;

	vector<int> t(T);
	for (int i = 0 ; i < T ; i++) {
		cin >> t[i];
	}

	weighted_graph G(n);
	weight_map weights = boost::get(boost::edge_weight, G);
	edge_desc ed;
	for (int i = 0 ; i < m ; i++) {
		int u; cin >> u;
		int v; cin >> v;
		int c; cin >> c;

		ed = boost::add_edge(v, u, G).first;
		weights[ed] = c;
	}

	// every two planets in the teleportation network are, let's say, "teleportable" iff they are already reachable without the network (in both ways)

	// scc_map[i]: index of SCC containing i-th vertex
	std::vector<int> scc_map(n);  // exterior property map
	// nscc: total number of SCCs
	int nscc = boost::strong_components(G, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

	// For the shortest paths, it's gonna be a mess to add too many edges
	// So instead we add a new vertex (think mass relay) and link every "teleportable" planets to this edge, with cost 0
	vector<vector<int> > teleportation_networks(nscc);
	for (int i = 0 ; i < T ; i++) {
		teleportation_networks[scc_map[t[i]]].push_back(t[i]);
	}

	for (int i = 0 ; i < nscc ; i++) {
		int cost = teleportation_networks[i].size() - 1;
		vertex_desc mass_relay = boost::add_vertex(G);

		for (auto it = teleportation_networks[i].begin() ; it != teleportation_networks[i].end() ; it++) {
			ed = boost::add_edge(*it, mass_relay, G).first;
			weights[ed] = 0;

			ed = boost::add_edge(mass_relay, *it, G).first;
			weights[ed] = cost;
		}
	}

	// We also need to keep track of the new number of vertices
	int n_with_teleportation = n + nscc;

	std::vector<int> dist_map(n_with_teleportation);
  	std::vector<vertex_desc> pred_map(n_with_teleportation);

	// Remember that we want to deliver to planet n - 1
  	boost::dijkstra_shortest_paths(G, n - 1,
    		boost::distance_map(boost::make_iterator_property_map(
      			dist_map.begin(), boost::get(boost::vertex_index, G)))
    		.predecessor_map(boost::make_iterator_property_map(
      			pred_map.begin(), boost::get(boost::vertex_index, G))));

	int best_time = INT_MAX;
	for (int i = 0 ; i < k ; i++) {
		best_time = min(best_time, dist_map[i]);
	}

	// We need to deliver the parcel in 10^6 microseconds or less 
	if (best_time > 1000000) {
		cout << "no" << endl;
	} else {
		cout << best_time << endl;
	}
}

int main() {
	ios::sync_with_stdio(false);
	int t; cin >> t;
	while (t--) {
		testcase();
	}
	return 0;
}
