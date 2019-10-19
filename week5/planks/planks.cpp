#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <tuple>
#include <bitset>

using namespace std;

const int b_size = 21; // Because there are <= 20 planks

vector<int> l;

int length_of(bitset<b_size> combination, vector<int> &lengths) {
    int n = lengths.size();
    int length = 0;
    for (int i = 0 ; i < n ; i++) {
        if (combination.test(i)) {
            length += lengths[i];
        }
    }
    return length;
}

vector<vector<int> > all_combinations(vector<int> &Si, int side_of_square) {
    int n = Si.size();
    vector<vector<int> > combinations(0);
    
    // We loop over all possible pairs of combinations
    for (int i = 0 ; i < (1 << n) ; i++) {
        bitset<b_size> ci(i);
        for (int j = 0 ; j < (1 << n) ; j++) {
            bitset<b_size> cj(j);
            
            auto i_inter_j = ci & cj;
            auto i_minus_j = ci & ~cj;
            auto j_minus_i = ~ci & cj;
            auto i_union_j_bar = ~(ci | cj);
            
            // Lengths of all the sides
            vector<int> lengths = {
                length_of(i_inter_j, Si),
                length_of(i_minus_j, Si),
                length_of(j_minus_i, Si),
                length_of(i_union_j_bar, Si)
            };
            
            if (*max_element(lengths.begin(), lengths.end()) <= side_of_square) {
                combinations.push_back(lengths);
            }
        }
    }
    
    return combinations;
}


void testcase() {
    int n; cin >> n;
    
    l = vector<int>(n);
    int perimeter = 0;
    for (int i = 0 ; i < n ; i++) {
        cin >> l[i];
        perimeter += l[i];
    }
    int side_of_square = perimeter / 4;
    
    // Putting the planks in descending order
    sort(l.rbegin(), l.rend());
    
    if (n < 4 || l[0] > side_of_square) {
        // Building a square with 3 sides is impossible, and so would it with planks that are bigger than the side of the square
        cout << 0 << endl;
        return;
    }
    
    int split = n / 2;
    
    vector<int> S1(split);
    copy(l.begin(), l.begin() + split, S1.begin());
    
    vector<int> S2(n - split);
    copy(l.begin() + split, l.end(), S2.begin());
    
    vector<vector<int> > combinations_left = all_combinations(S1, side_of_square);
    vector<vector<int> > combinations_right = all_combinations(S2, side_of_square);
    // We sort it, cause we'll need that later
    sort(combinations_right.begin(), combinations_right.end());
    
    long num_squares = 0;
    // For all the combinations on the left, we try to find all the combinations on the right that make it sum up to a square
    for (auto it = combinations_left.begin() ; it != combinations_left.end() ; it++) {
        // First, we find how much is needed to make the sides of the left combination reach side_of_square
        vector<int> needed(0);
        for (auto jt = it->begin() ; jt != it->end() ; jt++) {
            needed.push_back(side_of_square - *jt);
        }
        
        // Now that it's sorted, we know that every vector that completes a square is gonna be side-to-side, so in that range
        vector<vector<int> >::iterator a, b;
        tie(a, b) = equal_range(combinations_right.begin(), combinations_right.end(), needed);
        num_squares += (b - a);
    }
    
    cout << num_squares / 24 << endl; // Don't ask me why, I just got 24 times the correct results
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
