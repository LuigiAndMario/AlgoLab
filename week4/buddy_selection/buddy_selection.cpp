// STL includes
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

using namespace std;

void testcase() {
    int n; cin >> n; // # nodes
    int c; cin >> c; // # characteristics
    int f; cin >> f; // Size of the matching
    
    map<string, int> as_int;
    int cur = 0;
    vector<set<int>> interests(n);
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < c ; j++) {
            string interest; cin >> interest;
            if (as_int.count(interest) == 0) {
                as_int[interest] = cur++;
            }
            interests[i].insert(as_int[interest]);
        }
    }
    
    graph G(n);
    for (int i = 0 ; i < n ; i++) {
        for (int j = i + 1 ; j < n ; j++) {
            int in_common = 0;
            for (int k : interests[i]) {
                if (interests[j].find(k) != interests[j].end()) {
                    in_common++;
                }
            }
            if (in_common > f) {
                boost::add_edge(i, j, G);
            }
        }
    }
    
    std::vector<vertex_desc> mate_map(n);  // exterior property map
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
    
    boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
    for (int i = 0; i < n; ++i) {
        // mate_map[i] != NULL_VERTEX: the vertex is matched
        // i < mate_map[i]: visit each edge in the matching only once
        if (mate_map[i] == NULL_VERTEX) {
            cout << "optimal" << endl;
            return;
        }
    }
    cout << "not optimal" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
