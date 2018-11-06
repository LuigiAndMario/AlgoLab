#include <iostream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define OFFSET 65
#define ALPHABET_SIZE 26

typedef    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<
boost::edge_capacity_t, int, boost::property<boost::edge_residual_capacity_t, int, boost::property<
boost::edge_reverse_t, Traits::edge_descriptor> > > > Graph;
// Interior Property Maps
typedef    boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef    boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef    boost::property_map<Graph, boost::edge_reverse_t>::type    ReverseEdgeMap;
typedef    boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef    boost::graph_traits<Graph>::edge_descriptor Edge;
typedef    boost::graph_traits<Graph>::edge_iterator EdgeIt;

// Copied from the slides
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap    &capacitymap;
    ReverseEdgeMap    &revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
    G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void testcase() {
    int h; std::cin >> h; // # lines
    int w; std::cin >> w; // # columns (aka. words per line)

    // Mapping every character of the message to the number of time they occur
    std::string message; std::cin >> message;
    std::vector<int> letter_occurrence(ALPHABET_SIZE, 0);
    int message_size = 0;
    for (char &c: message) {
        letter_occurrence[c - OFFSET]++;
        message_size++;
    }

    // Creating the graph
    Graph possibilities(ALPHABET_SIZE);
    Vertex src = boost::add_vertex(possibilities);
    Vertex tgt = boost::add_vertex(possibilities);

    EdgeCapacityMap capacity_map = get(boost::edge_capacity, possibilities);
    ReverseEdgeMap rev_edge_map = get(boost::edge_reverse, possibilities);
    EdgeAdder edge_adder(possibilities, capacity_map, rev_edge_map);

    // Each letter in the message has an edge to the target with weight the number of its occurrences
    for (int i = 0 ; i < ALPHABET_SIZE ; i++) {
        if (letter_occurrence[i]) {
            edge_adder.addEdge(i, tgt, letter_occurrence[i]);
        }
    }

    // Parsing the newspaper
    std::vector<std::vector<int> > front_page(h, std::vector<int>(w));
    for (int i = 0 ; i < h ; i++) {
        std::string line; std::cin >> line;
        int j = 0;
        for (char &c: line) {
            front_page[i][j++] = c - OFFSET;
        }
    }
    
    std::vector<std::vector<int> > back_page(h, std::vector<int>(w)); // adjusted
    for (int i = 0 ; i < h ; i++) {
        std::string line; std::cin >> line;
        int j = w - 1;
        for (char &c: line) {
            back_page[i][j--] = c - OFFSET;
        }
    }
    // We now have that front_page[i][j] is what's on the opposite side of back_page[i][j]

    std::vector<std::vector<int> > bits(ALPHABET_SIZE, std::vector<int>(ALPHABET_SIZE, 0));
    for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < w ; j++) {
            int front = front_page[i][j];
            int back = back_page[i][j];
            bits[front][back]++;
        }
    }

    for (int i = 0 ; i < ALPHABET_SIZE ; i++) {
        for (int j = 0 ; j < ALPHABET_SIZE ; j++) {
            if (bits[i][j]) {
                Vertex current = boost::add_vertex(possibilities);
                edge_adder.addEdge(src, current, bits[i][j]);

                if (letter_occurrence[i] != 0) {
                    edge_adder.addEdge(current, i, bits[i][j]);
                }

                if (letter_occurrence[j] != 0) {
                    edge_adder.addEdge(current, j, bits[i][j]);
                }
            }
        }
    }

    std::cout << (boost::push_relabel_max_flow(possibilities, src, tgt) >= message_size ? "Yes" : "No") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

