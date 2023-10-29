#include <iostream>
#include <vector>
#include <concepts>

template<typename T>
concept GoodArray = requires (T arr) {
    { arr[0] } -> std::convertible_to<long long>;

    arr.size();
    arr.data();
    arr.begin();
    arr.end();
};

template<GoodArray T>
class PrefSumms {
private:
    std::vector<long long> pref_;

public:
    PrefSumms() = delete;
    PrefSumms(T& arr) : pref_(arr.size() + 1, 0){
        for (size_t i = 1; i < pref_.size(); ++i) {
            pref_[i] += pref_[i - 1] + arr[i - 1];
        }
    }

    std::vector<long long> getArr() const {
        return this->pref_;
    }

    size_t size() {
        return pref_.size();
    }

    auto data() {return pref_.data();}
    auto begin() {return pref_.begin();}
    auto end() {return pref_.end();}
};

int main() {
    std::vector<int> exampleArray{1, 100, 2, 50, 9, 8, 5};
    PrefSumms p(exampleArray);

    auto arr = p.getArr();

    for (auto e : arr) {
        std::cout << e << ' ';
    }
    std::cout << std::endl;

    return 0;
}