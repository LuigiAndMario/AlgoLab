#include <iostream>
#include <vector>
#include <string>

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
    int h; cin >> h; // height
    int w; cin >> w; // width
    
    const int OFFSET = (int)'A';
    const int ALPHABET_SIZE = (int)'Z' - OFFSET + 1; // Yes, I could have just written 26, I know...
    
    string note_str; cin >> note_str;
    int note_length = 0; // I don't know how to get  a string's length...
    vector<int> note(ALPHABET_SIZE, 0); // # occurences of each letter in the note
    for (char &c: note_str) {
        note[(int)c - OFFSET]++;
        note_length++;
    }
    
    // Parsing the daily telegraph
    vector<vector<int> > front(h, vector<int>(w));
    for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < w ; j++) {
            char c; cin >> c;
            front[i][j] = ((int)c - OFFSET);
        }
    }
    vector<vector<int> > back(h, vector<int>(w));
    for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < w ; j++) {
            char c; cin >> c;
            // front[i][j] corresponds to back[i][w - 1 - j]
            back[i][w - 1 - j] = ((int)c - OFFSET);
        }
    }
    
    // Turning it into cutouts
    vector<vector<int> > cutouts(ALPHABET_SIZE, vector<int>(ALPHABET_SIZE, 0)); // cutouts[i][j] = # times i is in the front and j the back
    for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < w ; j++) {
            int side1 = front[i][j];
            int side2 = back[i][j];
            
            cutouts[side1][side2]++;
        }
    }
    
    // Creating the graph
    graph G(ALPHABET_SIZE);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);
    
    // Adding all the edges of the cutouts
    for (int i = 0 ; i < ALPHABET_SIZE ; i++) {
        for (int j = 0 ; j < ALPHABET_SIZE ; j++) {
            if (cutouts[i][j] != 0) {
                vertex_desc cutout = boost::add_vertex(G);
                
                adder.add_edge(v_source, cutout, cutouts[i][j]);
                adder.add_edge(cutout, i, cutouts[i][j]);
                adder.add_edge(cutout, j, cutouts[i][j]);
            }
        }
    }
    
    // Adding all edges of the note
    for (int i = 0 ; i < ALPHABET_SIZE ; i++) {
        if (note[i] != 0) { // We don't add any 0-capacity edges
            adder.add_edge(i, v_target, note[i]);
        }
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    cout << (flow == note_length ? "Yes" : "No") << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
