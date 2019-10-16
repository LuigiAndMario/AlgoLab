#include <iostream>
#include <vector>

using namespace std;

int choose(int n, int k) {
    if (n < k) {
        return 0;
    }

    int prod = 1;
    for (int i = 1 ; i <= k ; i++) {
        prod *= ((n + 1 - i) / (double) i);
    }

    return prod;
}

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> x(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> x[i];
        }

        vector<int> sums(n);
        sums[0] = x[0];
        int even = 0;
        int odd = 0;
        if (sums[0] % 2 == 0) {
            even++;
        } else {
            odd++;
        }
        for (int i = 1 ; i < n ; i++) {
            sums[i] = sums[i - 1] + x[i];
            if (sums[i] % 2 == 0) {
                even++;
            } else {
                odd++;
            }
        }

        int pairs = (even + choose(even, 2)) + choose(odd, 2);

        cout << pairs << endl;
    }
}