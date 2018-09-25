#include <iostream>
#include <vector>

int main() {
    int t; std::cin >> t;
    for (int i = 0 ; i < t ; i++) {
        int n; std::cin >> n;

        std::vector<std::vector<int> > matrix(n, std::vector<int>(n));
        for (int j = 0 ; j < n ; j++) {
            for (int k = 0 ; k < n ; k++) {
                std::cin >> matrix.at(j).at(k);
            }
        }

        std::vector<std::vector<int> > sums(n, std::vector<int>(n));
        for (int j = 0 ; j < n ; j++) {
            int sum = 0;
            for (int k = 0 ; k < n ; k++) {
                sum += matrix[j][k];
                sums[j][k] += sum;
            }
        }

        int even_quads = 0;
        // For every segment k..j, we get the sum over a particular line l.
        for (int j = 0 ; j < n ; j++) {
            for (int k = j ; k < n ; k++) {
                int even = 0;
                int odd = 0;

                for (int l = 0 ; l < n ; l++) {
                    int sum = sums[l][k] - (j == 0 ? 0 : sums[l][j - 1]);

                    if (l == 0) {
                        // First line: we simply set even or odd to 1 regarding on the quad's parity.
                        if (sum % 2 == 0) {
                            even = 1;
                        } else {
                            odd = 1;
                        }

                    } else {
                        // Further lines: we consider this line, and the one(s) before it.
                        if (sum % 2 == 0) {
                            // Sum is even -> we simply increment the number of even quads.
                            even++;
                        } else {
                            // Sum is odd -> it becomes even with an odd number of odd quads.
                            // Therefore, there is now 'odd' even quads (all the odd quads become even),
                            // and 'even' + 1 odd quads (all the previous even quads become odd, plus the current one).
                            int odd_tmp = even + 1;
                            even = odd;
                            odd = odd_tmp;
                        }
                    }
                    even_quads += even;
                }
            }
        }

        std::cout << even_quads << std::endl;

    }

    return 0;
}
