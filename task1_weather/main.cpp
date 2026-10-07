#include <iomanip>
#include <iostream>

struct MinMax {
  int min;
  int max;
};

double Average(const int* temps, int n) {
  long long sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += temps[i];
  }
  return static_cast<double>(sum) / n;
}

MinMax FindMinMax(const int* temps, int n) {
  MinMax result = {temps[0], temps[0]};
  for (int i = 1; i < n; ++i) {
    if (temps[i] < result.min) {
      result.min = temps[i];
    }
    if (temps[i] > result.max) {
      result.max = temps[i];
    }
  }
  return result;
}

int CountFrostDays(const int* temps, int n) {
  int count = 0;
  for (int i = 0; i < n; ++i) {
    if (temps[i] < 0) {
      ++count;
    }
  }
  return count;
}

int main() {
  int n = 0;
  std::cin >> n;
  if (n <= 0) {
    return 1;
  }

  int* temps = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> temps[i];
  }

  MinMax extremes = FindMinMax(temps, n);

  std::cout << std::fixed << std::setprecision(1) << Average(temps, n) << '\n';
  std::cout << extremes.min << ' ' << extremes.max << '\n';
  std::cout << CountFrostDays(temps, n) << '\n';

  delete[] temps;
}
