#include <iostream>
#include <vector>

using namespace std;

vector<vector<int> > x;
vector<vector<int> > sums;

int choose(int n, int k) {
    if (n < k) {
        return 0;
    }

    int prod = 1;
    for (int i = 1; i <= k; i++) {
        prod *= ((n + 1 - i) / (double) i);
    }

    return prod;
}

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        x = vector<vector<int> >(n);
        for (int i = 0; i < n; i++) {
            x[i] = vector<int>(n);
            for (int j = 0 ; j < n ; j++) {
                cin >> x[i][j];
            }
        }

        sums = vector<vector<int> >(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            int sum = 0;

            for (int j = 0; j < n; j++) {
                sum += x[i][j];
                sums[i][j] = sum;
            }
        }

        int quads = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i ; j < n ; j++) {
                int even = 0;
                int odd = 0;

                for (int k = 0; k < n; k++) {
                    int sum = sums[k][j] - (i == 0 ? 0 : sums[k][i - 1]);

                    if (k == 0) {
                        if (sum % 2 == 0) {
                            even++;
                        } else {
                            odd++;
                        }
                    } else {
                        if (sum % 2 == 0) {
                            even++;
                        } else {
                            int tmp = odd;
                            
                            odd = even + 1;
                            even = tmp;
                        }
                    }

                    quads += even;
                }
            }
        }

        cout << quads << endl;
    }
}