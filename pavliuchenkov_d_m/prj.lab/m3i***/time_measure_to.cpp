//
// Created by dmitrij on 3/26/22.
//

#include <chrono>
#include <iostream>
#include "m3i_by_t_o.h"
#include <vector>

typedef std::chrono::steady_clock Clock;

std::vector<double> test_time(int dim1 = 10, int dim2 = 10, int dim3 = 10, int iter = 10) {
    std::vector<double> times(iter, 0);
    for (int i=0; i < iter; ++i) {
        auto start = Clock::now();
        M3i give_me_a_name(dim1, dim2, dim3);
        auto end = Clock::now();
        auto duration = end - start;
        times[i] = duration.count() * 0.001;
    }
    return times;
}

int main() {

    {
        // 10x10x10 - 10 times
        int dim1 = 10, dim2 = 10, dim3 = 10;
        int iter = 10;
        std::vector<double> times;
        auto start = Clock::now();
        times = test_time(dim1, dim2, dim3, iter);
        auto end = Clock::now();
        auto duration = end - start;
        double mean_time = duration.count() * 0.001 / iter;
        double median = 0, dispersion = 0;
        for (int i=0; i < iter; ++i) {
            median += times[i];
            dispersion += times[i] * times[i];
        }
        median /= iter;
        dispersion /= iter;
        dispersion = dispersion - median * median;
        std::cout << "Size: " << dim1 << "x" << dim2 << "x" << dim3 << std::endl;
        std::cout << "Iterations: " << iter << std::endl;
        std::cout << "Mean time (ms): " << mean_time << std::endl;
        std::cout << "Median time (ms): " << median << std::endl;
        std::cout << "Dispersion (ms): " << dispersion << std::endl;
        std::cout << std::endl;
    }

    {
        // 100x100x100 - 100 times
        int dim1 = 100, dim2 = 100, dim3 = 100;
        int iter = 100;
        std::vector<double> times;
        auto start = Clock::now();
        times = test_time(dim1, dim2, dim3, iter);
        auto end = Clock::now();
        auto duration = end - start;
        double mean_time = duration.count() * 0.001 / iter;
        double median = 0, dispersion = 0;
        for (int i=0; i < iter; ++i) {
            median += times[i];
            dispersion += times[i] * times[i];
        }
        median /= iter;
        dispersion /= iter;
        dispersion = dispersion - median * median;
        std::cout << "Size: " << dim1 << "x" << dim2 << "x" << dim3 << std::endl;
        std::cout << "Iterations: " << iter << std::endl;
        std::cout << "Mean time (ms): " << mean_time << std::endl;
        std::cout << "Median time (ms): " << median << std::endl;
        std::cout << "Dispersion (ms): " << dispersion << std::endl;
        std::cout << std::endl;
    }

    {
        // 1000x1000x1000 - 1000 times
        int dim1 = 1000, dim2 = 1000, dim3 = 1000;
        int iter = 1000;
        std::vector<double> times;
        auto start = Clock::now();
        times = test_time(dim1, dim2, dim3, iter);
        auto end = Clock::now();
        auto duration = end - start;
        double mean_time = duration.count() * 0.001 / iter;
        double median = 0, dispersion = 0;
        for (int i=0; i < iter; ++i) {
            median += times[i];
            dispersion += times[i] * times[i];
        }
        median /= iter;
        dispersion /= iter;
        dispersion = dispersion - median * median;
        std::cout << "Size: " << dim1 << "x" << dim2 << "x" << dim3 << std::endl;
        std::cout << "Iterations: " << iter << std::endl;
        std::cout << "Mean time (ms): " << mean_time << std::endl;
        std::cout << "Median time (ms): " << median << std::endl;
        std::cout << "Dispersion (ms): " << dispersion << std::endl;
        std::cout << std::endl;
    }
}

/*
Size: 10x10x10
Iterations: 10
Mean time (ms): 7.322
Median time (ms): 4.8854
Dispersion (ms): 4.93627

Size: 100x100x100
Iterations: 100
Mean time (ms): 4628.07
Median time (ms): 3909.46
Dispersion (ms): 2.76356e+06

Size: 1000x1000x1000
Iterations: 1000
 ???
 (Started to worry about my notebook)
 */
