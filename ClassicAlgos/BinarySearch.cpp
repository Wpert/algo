#include <iostream>
#include <vector>
// unfortunatelly I dont even know how to write BS
// it is so simple, but templates are too much

// so I would like to solve few tasks to show ideas
// Задачи писать буду на русском, так как
// я устал думать на английском, мне за это не платят

int BinarySearch(std::vector<int>& arr, int num) {
    // lp -- left pointer
    // rp -- right pointer
    // mp -- middle pinter
    size_t lp = 0, rp = arr.size() - 1;
    while (rp - lp > 1) {
        size_t mp = (lp + rp) / 2;
        if (arr[mp] > num) {
            rp = mp;
        }
        else if (arr[mp] < num) {
            lp = mp;
        }
        else {
            return mp;
        }
    }

    // element not found
    return -1;
}

int main() {
// Задача: Дан отсортированный массив, найти индекс числа x
// если числа x в массиве нет, вывести -1
    std::vector<int> sortedArray{1, 2, 3, 6, 20, 22, 26, 30};

    for (int i = 20; i < 25; ++i) {
        std::cout << BinarySearch(sortedArray, i) << std::endl;
    }

// дальше можно было бы рассказать про lower/upper_bound
// про бинарный поиск по ответу / вещественный бинпоиск, но в другой раз

    return 0;
}