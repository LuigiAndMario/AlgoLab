#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <tuple>

using namespace std;

struct Drink {
    int c;
    int v;
};

const tuple<int, int> MEMO_BASE = make_tuple(0, 0);

vector<Drink> drink;
vector<vector<tuple<int, int> > > memo; // drink index, quantity so far -> (cost, drinks)

void testcase() {
    int n; cin >> n;
    int k; cin >> k;
    
    drink = vector<Drink>(n);
    int v_max = INT_MIN;
    for (int i = 0 ; i < n ; i++) {
        int c; cin >> c;
        int v; cin >> v;
        drink[i] = {c, v};
        v_max = max(v_max, v);
    }
    int q_max = k + v_max;
    
    memo = vector<vector<tuple<int, int> > >(n, vector<tuple<int, int> >(q_max, MEMO_BASE));
    
    for (int j = 1 ; j < q_max ; j++) {
        // This value is equal to 1 when j <= vj,
        // To 1 when vj < j <= 2vj
        // To 3 when 2vj < j <= 3vj
        // And  in general, to n when (n - 1)vj < j <= nvj
        int how_many_j_in_vj = ((j - 1) / drink[0].v) + 1;
        memo[0][j] = make_tuple(how_many_j_in_vj * drink[0].c, 1);
    }
    
    for (int i = 1 ; i < n ; i++) {
        memo[i][0] = memo[i - 1][0]; // For this drink and nothing, we pull the value from the previous drink and nothing
        
        int vi = drink[i].v;
        int ci = drink[i].c;
        
        for (int j = 1 ; j < q_max ; j++) {
            tuple<int, int> with = memo[i][j - vi]; // Equivalent to taking one bottle of this drink, and staying here
            tuple<int, int> without = memo[i - 1][j]; // Equivalent to moving to the previous bottle without changing anything to the volume
            
            int c_min;
            int d_max;
            
            // Considering the case where we take a bottle (with)
            if (j < vi) {
                // The volume of the current bottle is more than what we already have - we ditch everything and replace it by that (for now)
                // (because otherwise we would have v_previous + vi, where vi > j
                c_min = ci;
                d_max = 1;
            } else {
                // We take one bottle of this drink
                c_min = ci + get<0>(with);
                d_max = get<1>(with);
                if (with == memo[i - 1][j - vi]) {
                    // This is not the same drink as we took from before - we increment the number of drinks
                    d_max++;
                }
            }
            
            // Considering the case where we don't take a bottle (without)
            if (get<0>(without) < c_min) {
                // Cheaper cost -> better
                c_min = get<0>(without);
                d_max = get<1>(without);
            } else if (get<0>(without) == c_min && get<1>(without) > d_max) {
                // Same price, but more drinks -> better
                d_max = get<1>(without);
            }
            
            memo[i][j] = make_tuple(c_min, d_max);
        }
    }
    
    cout << get<0>(memo[n - 1][k]) << " " << get<1>(memo[n - 1][k]) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
