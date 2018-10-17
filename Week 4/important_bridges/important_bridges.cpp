#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

// Copied-pasted from the doc - why the hell do they use magic number like that though ?
struct edge_component_t {
    enum {
        num = 555
    };
    typedef boost::edge_property_tag kind;
} edge_component;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<edge_component_t, int> > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
typedef boost::property_map<Graph, edge_component_t>::type ComponentMap;

bool weird_sort(std::pair<Vertex, Vertex> e_i, std::pair<Vertex, Vertex> e_j) {
    return e_i.first == e_j.first ? e_i.second < e_j.second : e_i.first < e_j.first;
}

std::pair<int, int> adjust(std::pair<int, int> p) {
    return p.first > p.second ? std::make_pair(p.second, p.first) : p;
}

void testcase() {
    int n; std::cin >> n; // Islands (vertices)
    int m; std::cin >> m; // Bridges (edges)

    Graph archipelago(n);
    for (int i = 0 ; i < m ; i++) {
        int i_1; std::cin >> i_1;
        int i_2; std::cin >> i_2;

        Edge ed;
        bool success;
        tie(ed, success) = boost::add_edge(i_1, i_2, archipelago);
    }

    ComponentMap edge_components = get(edge_component, archipelago);
    int bcc = biconnected_components(archipelago, edge_components);
    std::vector<std::vector<std::pair<int, int> > > bridges_per_component(m);

    EdgeIterator i;
    EdgeIterator end;
    for (tie(i, end) = boost::edges(archipelago) ; i != end ; i++) {
        bridges_per_component[edge_components[*i]].push_back(std::make_pair(source(*i, archipelago), target(*i, archipelago)));
    }

    std::vector<std::pair<int, int> > important_bridges;
    for (int i = 0 ; i < bcc ; i++) {
        if (bridges_per_component[i].size() == 1) {
            // important bridge
            important_bridges.push_back(adjust(bridges_per_component[i][0]));
        }
    }

    std::sort(important_bridges.begin(), important_bridges.end(), weird_sort);

    std::cout << important_bridges.size() << std::endl;
    for (int i = 0 ; i < important_bridges.size() ; i++) {
        std::cout << important_bridges[i].first << " " << important_bridges[i].second << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int c; std::cin >> c;
    while (c--) testcase();
    return 0;
}
