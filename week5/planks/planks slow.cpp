#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> l;
vector<int> combinations;

void display_bits(int x, int n) {
    for (int i = 0 ; i < n ; i++) {
        cerr << ((x & (1 << i)) >> i);
    }
}

bool contains(vector<tuple<int, int> > &combinations, tuple<int, int> candidate) {
    for (auto it = combinations.begin() ; it != combinations.end() ; it++) {
        if (get<0>(*it) == get<1>(candidate) && get<1>(*it) == get<0>(candidate)) {
            return true;
        }
        if (get<0>(*it) == get<0>(candidate) && get<1>(*it) == get<1>(candidate)) {
            return true;
        }
    }
    return false;
}

void testcase() {
    int n; cin >> n;
    
    l = vector<int>(n);
    int side_of_square = 0;
    for (int i = 0 ; i < n ; i++) {
        cin >> l[i];
        side_of_square += l[i];
    }
    side_of_square /= 4;
    
    // Putting the planks in ascending order
    sort(l.begin(), l.end());
    reverse(l.begin(), l.end());
    
    if (n < 4 || l[0] > side_of_square) {
        // Building a square with 3 sides is impossible, and so would it with planks that are bigger than the side of the square
        cout << 0 << endl;
        return;
    }
    
    int available = 0;
    for (int i = 0 ; i < n ; i++) {
        available |= (1 << i);
    }
    
    // IDEA:
    // Part 1: For every possible combinations, regardless of availability, we save all the ones that sum up to side_of_square
    // Part 2: We then find every 4 sets of combinations such that there is no overlap (& = 0), and that use all the planks (| = (1 << n) - 1)
    
    // Part 1
    cerr << "Computing combinations...";
    combinations = vector<int>(0); // Every possible combination of planks that sum up to side_of_square
    for (int i = 0 ; i < (1 << n) ; i++) {
        int length = 0;
        for (int j = 0 ; j < n ; j++) {
            if ((i & (1 << j)) != 0) { // Holds when plank j is in combination i
                length += l[j];
            }
        }
        
        if (length == side_of_square) {
            combinations.push_back(i);
        }
    }
    cerr << combinations.size() << " combinations were found" << endl;
    
    if (combinations.size() < 4) {
        // If we can't build 4 sides, we can't build a square
        cout << 0 << endl;
        return;
    }
    
    /*
    cerr << combinations.size() << " combinations possible:" << endl;
    for (auto it = combinations.begin() ; it != combinations.end() ; it++) {
        display_bits(*it, n); cerr << endl;
    }
     */
    
    
    // Part 2
    // Idea:
    // Computing the compaptibility of singles with singles
    // Then, compare the compaptibility of pairs with singles
    // Lasty, compare the compaptibility of triples and singles
    // It's hard to understand, but it rquires quite some...
    // ...
    // pair-ception! :D
    // (okay, no more jokes, fine...)
    
    // Part 2.1: Computing the compaptibility of singles with singles
    cerr << "Computing single-single compaptibility...";
    vector<tuple<int, int> > compaptible_pairs(0);
    for (auto it = combinations.begin() ; it != combinations.end() ; it++) {
        for (auto jt = it + 1 ; jt != combinations.end() ; jt++) {
            int overlap = *it & *jt;
            
            if (overlap == 0) {
                compaptible_pairs.push_back(make_tuple(*it, *jt));
            }
        }
    }
    cerr << " " << compaptible_pairs.size() << " pairs were found" << endl;
    
    // Part 2.2: Computing pair-single compaptibility
    cerr << "Computing pair-single compaptibility...";
    vector<tuple<int, int, int> > compaptible_triples(0);
    for (auto it = compaptible_pairs.begin() ; it != compaptible_pairs.end() ; it++) {
        for (auto jt = combinations.begin() ; jt != combinations.end() ; jt++) {
            int overlap = (get<0>(*it) | get<1>(*it)) & *jt;
            
            if (overlap == 0) {
                compaptible_triples.push_back(make_tuple(get<0>(*it), get<1>(*it), *jt));
            }
        }
    }
    cerr << " " << compaptible_triples.size() << " triples were found" << endl;
    
    // Part 2.3: Computing triple-single compaptibility
    cerr << "Computing triple-single compaptibility..." << endl;
    int squares = 0;
    for (auto it = compaptible_triples.begin() ; it != compaptible_triples.end() ; it++) {
        for (auto jt = combinations.begin() ; jt != combinations.end() ; jt++) {
            int overlap = (get<0>(*it) | get<1>(*it) | get<2>(*it)) & *jt;
            int planks_used = get<0>(*it) | get<1>(*it) | get<2>(*it) | *jt;
            
            if (overlap == 0 && planks_used == available) {
                squares++;
            }
        }
    }
    cerr << " done" << endl;
    
    // Result
    cout << squares / 12 << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    int i = 0;
    while (t--) {
        cerr << "CASE " << i++ << endl;
        testcase();
    }
    return 0;
}
