#include <iostream>
#include <vector>
#include <climits>

using namespace std;

typedef vector<int> vec;
typedef vector<vec>vec2;
typedef vector<vec2> vec3;

const int MEMO_INIT = -1;

vec2 roads; // roads[i] = all the cities reachable from i
vec c;
vec3 memo;

int find(int bulb, bool turn_on, bool parent_on) {
    if (roads[bulb].size() == 0) {
        // Final stage
        return parent_on ? 0 : c[bulb];
    }
    
    if (memo[turn_on][parent_on][bulb] != MEMO_INIT) {
        return memo[turn_on][parent_on][bulb];
    }
    
    int result;
    if (turn_on) {
        // I am on, which costs this much, and the ones below me do whatever is the cheapest
        result = c[bulb];
        for (auto it = roads[bulb].begin() ; it != roads[bulb].end() ; it++) {
            result += min(find(*it, false, turn_on), find(*it, true, turn_on));
        }
    } else {
        // I am not on...
        result = 0;
        if (parent_on) {
            // ...but I got light from the one above me - all the ones below me do whatever is the cheapest
            for (auto it = roads[bulb].begin() ; it != roads[bulb].end() ; it++) {
                result += min(find(*it, false, turn_on), find(*it, true, turn_on));
            }
        } else {
            // ...and neither is the one above me
            // I need at least one light from the ones below me - all others do whatever is the cheapest
            vec results_on(roads[bulb].size()); // results if everyone turns the lights on
            vec results_whatever(roads[bulb].size()); // best results
            int all = 0; // sum of all the best results
            // We try to find the one guy that will be the cheapest to turn on
            // To do so, we compute the best results...
            for (int i = 0 ; i < roads[bulb].size() ; i++) {
                results_on[i] += find(roads[bulb][i], true, turn_on);
                results_whatever[i] += min(find(roads[bulb][i], true, turn_on), find(roads[bulb][i], false, turn_on));
                all += results_whatever[i];
            }
            // ...and we get the minimum of the best results but one light is turned on, for any possible bulb to set to on
            result = INT_MAX;
            for (int i = 0 ; i < roads[bulb].size() ; i++) {
                result = min(result, all - results_whatever[i] + results_on[i]);
            }
        }
    }
    
    memo[turn_on][parent_on][bulb] = result;
    return result;
}

void testcase() {
    int n; cin >> n;
    roads = vec2(n, vector<int>());
    for (int i = 0 ; i < n - 1 ; i++) {
        int from; cin >> from;
        int to; cin >> to;
        if (from != to) {
            roads[from].push_back(to);
        }
    }
    c = vec(n);
    for (int i = 0 ; i < n ; i++) {
        cin >> c[i];
    }
    
    memo = vec3(2, vec2(2, vec(n, MEMO_INIT)));
    
    cout << min(find(0, false, false), find(0, true, false)) << endl;
    
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
