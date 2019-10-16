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
            for (int j = i ; j < n; j++) {
                int sum = 0;
                for (int k = i; k <=j ; k++) {
                    sum += x[k];
                }
                if (sum % 2 == 0) {
                    num_pairs++;
                }
            }
        }

        cout << num_pairs << endl;
    }
}