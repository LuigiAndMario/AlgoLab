#include <iostream>
#include <vector>
#include <map>
#include <iomanip>

#define DEBUG true
#define DEBUG_PRINT(ARG) if (DEBUG) std::cerr << ARG << std::endl;

#define STUDENT_LIMIT 22

void testcase() {
    int n; std::cin >> n; // # students
    int k; std::cin >> k; // max non-playing players
    
    std::vector<int> s(n); // skill levels
    for (int i = 0 ; i < n ; i++) {
        std::cin >> s[i];
    }

    // We split the problem in two
    int half = n / 2;
    int rest = n - half;
    
    // First half
    std::map<std::pair<std::pair<int, int>, int>, int> potential_picks; // [skills of team 1, skills of team 2, # selected players] -> # occurrences
    for (int i = 0 ; i < (1 << half) ; i++) {
        std::vector<int> selected;
        for (int j = 0 ; j < half ; j++) {
            if ((i & (1 << j)) != 0) { // True if the j^th bit of i is 1
                // This allows us to take all the combinations of players from 0 to i
                selected.push_back(s[j]);
            }
        }
        
        int size = selected.size();
        for (int j = 0 ; j < (1 << size) ; j++) {
            int team_1 = 0;
            int team_2 = 0;
            
            for (int l = 0 ; l < size ; l++) {
                // Again, we have here every combination of players from 0 to j - and, we know that team_1 \cap team_2 = \emptyset, so this allows us to get every combinations
                if ((j & (1 << l)) != 0) { // True if the l^th bit of j is 1
                    team_1 += selected[l];
                } else {
                    team_2 += selected[l];
                }
            }
            
            std::pair<std::pair<int, int>, int> pick = std::make_pair(std::make_pair(team_1, team_2), size);
            if (potential_picks.find(pick) == potential_picks.end()) {
                potential_picks[pick] = 1;
            } else {
                potential_picks[pick]++;
            }
        }
    }
    
    std::map<int, std::vector<std::pair<int, int> > > picks; // skill difference -> [# selected players, occurrences]
    for (auto it = potential_picks.begin() ; it != potential_picks.end() ; it++) {
        picks[it->first.first.second - it->first.first.first].push_back(std::make_pair(it->first.second, it->second));
    }
    
    // Now for the rest
    int possible_picks = 0;
    for (int i = 0 ; i < (1 << rest) ; i++) {
        std::vector<int> selected;
        for (int j = 0 ; j < rest ; j++) {
            if ((i & (1 << j)) != 0) { // True if the j^th bit of i is 1
                // All possible combinations
                selected.push_back(s[half + j]);
            }
        }
        
        int size = selected.size();
        for (int j = 0 ; j < size ; j++) {
            int team_1 = 0;
            int team_2 = 0;
            
            for (int l = 0 ; l < size ; l++) {
                if ((j & (1 << l)) != 0) {
                    team_1 += selected[l];
                } else {
                    team_2 += selected[l];
                }
            }
            
            int difference = team_2 - team_1;
            if (picks.find(difference) != picks.end()) {
                std::vector<std::pair<int, int> > vec = picks[difference];
                for (auto it = vec.begin() ; it != vec.end() ; it++) {
                    if (size + it->first >= n - k) {
                        // Enough students picked with that composition - we get all its occurences
                        possible_picks += it->second;
                    }
                }
            }
        }
    }
    
    std::cout << possible_picks << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
