#include <iostream>
#include <vector>
#include <climits>
#include <bitset>
#include <algorithm>

#define MAX_ROOMS 30 // Given in the description

std::vector<int> b;
std::vector<std::vector<std::pair<int, int> > > lights_to_switch;
std::vector<int> needed;

std::vector<int> get_flips(std::vector<int> current, int light, int N) {
    std::vector<int> needed = current;
    for (int i = 0 ; i < N ; i++) {
        if ((light & (1 << i)) != 0) {
            for (int j = 0 ; j < b.size() ; j++) {
                needed[j] += lights_to_switch[i][j].first - lights_to_switch[i][j].second;
            }
        }
    }

    return needed;
}

void testcase() {
    int N; std::cin >> N; // # of switches
    int M; std::cin >> M; // # of rooms

    b = std::vector<int>(M); // target brightness for each room
    for (int i = 0 ; i < M ; i++) {
        std::cin >> b[i];
    }

    lights_to_switch = std::vector<std::vector<std::pair<int, int> > >(N, std::vector<std::pair<int, int> >(M));
    // [i][j] = for switch i and room j, there are .first lights initially on, and .second lights initially off
    std::vector<int> rooms(M, 0); // rooms[i] = x === x lights are on in room i
    for (int i = 0 ; i < N ; i++) {
        for (int j = 0 ; j < M ; j++) {
            int on_j; std::cin >> on_j;
            int off_j; std::cin >> off_j;
            lights_to_switch[i][j] = std::make_pair(on_j, off_j);
            rooms[j] += on_j;
        }
    }

    needed = std::vector<int>(b);
    for (int i = 0 ; i < M ; i++) {
        // Removing the already turned on lights
        needed[i] -= rooms[i];
    }

    int minimum_flips = INT_MAX;
    int half = N / 2;
    int rest = N - half;
    std::vector<std::pair<std::vector<int>, int> > first_pass(1 << half); // [switch index] = [nb flips to light up each room][nb lit rooms]
    for (int i = 0 ; i < (1 << half) ; i++) {
        std::bitset<MAX_ROOMS> room_bits(i);
        std::vector<int> num_flips = get_flips(needed, i, N);
        first_pass[i] = std::make_pair(num_flips, room_bits.count());
    }

    std::sort(first_pass.begin(), first_pass.end());

    std::vector<std::vector<int> > second_pass(first_pass.size());
    for (int i = 0 ; i < first_pass.size() ; i++) {
        second_pass[i] = first_pass[i].first;
    }

    for (int i = 0 ; i < (1 << rest) ; i++) {
        int flips = 0;
        std::vector<int> candidate(M, 0);
        for (int j = half ; j < N ; j++) {
            if ((i & (1 << (j - half))) != 0) {
                flips++;
                for (int k = 0 ; k < M ; k++) {
                    candidate[k] += lights_to_switch[j][k].second - lights_to_switch[j][k].first; // What we would lose if we flipped that switch
                }
            }
        }

        auto it = lower_bound(second_pass.begin(), second_pass.end(), candidate);
        if (*it == candidate) {
            int index = it - second_pass.begin();
            minimum_flips = std::min(minimum_flips, flips + first_pass[index].second);
        }
    }

    if (minimum_flips == INT_MAX) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << minimum_flips << std::endl;
    }
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
