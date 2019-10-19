#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> l;

int length_of(int x, int n) {
    int length = 0;
    for (int i = 0 ; i < n ; i++) {
        if (x & (1 << i)) {
            length += l[i];
        }
    }
    return length;
}

vector<vector<int> > all_combinations(vector<int> &Si, int side_of_square) {
    int n = Si.size();
    vector<vector<int> > combinations(0);
    
    for (int i = 0 ; i < (1 << n) ; i++) {
        for (int j = 0 ; j < (1 << n) ; j++) {
            // i, j two possible combinations
            int i_inter_j = i & j;
            int i_minus_j = i & ~j;
            int j_minus_i = ~i & j;
            int i_union_j_bar = ~(i | j);
            
            // Lengths of all the sides
            vector<int> lengths = {
                length_of(i_inter_j, n),
                length_of(i_minus_j, n),
                length_of(j_minus_i, n),
                length_of(i_union_j_bar, n)
            };
            
            bool less_than_side = true;
            for (int i = 0 ; i < 4 ; i++) {
                if (lengths[i] > side_of_square) {
                    less_than_side = false;
                }
            }
            
            if (less_than_side) {
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
    
    // Putting the planks in ascending order
    sort(l.begin(), l.end());
    reverse(l.begin(), l.end());
    
    if (n < 4 || l[0] > side_of_square) {
        // Building a square with 3 sides is impossible, and so would it with planks that are bigger than the side of the square
        cout << 0 << endl;
        return;
    }
    
    int split = n / 2;
    int rest = n - split;
    
    vector<int> S1(split);
    vector<int> S2(rest);
    
    for (int i = 0 ; i < split ; i++) {
        S1[i] = l[i];
    }
    vector<vector<int> > combinations_left = all_combinations(S1, side_of_square);
    
    for (int i = split ; i < n ; i++) {
        S2[split - i] = l[i];
    }
    vector<vector<int> > combinations_right = all_combinations(S2, side_of_square);
    // sort(combinations_right.begin(), combinations_right.end());
    
    int num_squares = 0;
    // For all the combinations on the left, we try to find all the combinations on the right that make it sum up to a square
    for (auto it = combinations_left.begin() ; it != combinations_left.end() ; it++) {
        for (auto jt = combinations_right.begin() ; jt != combinations_right.end() ; jt++) {
            if (it->at(0) + jt->at(0) == side_of_square
                && it->at(1) + jt->at(1) == side_of_square
                && it->at(2) + jt->at(2) == side_of_square
                && it->at(3) + jt->at(3) == side_of_square) {
                // Look, I *know* it's dirty as shit, but it works, so whatcha gonna do? huh?
                num_squares++;
            }
        }
    }
    
    cout << num_squares << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
