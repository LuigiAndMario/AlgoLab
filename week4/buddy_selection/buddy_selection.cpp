// STL includes
#include <iostream>
#include <vector>
#include <climits>
#include <string>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

typedef vector<int> vec;
typedef vector<vec> vec2;
typedef vector<string> vecs;

vecs index_of;
vec2 interests;

int find(string characteristic) {
    for (int i = 0 ; i < index_of.size() ; i++) {
        if (index_of[i] == characteristic) {
            return i;
        }
    }
    return -1;
}

void testcase() {
    int n; cin >> n; // students
    int c; cin >> c; // characteristics
    int f; cin >> f; // goal
    
    index_of = vecs(0);
    interests = vec2(n, vec(c, -1));
    
    // O(ncl), l the maximal length of the strings (so 20, so O(nc))
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < c ; j++) {
            string characteristic; cin >> characteristic;
            int index = find(characteristic);
            if (index == -1) {
                index_of.push_back(characteristic);
                index = index_of.size() - 1;
            }
            
            interests[i][j] = index;
        }
    }
    
    graph g(n);
    edge_desc ed;
    // O(n^2c^2) - for now, I get 100 points with this absolutely dreadful n^2c^2 horror of a thing. Is it normal ?
    // The idea here is to add an edge between every two student only if they share > f interests
    // In other words, each edge represents a possible pairing
    for (int i = 0 ; i < n ; i++) { // Student i
        for (int j = 0 ; j < n ; j++) { // Student j
            if (j != i) {
                int in_common = 0;
                for (int k = 0 ; k < c ; k++) { // Student i's interests
                    for (int l = 0 ; l < c ; l++) { // Student j's interests
                        if (interests[i][k] == interests[j][l]) {
                            in_common++;
                        }
                    }
                }
                
                if (in_common > f) {
                    ed = boost::add_edge(i, j, g).first;
                }
            }
        }
    }
    
    // Now that we have a graph where each edge represents a possible pairing, we check if we can have a perfect maximum matching
    // In other words, we check that it is possible to match every student with someone else, and not leaving anyone behind
    // This would not be the case if we had, for example, an isolated student, or something like o--o--o
    vector<vertex_desc> mate_map(n); // exterior property map
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
    boost::edmonds_maximum_cardinality_matching(g, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));
    // int matching_size = boost::matching_size(g, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));
    
    // A maximum matching is perfect if there is no isolated vertex
    for (auto it = mate_map.begin() ; it != mate_map.end() ; it++) {
        if (*it == NULL_VERTEX) {
            cout << "optimal" << endl;
            return;
        }
    }
    
    cout << "not optimal" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
