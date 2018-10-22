#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_JEDI 10

std::vector<std::pair<int, int> > segments; // segments[i] = <a, b> === Jedi `i` defends from `a` to `b` (modular)
std::vector<int> coverage;

bool earliest_deadline(std::pair<int, int> segment_1, std::pair<int, int> segment_2) {
    return segment_1.second < segment_2.second;
}

int get_max(std::vector<std::pair<int, int> > &tasks, int min, int max) {
    int a = min;
    int fighting_jedis = 0;
    for (std::vector<std::pair<int, int> >::iterator i = tasks.begin() ; i != tasks.end() ; i++) {
        if (i->first > a && i->second < max) {
            a = i->second;
            fighting_jedis++;
        }
    }

    return fighting_jedis;
}

void testcase() {
    int n; std::cin >> n; // # Jedi
    int m; std::cin >> m; // # Segments

    segments = std::vector<std::pair<int, int> >(n);
    coverage = std::vector<int>(m);
    for (int i = 0 ; i < n ; i++) {
        int a; std::cin >> a;
        int b; std::cin >> b;
        segments[i] = std::make_pair(--a, --b);

        coverage[a]++; // One jedi starts covering at a
        coverage[b]--; // One jedi stops covering at b
        if (a > b) {
            coverage[0]++; // One jedi starts covering at 0
        }
    }

    int s = 0; // The segment with <= 10 overlaps
    int min_jedi = n;
    int jedi = 0;
    for (int i = 0 ; i < m ; i++) {
        jedi += coverage[i];
        if (jedi <= MAX_JEDI && jedi <= min_jedi) {
            s = i;
            min_jedi = jedi;
        }
    }

    std::vector<std::pair<int, int> > overlapping_tasks;
    std::vector<std::pair<int, int> > non_overlapping_tasks;
    for (int i = 0 ; i < n ; i++) {
        int a = segments[i].first;
        int b = segments[i].second;

        std::pair<int, int> adjusted = std::make_pair(a - s + (a <= s ? m : 0), b - s + (b <= s ? m : 0));

        if ((a <= b && a <= s && s <= b) || (a > b && (a <= s || s <= b))) {
            overlapping_tasks.push_back(adjusted);
        } else {
            non_overlapping_tasks.push_back(adjusted);
        }
    }

    // We want to have the most Jedi fight at the same time (=== minimise the number of Jedi just standing there)
    std::random_shuffle(non_overlapping_tasks.begin(), non_overlapping_tasks.end());
    std::sort(non_overlapping_tasks.begin(), non_overlapping_tasks.end(), earliest_deadline);

    int fighting_jedis = get_max(non_overlapping_tasks, -1, m);
    for (std::vector<std::pair<int, int> >::iterator i = overlapping_tasks.begin() ; i != overlapping_tasks.end() ; i++) {
        fighting_jedis = std::max(fighting_jedis, get_max(non_overlapping_tasks, i->second % m, i->first) + 1);
    }

    std::cout << fighting_jedis << std::endl;

}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
