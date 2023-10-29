#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <concepts>
#include <algorithm>

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
class UnionSets {
private:
    std::map<T, T> parent_;
    // std::vector<T> parent_;
    std::map<T, long long> rank_;
    // std::vector<T> rank_;

public:
    UnionSets() = default;
    
    void newSet(T elem) {
        this->parent_[elem] = elem;
        this->rank_[elem] = 0; 
    }

    T findSet(const T elem) {
        if (elem == parent_[elem]) {
            return elem;
        }

        return parent_[elem] = findSet(parent_[elem]);
    }
    
    void unionSets(T a, T b) {
        a = findSet(a);
        b = findSet(b);

        if (a != b) {
            if (rank_[a] < rank_[b])
                std::swap(a, b);
            parent_[b] = a;
            
            if (rank_[a] == rank_[b])
                ++rank_[a];
        }
    }
};

template<Integral T>
class UnionSets<T> {
private:
    std::vector<T> parent_;
    std::vector<T> rank_;

public:
    UnionSets() = delete;
    UnionSets(size_t MAX_N) : parent_(MAX_N), rank_(MAX_N) {}
    
    void newSet(int elem) {
        this->parent_[elem] = elem;
        this->rank_[elem] = 0; 
    }

    int findSet(const T elem) {
        if (elem == parent_[elem]) {
            return elem;
        }

        return parent_[elem] = findSet(parent_[elem]);
    }
    
    void unionSets(T a, T b) {
        a = findSet(a);
        b = findSet(b);

        if (a != b) {
            if (rank_[a] < rank_[b])
                std::swap(a, b);
            parent_[b] = a;
            
            if (rank_[a] == rank_[b])
                ++rank_[a];
        }
    }
};


// task example
int main() {
    // there are 3 different types of magic:
    // 0-th magic contains only zero element
    // 1-th magic contains odd numbers
    // 2-th magic contains even number
    // so i need to separate them somehow
    UnionSets<long long> MagicNumbers(100);
    // make different sets for every number
    int range = 10;

    for (int i = 0; i < range + 1; ++i) {
        MagicNumbers.newSet(i);
    }
    // and union it
    for (int i = 1; i < range - 1; ++i) {
        MagicNumbers.unionSets(i, i + 2);
    }

    for (int i = 0; i < range + 1; ++i) {
        std::cout << i << ": " << MagicNumbers.findSet(i) << std::endl;
    }
    // task solved

    // obv we can use it for any other trivial tasks like
    // finid strongly connected components
    // 

    return 0;
}