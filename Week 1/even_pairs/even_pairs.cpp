#include <iostream>
#include <vector>

int main() {
    int t; std::cin >> t;
    for (int a = 0 ; a < t ; a++) {
        int n; std::cin >> n;
        
        std::vector<int> v(n);
        for (int j = 0 ; j < n ; j++) {
            std::cin >> v.at(j);
        }

        // First approach - too slow! O(n^3)
        /*
        int even_pairs = 0;
        for (int j = 0 ; j < n ; j++) {
            for (int i = 0 ; i <= j ; i++) {
                int sum = 0;
                for (int k = i ; k <= j ; k++) {
                    sum += v.at(k);
                }
                even_pairs += sum % 2 == 0 ? 1 : 0;
            }
        }

        std:: cout << even_pairs << std::endl;
        */

        // Second approach - too slow! O(n^2)
        // We have that x_i + ... + x_j = (x_0 + ... + x_j) - (x_0 + ... + x_{i-1})
        /*
        std::vector<int> Si(0);
        for (int i = 0 ; i < n ; i++) {
            Si.push_back(0);
            for (int j = 0 ; j <= i ; j++) {
               Si.at(i) += v.at(j);
            }
        }

        int even_pairs = 0;
        for (int j = 0 ; j < n ; j++) {
            for (int i = 0 ; i < j ; i++) {
                int sum = Si.at(j) - Si.at(i);
                even_pairs += sum % 2 == 0 ? 1 : 0;
            }
            even_pairs += Si.at(j) % 2 == 0 ? 1 : 0;
        }

        std::cout << even_pairs << std::endl;
        */

        // Third approach
        std::vector<int> Si(n);
        for (int i = 0 ; i < n ; i++) {
            Si.at(i) = 0;
            for (int j = 0 ; j <= i ; j++) {
                Si.at(i) += v.at(j);
            }
        }

        int even = 0;
        int odd = 0;
        for (int i = 0 ; i < n ; i++) {
            if (Si.at(i) % 2 == 0) {
                even++;
            } else {
                odd++;
            }
        }

        int even_pairs = (even * (even - 1))/2;
        even_pairs += (odd * (odd - 1))/2;
        even_pairs += even;

        std::cout << even_pairs << std::endl;
    }

    return 0;
}
