#include <iostream>
#include <vector>

using namespace std;

vector<int> v;

int abs(int x) {
    return x < 0 ? -x : x;
}

int main() {
    ios::sync_with_stdio(false);

    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        int k; cin >> k;

        v = vector<int>(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> v[i];
        }

        int i = 0;
        int j = 0;
        int sum = v[0];

        int i_best = i;
        int j_best = j;
        int v_best = abs(k - sum);

        j++;
        sum += v[1];

        while (v_best != 0 && j < n && i < n) {
            int val = abs(k - sum);

            if (val < v_best) {
                // Just scored better!
                // Let's save this for now...
                i_best = i;
                j_best = j;
                v_best = val;

                // ... and expand on the right
                if (j == n - 1) {
                    break;
                }
                j++;
                sum += v[j];
            } else {
                // Just scored worse...
                // Let's shrink on the left
                if (i == j) {
                    // We can't shrink, so we expand on the right instead
                    if (j == n - 1) {
                        break;
                    }
                    j++;
                    sum += v[j];
                } else {
                    sum -= v[i];
                    i++;
                }
            }
        }

        cout << i_best << " " << j_best << endl;
    }

    return 0;
}