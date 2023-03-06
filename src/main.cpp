#include <fstream>
#include <iostream>
#include <numeric>

#include "RandomWalk.hpp"

std::vector<double> sample_stats(std::vector<std::size_t> x)
{
    std::size_t n = x.size();

    std::vector<double> res(3, 0);

    double mean = std::accumulate(x.begin(), x.end(), 0.0) / static_cast<double>(n);
    res[1] = mean;

    double variance = std::accumulate(x.begin(), x.end(), -mean * mean * static_cast<double>(n),
                                      [&](double acc, double x)
                                      { return acc + x * x; });
    variance /= (static_cast<double>(n) - 1.0);
    res[2] = variance;

    return res;
}

int main()
{
    RandomWalk<2> walker;
    auto boundary1 = [](const Point<int, 2>& pt) -> bool
    {
        return std::abs(pt[0]) >= 2 || std::abs(pt[1]) >= 2;
    };

    std::vector<std::size_t> times = walker.walkWhileSample(100000, boundary1);
    std::vector<double> cm = sample_stats(times);
    std::cout << cm[1] << " ± " << sqrt(cm[2]) << std::endl;

    return 0;
}