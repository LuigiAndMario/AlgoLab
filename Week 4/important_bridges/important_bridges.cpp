#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::allow_parallel_edge_tag, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;

bool my_sort(std::pair<Vertex, Vertex> e_i, std::pair<Vertex, Vertex> e_j) {
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

    std::vector<Vertex> art_pts;
    articulation_points(archipelago, back_inserter(art_pts));

    std::vector<std::pair<Vertex, Vertex> > critical_bridges;
    std::set<std::pair<Vertex, Vertex> > critical_bridges_set;
    EdgeIterator ei;
    EdgeIterator end;

    for (std::vector<Vertex>::iterator vi = art_pts.begin() ; vi != art_pts.end() ; vi++) {
        for (tie(ei, end) = edges(archipelago) ; ei != end ; ei++) {
            Vertex i_1 = source(*ei, archipelago);
            Vertex i_2 = target(*ei, archipelago);

            if (*vi == i_1 || *vi == i_2) {
                critical_bridges_set.insert(i_1 < i_2 ? std::make_pair(i_1, i_2) : std::make_pair(i_2, i_1));
            }
        }
    }

    critical_bridges = std::vector<std::pair<Vertex, Vertex> >(critical_bridges_set.begin(), critical_bridges_set.end());
    std::sort(critical_bridges.begin(), critical_bridges.end(), my_sort);
    // critical_bridges.erase(unique(critical_bridges.begin(), critical_bridges.end()), critical_bridges.end());
    int k = critical_bridges_set.size();

    std::cout << k << std::endl;
    /*
    for (int i = 0 ; i < k ; i++) {
        std::cout << critical_bridges[i].first << " " << critical_bridges[i].second << std::endl;
    }
    */
    for (std::set<std::pair<Vertex, Vertex> >::iterator bridge = critical_bridges_set.begin() ; bridge != critical_bridges_set.end() ; bridge++) {
        std::cout << bridge->first << " " << bridge->second << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int c; std::cin >> c;
    while (c--) testcase();
    return 0;
}
