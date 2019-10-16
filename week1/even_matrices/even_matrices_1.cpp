#include <iostream>
#include <vector>

using namespace std;

vector<vector<int> > x;

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

        int quads = 0;
        for (int i_1 = 0 ; i_1 < n ; i_1++) {
            for (int j_1 = 0 ; j_1 < n; j_1++) {
                for (int i_2 = i_1 ; i_2 < n ; i_2++) {
                    for (int j_2 = j_1 ; j_2 < n ; j_2++) {
                        int sum = 0;
                        for (int k = i_1 ; k <= i_2 ; k++) {
                            for (int l = j_1 ; l <= j_2 ; l++) {
                                sum += x[k][l];
                            }
                        }
                        quads += (1 - (sum % 2));
                    }
                }
            }
        }

        cout << quads << endl;
    }
}