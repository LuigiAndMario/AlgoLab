#include <iostream>
#include <vector>

using namespace std;

vector<vector<int> > x;
vector<vector<int> > sums;

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        x = vector<vector<int> >(n);
        for (int i = 0 ; i < n ; i++) {
            x[i] = vector<int>(n);
            for (int j = 0 ; j < n ; j++) {
                cin >> x[i][j];
            }
        }

        sums = vector<vector<int> >(n + 1);
        for (int i = 0 ; i <= n ; i++) {
            sums[i] = vector<int>(n + 1);
            sums[i][0] = 0;
            sums[0][i] = 0;
        }

        for (int i = 0 ; i < n ; i++) {
            for (int j = 0 ; j < n ; j++) {
                sums[i + 1][j + 1] = sums[i][j] + sums[i + 1][j] + sums[i][j + 1] + x[i][j];
            }
        }

        int quads = 0;
        for (int i_1 = 1 ; i_1 <= n ; i_1++) {
            for (int i_2 = i_1 ; i_2 <= n ; i_2++) {
                for (int j_1 = 1 ; j_1 <= n ; j_1++) {
                    for (int j_2 = j_1 ; j_2 <= n ; j_2++) {
                        int current = (sums[j_2][i_2] - sums[j_2][i_1 - 1]) - (sums[j_1 - 1][i_2] - sums[j_1 - 1][i_1 - 1]);

                        if (i_1 == 1 && i_2 == 1 && j_1 == 1 && j_2 == 2) {
                            // cerr << "sums[i_2][j_2] = " << sums[i_2][j_2] << ", sums[i_2][j_1 - 1] = " << sums[i_2][j_1 - 1] << ", sums[i_1 - 1][j_2] = " << sums[i_1 - 1][j_2] << ", sums[i_1 - 1][j_1 - 1] = " << sums[i_1 - 1][j_1 - 1] << endl;
                        }
                        
                        if (current % 2 == 0) {
                            quads++;
                            // cerr << "Adding quad " << i_1 << " - " << i_2 << ", " << j_1 << " | " << j_2 << endl;
                        }
                    }
                }
            }
        }

        cout << quads << endl;
    }
}