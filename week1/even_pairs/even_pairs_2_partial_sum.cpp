#include <iostream>
#include <vector>

using namespace std;

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> x(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> x[i];
        }

        vector<int> sums(n + 1);
        sums[0] = 0;
        for (int i = 0 ; i < n ; i++) {
            sums[i + 1] = sums[i] + x[i];
        }

        int pairs = 0;
        for (int i = 1; i <= n ; i++) {
            for (int j = i; j <= n; j++) {
                bool parity = (sums[j] - sums[i - 1]) % 2 == 0;
                if (parity) {
                    pairs++;
                    // cerr << "Pair " << i << ", " << j << " added" << endl;
                }
            }
        }

        cout << pairs << endl;
    }
}