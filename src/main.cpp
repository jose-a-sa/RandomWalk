#include <fstream>
#include <iostream>
#include <numeric>

#include "RandomWalk.hpp"

std::vector<double> sample_stats(std::vector<std::size_t> x)
{
    std::size_t n = x.size();

    std::vector<double> res;

    double mean = std::accumulate(x.begin(), x.end(), 0.0L) / (1.0L * n);
    res.push_back(mean);

    double variance = std::accumulate(x.begin(), x.end(), - mean * mean * n, [&](double acc, double x)
                                      { return std::move(acc) + x * x; });
    variance /= (n - 1.0L);
    res.push_back(variance);

    return res;
}

int main()
{
    RandomWalk<2> walker;
    auto boundary1 = [](const Point<int, 2> &pt) -> bool
    {
        return std::abs(pt[0]) >= 2 || std::abs(pt[1]) >= 2;
    };

    std::vector<std::size_t> times = walker.walkWhileSample(100000, boundary1);
    std::vector<double> cm = sample_stats(times);
    std::cout << cm[0] << " ± " << sqrt(cm[1]) << std::endl;

    return 0;
}