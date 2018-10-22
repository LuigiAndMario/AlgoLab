#include <iostream>
#include <set>
#include <map>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::graph_traits<Graph>::edge_descriptor	Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;



void testcase() {
    int n; std::cin >> n; // Number of students. For simplicity, n % 2 == 0
    int c; std::cin >> c; // Characteristics per student
    int f; std::cin >> f; // Minimum common characteristics

    std::vector<std::set<int> > students(n);
    // We store the hobbies as ints, for a faster comparison
    std::map<std::string, int> hobbies_as_ints;
    int counter = 0;
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < c ; j++) {
            std::string hobby; std::cin >> hobby;
            if (hobbies_as_ints.find(hobby) == hobbies_as_ints.end()) {
                hobbies_as_ints[hobby] = counter++;
            }
            students[i].insert(hobbies_as_ints[hobby]);
        }
    }

    std::map<std::pair<int, int>, int> edges;
    for (int i = 0 ; i < n - 1 ; i++) {
        for (int j = i + 1 ; j < n ; j++) {
            std::vector<int> common_hobbies(c);
            auto it = set_intersection(students[i].begin(), students[i].end(), students[j].begin(), students[j].end(), common_hobbies.begin());
            std::pair<int, int> buddies_pair = std::make_pair(i, j);
            common_hobbies.resize(it - common_hobbies.begin());
            edges[buddies_pair] = common_hobbies.size();
        }
    }

    Graph acceptable_pairings(n); // We only have in this graphs the pairings where the number of common hobbies is > f
    WeightMap weight_map = boost::get(boost::edge_weight, acceptable_pairings);
    for (std::map<std::pair<int, int>, int>::iterator it = edges.begin() ; it != edges.end() ; it++) {
        if (it->second > f) {
            Edge e;
            bool success;
            tie(e, success) = add_edge(it->first.first, it->first.second, acceptable_pairings);
            weight_map[e] = 1;
        }
    }

    std::vector<Vertex> mate_map(n);
    boost::edmonds_maximum_cardinality_matching(acceptable_pairings, boost::make_iterator_property_map(mate_map.begin(), get(boost::vertex_index, acceptable_pairings)));
    int matchingsize = matching_size(acceptable_pairings, boost::make_iterator_property_map(mate_map.begin(), get(boost::vertex_index, acceptable_pairings)));

    std::cout << (n == (2 * matchingsize) ? "not optimal" : "optimal") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
