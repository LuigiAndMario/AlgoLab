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

        int num_pairs = 0;
        for (int i = 0 ; i < n ; i++) {
            int sum = x[i];
            if (sum % 2 == 0) {
                num_pairs++;
            }
            for (int j = i + 1 ; j < n; j++) {
                sum += x[j];
                sum %= 2;
                if (sum == 0) {
                    num_pairs++;
                }
            }
        }

        cout << num_pairs << endl;
    }
}