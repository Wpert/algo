#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

template<typename T>
concept Multiplier = requires (T number) {
    number * number;
    number / number;
};

// power > 0; unless we can divide
template<Multiplier T>
T binaryPower(T mult, size_t power) {
    if (power == 0)
        return mult / mult;
    if (power == 1)
        return mult;
    
    if (power % 2 == 0)
        return binaryPower<T>(mult * mult, power / 2);
    else
        return binaryPower<T>(mult, power - 1) * mult;
}


int main() {
    std::cout << binaryPower<int>(2, 10) << std::endl;
    return 0;
}