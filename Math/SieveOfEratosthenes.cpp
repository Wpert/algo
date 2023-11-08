#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

template<typename T>
concept Integer = std::is_integral_v<T>;

// i can optimize up to 2 times
template<Integer T>
class SieveOfEratosthenes {
private:
    std::vector<bool> isPrime_;

public:
    SieveOfEratosthenes(size_t max_n) : isPrime_(max_n, true) {
        for (int prime = 2; prime * prime <= max_n; ++prime) { 
            if (isPrime_[prime] == true) {
                // already marked every before
                for (int composite = prime * prime; composite <= max_n; composite += prime) 
                    isPrime_[composite] = false; 
            }
        }

        // love this thing
    }

    bool isPrime(size_t x) {
        assert(x < isPrime_.size());

        return isPrime_[x];
    } 
};

int main() {
    SieveOfEratosthenes<int> sieve(1000);

    for (int i = 900; i < 1000; ++i) {
        if (sieve.isPrime(i))
            std::cout << i << ": true" << std::endl;
    }

    return 0;
}