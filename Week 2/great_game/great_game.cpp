#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

std::vector<std::vector<int> > even_odd_moves; // [even turn moves, odd turn moves]

int move(int moves_so_far, int meeple_position, int goal, std::vector<std::vector<int> > & graph) {
    if (meeple_position == goal) {
        return moves_so_far;
    }

    if (even_odd_moves[moves_so_far % 2][meeple_position] != -1) {
        return even_odd_moves[moves_so_far % 2][meeple_position] + moves_so_far;
    }

    bool even_turn = moves_so_far % 2 != 0;
    int result;
    if (even_turn) {
        // Do the worst
        int max_moves = INT_MIN;
        for (auto i = graph[meeple_position].begin() ; i < graph[meeple_position].end() ; i++) {
            max_moves = std::max(max_moves, move(moves_so_far + 1, *i, goal, graph));
        }
        result = max_moves;
    } else {
        // Do the best
        int min_moves = INT_MAX;
        for (auto i = graph[meeple_position].begin() ; i < graph[meeple_position].end() ; i++) {
            min_moves = std::min(min_moves, move(moves_so_far + 1, *i, goal, graph));
        }
        result = min_moves;
    }

    even_odd_moves[moves_so_far % 2][meeple_position] = result - moves_so_far;
    return result;
}

void testcase() {
    int n; std::cin >> n; // Number of positions
    int m; std::cin >> m; // Number of transitions
    int r; std::cin >> r; // Starting position of the red meeple
    r--;
    int b; std::cin >> b; // Starting position of the balck meeple
    b--;
    std::vector<std::vector<int> > graph(n);
    for (int i = 0 ; i < m ; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        graph[--u].push_back(--v);
    }

    int goal = n - 1;
    even_odd_moves = std::vector<std::vector<int> > (2, std::vector<int>(goal, -1));

    int moves_r = move(0, r, goal, graph);
    int moves_b = move(0, b, goal, graph);

    if (moves_r < moves_b) {
        std::cout << 0 << std::endl;
    } else if (moves_r > moves_b) {
        std::cout << 1 << std::endl;
    } else {
        // Equal number of moves in moves_r and moves_b
        // moves_r % 2 == 1 -> the red meeple is last moved by Sherlock.
        // Given that Sherlock starts playing, the red meeple will be moves before Moriarty can make the last, winning move for the black meeple.
        std::cout << 1 - moves_r % 2 << std::endl;
    }
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }

    return 0;
}
