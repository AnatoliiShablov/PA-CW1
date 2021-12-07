#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

#include "tbb/tbb.h"

enum class SortType {
  SSort,
  QSort,
  PQSort,
};

template <class It>
std::pair<It, It> partitionLMH(It first, It last) {
  thread_local static std::mt19937_64 engine{std::random_device{}()};
  std::iter_swap(first,
                 first + std::uniform_int_distribution<size_t>{
                             0, static_cast<size_t>(last - first - 1)}(engine));

  It middle1 = std::partition(
      first + 1, last,
      [&pivot = *first](auto const& elem) { return elem < pivot; });

  std::iter_swap(--middle1, first);

  It middle2 = std::partition(
      middle1, last,
      [&pivot = *first](auto const& elem) { return elem == pivot; });
  return {middle1, middle2};
}

template <SortType type, typename It>
void sort(It first, It last) {
  if constexpr (type == SortType::SSort) {
    std::sort(first, last);
  } else if constexpr (type == SortType::QSort) {
    if (first != last) {
      auto [m1, m2] = partitionLMH(first, last);
      sort<SortType::QSort>(first, m1);
      sort<SortType::QSort>(m2, last);
    }
  } else if constexpr (type == SortType::PQSort) {
    if (std::distance(first, last) < 2000) {
      sort<SortType::QSort>(first, last);
    } else {
      auto [m1, m2] = partitionLMH(first, last);
      tbb::parallel_invoke(
          [first, last = m1] { sort<SortType::PQSort>(first, last); },
          [first = m2, last] { sort<SortType::PQSort>(first, last); });
    }
  }
}

int main(int argc, char* argv[]) {
  size_t const amount = std::stoi(argv[1]);
  size_t const max = std::stoi(argv[2]);

  constexpr size_t Times = 5;

  std::unordered_map<SortType, std::array<uint64_t, Times> > tm;

  for (size_t time = 0; time < Times; ++time) {
    std::mt19937_64 engine{std::random_device{}()};

    std::vector<uint64_t> vec;
    vec.reserve(amount);
    for (size_t i = 0; i < amount; ++i) {
      vec.push_back(engine() % max);
    }

    std::vector<uint64_t> vec0{vec};
    std::vector<uint64_t> vec1{vec};
    std::vector<uint64_t> vec2{vec};

    {
      auto tpBegin = std::chrono::high_resolution_clock::now();

      sort<SortType::PQSort>(vec2.begin(), vec2.end());

      auto tpEnd = std::chrono::high_resolution_clock::now();

      tm[SortType::PQSort][time] =
          std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpBegin)
              .count();
    }

    {
      auto tpBegin = std::chrono::high_resolution_clock::now();

      sort<SortType::QSort>(vec1.begin(), vec1.end());

      auto tpEnd = std::chrono::high_resolution_clock::now();

      tm[SortType::QSort][time] =
          std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpBegin)
              .count();
    }

    {
      auto tpBegin = std::chrono::high_resolution_clock::now();

      sort<SortType::SSort>(vec0.begin(), vec0.end());

      auto tpEnd = std::chrono::high_resolution_clock::now();

      tm[SortType::SSort][time] =
          std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpBegin)
              .count();
    }

    if (vec0 != vec1 || vec1 != vec2) {
      std::abort();
    }
  }

  auto& tmSS = tm[SortType::SSort];
  auto& tmCQ = tm[SortType::QSort];
  auto& tmCPQ = tm[SortType::PQSort];

  std::cout << "SSort:\t| ";
  for (auto t : tmSS) {
    std::cout << t << "ms | ";
  }
  std::cout << "Avg: "
            << std::accumulate(tmSS.begin(), tmSS.end(), 0) / tmSS.size()
            << "ms\n";

  std::cout << "QSort:\t| ";
  for (auto t : tmCQ) {
    std::cout << t << "ms | ";
  }
  std::cout << "Avg: "
            << std::accumulate(tmCQ.begin(), tmCQ.end(), 0) / tmCQ.size()
            << "ms\n";

  std::cout << "PQSort:\t| ";
  for (auto t : tmCPQ) {
    std::cout << t << "ms | ";
  }
  std::cout << "Avg: "
            << std::accumulate(tmCPQ.begin(), tmCPQ.end(), 0) / tmCPQ.size()
            << "ms\n";

  return 0;
}
