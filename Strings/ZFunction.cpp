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
class Z_Function {
private:
    std::vector<size_t> z_;

public:
    Z_Function() = delete;
    Z_Function(T& str) : z_(str.size(), 0) {
        int n = str.size();
        z_[0] = n;

        int L, R, k;
        // [L,R] make a window which matches with prefix of s
        L = R = 0;
        
        for (int i = 1; i < n; ++i) {
            // if i > R nothing matches so we will calculate
            // Z[i] using naive way
            if (i > R) {
                L = R = i;
                while (R < n && str[R - L] == str[R])
                    ++R;
                
                z_[i] = R - L;
                --R;
            }
            else {
                // k = i-L so k corresponds to number which
                // matches in [L,R] interval.
                k = i - L;
    
                // if Z[k] is less than remaining interval
                // then Z[i] will be equal to Z[k].
                if (z_[k] < R-i+1)
                    z_[i] = z_[k];
                else {
                    // else start from R and check manually
                    L = i;
                    while (R < n && str[R - L] == str[R])
                        R++;
                    z_[i] = R - L;
                    R--;
                }
            }
        }
    }

    const std::vector<size_t>& GetZ() {
        return this->z_;
    }

    const size_t operator [] (int index) {
        return z_[index];
    }

    const size_t size() const noexcept {
        return this->z_.size();
    }

    auto data() { return this->pref_.data(); }
    auto begin() { return this->pref_.begin(); }
    auto end() { return this->pref_.end(); }
};

// compare two arrays by elments i think
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
    // std::string input;
    // std::cin >> input;

    // Z_Function<std::string> toTest(input);

    // for (int i = 0; i < toTest.size(); ++i) {
    //     std:: cout << toTest[i] << ", ";
    // }

    {
        std::string testString("abracadabra");
        Z_Function<std::string> toTest(testString);

        std::vector<size_t> testAnswer{11, 0, 0, 1, 0, 1, 0, 4, 0, 0, 1};
        assert(testFunction(testAnswer, toTest));
    }

    return 0;
}