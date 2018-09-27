#include <iostream>
#include <vector>

int main() {
    int t; std::cin >> t;
    for (int a = 0 ; a < t ; a++) {
        int n; std::cin >> n;

        std::vector<int> v(n);
        for (int j = 0 ; j < n ; j++) {
            std::cin >> v[j];
        }

        std::vector<int> even(n);
        std::vector<int> odd(n);

        if (v[0] == 0) {
            even[0]++;
        } else {
            odd[0]++;
        }
        int even_pairs = even[0];

        for (int i = 1 ; i < n ; i++) {
            if (v[i] == 0) {
                even[i] = even[i - 1] + 1;
                odd[i] = odd[i - 1];
            } else {
                even[i] = odd[i - 1];
                odd[i] = even[i - 1] + 1;
            }
            even_pairs += even[i];
        }

        std::cout << even_pairs << std::endl;
    }

    return 0;
}
