#pragma once
#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include "Point.hpp"

template <std::size_t D>
class RandomWalk
{
private:
    std::array<double, 2 * D + 1> cdf;
    Point<int, D> dx;
    std::mt19937 generator_;
    std::uniform_real_distribution<double> dist_;
    Point<Point<std::size_t, 2>, D> uniform_weights_();
    void init_rand_();

public:
    RandomWalk();
    RandomWalk(const Point<int, D> &dx);
    RandomWalk(const Point<Point<std::size_t, 2>, D> &weights, const Point<int, D> &dx);
    ~RandomWalk();
    Point<std::size_t, 2> random_direction();
    std::size_t walkWhile(std::function<bool(Point<int, D>)> boundary, std::size_t max_iter = -1);
    std::vector<size_t> walkWhileSample(std::size_t sample_size, std::function<bool(Point<int, D>)> boundary, std::size_t max_iter = -1);
};

template <std::size_t D>
inline Point<Point<std::size_t, 2>, D> RandomWalk<D>::uniform_weights_()
{
    Point<Point<std::size_t, 2>, D> res;
    res.fill({1, 1});
    return res;
}

template <std::size_t D>
inline void RandomWalk<D>::init_rand_()
{
    std::random_device rd;
    this->generator_ = std::mt19937(rd());
    this->dist_ = std::uniform_real_distribution<double>(0.0, 1.0);
}

template <std::size_t D>
inline RandomWalk<D>::RandomWalk() : RandomWalk<D>::RandomWalk(uniform_weights_(), Point<int, D>(1))
{
}

template <std::size_t D>
inline RandomWalk<D>::RandomWalk(const Point<int, D> &dx) : RandomWalk<D>::RandomWalk(uniform_weights_(), dx)
{
}

template <std::size_t D>
inline RandomWalk<D>::RandomWalk(const Point<Point<std::size_t, 2>, D> &weights, const Point<int, D> &dxx) : dx(move(dxx))
{
    init_rand_();

    std::size_t n = accumulate(weights.begin(), weights.end(), 0, [](const std::size_t &acc, const Point<std::size_t, 2> &w) -> std::size_t
                          { return std::move(acc) + w[0] + w[1]; });
    this->cdf = std::array<double, 2 * D + 1>();
    this->cdf.fill(0);
    for (size_t i = 0; i < 2 * D; i++)
    {
        size_t dim = i >> 1, dir = i % 2;
        size_t w = (dir == 0) ? weights[dim][0] : weights[dim][1];
        this->cdf[i + 1] = ((1.0L * w) / n) + this->cdf[i];
    }
}

template <std::size_t D>
inline RandomWalk<D>::~RandomWalk()
{
}

template <std::size_t D>
inline Point<std::size_t, 2> RandomWalk<D>::random_direction()
{
    double p = this->dist_(this->generator_);

    for (std::size_t i = 0; i < 2*D; i++)
    {
        if (this->cdf[i] <= p && p < this->cdf[i + 1])
            return {i >> 1, i % 2};
    }

    return {0, 0};
}

template <std::size_t D>
inline std::size_t RandomWalk<D>::walkWhile(std::function<bool(Point<int, D>)> boundary, std::size_t max_iter)
{
    if (max_iter == -1)
        max_iter = 100000;

    Point<int, D> pt(0);
    std::size_t time = 0;
    while (time < max_iter && !boundary(pt))
    {
        time++;
        Point<std::size_t, 2> dir = this->random_direction();
        int dim = dir[0];
        int sgn = dir[1] == 0 ? 1 : -1;
        pt[dim] += sgn * dx[dim];
    }

    return time;
}

template <std::size_t D>
inline std::vector<std::size_t> RandomWalk<D>::walkWhileSample(std::size_t sample_size, std::function<bool(Point<int, D>)> boundary, std::size_t max_iter)
{
    std::vector<std::size_t> times(sample_size, 0);

    for (int k = 0; k < sample_size; k++)
        times[k] = this->walkWhile(boundary, max_iter);

    return times;
}
