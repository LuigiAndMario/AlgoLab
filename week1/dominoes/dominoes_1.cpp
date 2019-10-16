#include <iostream>
#include <vector>

using namespace std;

int main() {
    int t; cin >> t;
    while (t--) {
        // Parsing the input
        int n; cin >> n;
        vector<int> dominoes(n);
        for (int i = 0 ; i < n ; i++) {
            cin >> dominoes[i];
        }

        int d_i = 0;
        int r_i = d_i + dominoes[d_i] - 1;
        int d_j = d_i + 1;
        // cerr << "d_i = " << d_i << ", r_i = " << r_i << ", d_j = " << d_j << endl;
        if (r_i == 0) {
            cout << 1 << endl;
        } else {
            while (d_j != d_i && d_j <= r_i && d_j < n) {
                int r_j = d_j + dominoes[d_j] - 1;
                // cerr << "\tr_j = " << r_j << endl;
                if (r_j >= r_i) {
                    d_i = d_j;
                    r_i = r_j;
                    // cerr << "\t\td_i = " << d_i << ", r_i = " << r_i << endl;  
                }
                d_j++;
                // cerr << "\td_j = " << d_j << endl;
            }

            cout << min(d_j, n) << endl;
        }
    }
}