#include <cstdint>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include "vecset.h"

std::random_device rd;  // a seed source for the random number engine
std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

int64_t cvm1(std::vector<int> &xs, double eps, double delta) {
  uint64_t thresh = 12.0 / (eps * eps) * ::log2(8.0 * std::size(xs) / delta);
  double p = 1;
  std::bernoulli_distribution bd(1.0);
  std::bernoulli_distribution bd2(0.5);
  std::unordered_set<int> X;
  X.reserve(thresh * 3);
  for (const auto x : xs) {
    if (bd(gen)) {
      X.insert(x);
    } else {
      X.erase(x);
    }
    if (X.size() == thresh) {
      std::unordered_set<int> Y;
      Y.reserve(thresh * 3);
      for (const auto xx : X) {
        if (bd2(gen)) {
          Y.insert(xx);
        }
      }
      if (std::size(Y) == thresh) {
        return -1l;
      }
      X = std::move(Y);
      bd.param(std::bernoulli_distribution::param_type(p /= 2.0));
    }
  }
  return std::size(X) / p;
}

int main() {
  int64_t nelts = 72344;
  size_t iters = 1000;
  std::vector<int> xs(nelts * 20);
  for (size_t i = 0; i < std::size(xs); ++i) {
    xs[i] = i % nelts;
  }
  for (size_t i = 0; i < iters; ++i) {
    std::cout << cvm1(xs, 0.1, 0.1) << std::endl;
  }
}