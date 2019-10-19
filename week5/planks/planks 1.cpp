#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> l;

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
    
    // Assumption for test 1:
    // - n <= 8 (i.e. be as slow as you want)
    // - the answer is 0 or 1 (i.e. if you can make squares, none of them is different)
    int available = 0;
    for (int i = 0 ; i < n ; i++) {
        available |= (1 << i);
    }
    
    // IDEA: check every combination of available planks for each side. If it is equal to side_length, we mark them as used and move onto the next side
    int sides_done = 0;
    for (int i = 0 ; i < (1 << n) ; i++) {
        if ((available & i) == i) { // Holds when all the planks in the combination i are available
            int length = 0;
            for (int j = 0 ; j < n ; j++) {
                if ((i & (1 << j)) != 0) { // Holds when plank j is in combination i
                    length += l[j];
                }
            }
            if (length == side_of_square) {
                sides_done++;
                available ^= i; // marking all the bits in combination i as done
            }
        }
    }
    
    if (available || sides_done < 4) {
        // Not all the planks are used
        cout << 0 << endl;
    } else {
        cout << 1 << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
