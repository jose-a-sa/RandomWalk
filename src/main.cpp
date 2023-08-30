#include <fstream>
#include <iostream>
#include <numeric>

#include "RandomWalk.hpp"

std::vector<double> sample_stats(std::vector<int> x, std::function<bool(int)> outlier)
{
    std::vector<double> res(3, 0);

    std::size_t n = 0;
    int mom1 = 0, mom2 = 0;

    for(auto xi : x)
    {
        if(!outlier(xi))
        {
            n++;
            mom1 += xi;
            mom2 += xi*xi;
        }
    }
    
    res[1] = mom1 / static_cast<double>(n);
    res[2] = (mom2 - static_cast<double>(n)*res[1]*res[1] ) / (static_cast<double>(n) - 1.0);

    return res;
}

int main()
{
    RandomWalk<2> walker;
    auto boundary1 = [](const Point<int, 2>& pt) -> bool
    {
        return std::abs(pt[0]) >= 2 || std::abs(pt[1]) >= 2;
    };

    auto times = walker.walkWhileSample(100000, boundary1);
    std::vector<double> cm = sample_stats(times, [](int x) -> bool {return x < 0;});
    std::cout << cm[1] << " ± " << sqrt(cm[2]) << std::endl;

    return 0;
}