#include <cmath>
#include <iostream>

#define ARR_LEN 100

int main() {
    double numbers[ARR_LEN];
    int count = 0;

    while (std::cin >> numbers[count]) {
        if (++count >= ARR_LEN) {
            break;
        }
    }

    if (count == 0) {
        return 0;
    }

    double max = numbers[0];
    double min = numbers[0];
    double sum = 0;

    for (int i = 0; i < count; ++i) {
        if (numbers[i] > max) {
            max = numbers[i];
        }

        if (numbers[i] < min) {
            min = numbers[i];
        }

        sum += numbers[i];
    }

    double mean = sum / count;
    double var = 0;

    for (int i = 0; i < count; ++i) {
        var += std::pow((numbers[i] - mean), 2);
    }

    double stddev = std::sqrt(var / count);

    std::cout << "max: " << max << "\n";
    std::cout << "min: " << min << "\n";
    std::cout << "mean: " << mean << "\n";
    std::cout << "stddev: " << stddev << "\n";

    return 0;
}