#include <iostream>
#include <algorithm>
#include <vector>
#include <concepts>
#include <type_traits>
#include <cassert>

template<typename T>
concept GoodArray = requires (T arr) {
    { arr[0] } -> std::convertible_to<long long>;
    arr.size();
    arr.data();
    arr.begin();
    arr.end();
};

template<GoodArray T>
class PrefFunction {
private:
    std::vector<size_t> pref_;

public:
    PrefFunction() = delete;
    PrefFunction(T& str) : pref_(str.size(), 0) {
        size_t n = str.size();
        for (size_t i = 1; i < n; ++i) {
            int j = pref_[i - 1];

            while (j > 0 && str[i] != str[j])
                j = pref_[j - 1];
            
            if (str[i] == str[j])
                ++j;
            pref_[i] = j;
        }
    }

    const std::vector<size_t>& GetPref() {
        return this->pref_;
    }

    const size_t operator [] (int index) {
        return pref_[index];
    }

    const size_t size() const noexcept {
        return this->pref_.size();
    }

    auto data() { return this->pref_.data(); }
    auto begin() { return this->pref_.begin(); }
    auto end() { return this->pref_.end(); }
};

template<GoodArray T, GoodArray U>
bool testFunction(const T& rightArray, const U& toTestArray) {
    if (!(rightArray.size() == toTestArray.size()))
        return false;

    size_t n = rightArray.size();
    for (size_t i = 0; i < n; ++i)
        if (rightArray[i] != toTestArray[i])
            return false;

    return true;
}

template<GoodArray From, GoodArray To>
To convertToVec(From& arr) {
    size_t n = arr.size();
    To temp(n);
    for (size_t i = 0; i < n; ++i) {
        temp[i] = arr[i];
    }
    return temp;
}

int main () {
    {
        std::string testInput = "abacaba";
        std::vector<size_t> testAnswer{0, 0, 1, 0, 1, 2, 3};
        
        PrefFunction<std::string> firstTry(testInput);
        assert(testFunction(testAnswer, firstTry.vec()));
        std::cout << "First test passed! 1 assert passed." << std::endl;

        // size_t n = firstTry.Size(); 
        // size_t sum = 0;
        // for (size_t i = 0; i < n; ++i) {
        //     sum += firstTry[i];
        // }
        // std::cout << "Sum of first input example: " << sum << std::endl;
    }

    {
        std::string testStrInput = "mynameisalexmynameisntmyles";
        auto testIntVecInput = convertToVec<std::string, std::vector<int>>(testStrInput);
        auto testLLVecInput = convertToVec<std::string, std::vector<long long>>(testStrInput);

        std::vector<size_t> testAnswer{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 1, 2, 0, 0, 0};
        
        PrefFunction<std::string> firstTry(testStrInput);
        PrefFunction<std::vector<int>> secondTry(testIntVecInput);
        PrefFunction<std::vector<long long>> thirdTry(testLLVecInput);

        assert(testFunction(testAnswer, firstTry));
        assert(testFunction(testAnswer, secondTry));
        assert(testFunction(testAnswer, thirdTry));
        std::cout << "Second test passed! 3 asserts passed." << std::endl;
    }

    return 0;
}