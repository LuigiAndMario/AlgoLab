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
        int diff_best = abs(k - sum);

        bool safe = true;

        while (sum != k && safe) {
            if (sum < k) {
                // Too small - expanding
                j++;
                sum += v[j];

                if (j >= n) {
                    safe = false;
                }
            } else if (sum > k) {
                sum -= v[i];
                i++;
            }

            int diff = abs(k - sum);
            if (diff < diff_best) {
                diff_best = diff;
                i_best = i;
                j_best = j;
            }
        }

        if (sum == k) {
            i_best = i;
            j_best = j;
        }

        cout << i_best << " " << j_best << endl;
    }

    return 0;
}