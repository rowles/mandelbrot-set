#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <stdint.h>
#include <string>

namespace mandelbrot {

using Complex = std::complex<double>;
const static uint8_t MAX_ITERS = 200;

// calculate iterations using std complex
inline uint8_t calculate_iters_complex(const uint8_t x, const uint8_t y) {
  Complex c{(double)x, (double)y};
  Complex z{0};

  auto iter = 0;

  while (std::abs(z) < 2.0 && iter < MAX_ITERS) {
    z = z * z * z + c;
    iter++;
  }

  return iter;
}

// calculate iterations using floating point
inline uint8_t calculate_iters_float(const uint8_t x, const uint8_t y) {
  float zx = 0.0f;
  float zy = 0.0f;

  auto zx_sqr = 0.0f;
  auto zy_sqr = 0.0f;

  auto iter = 0;

  while ((zx_sqr + zy_sqr) < 4 && iter < MAX_ITERS) {
    const auto zxzy = zx + zy;
    const auto zx0 = (zx_sqr) - (zy_sqr) + x;

    zy = (zxzy) * (zxzy)-zx_sqr - zy_sqr + y;
    zx = zx0;

    zx_sqr = zx * zx;
    zy_sqr = zy * zy;

    iter++;
  }

  return iter;
}

// calculate iterations using fixed point
// note: still not perfect
inline uint8_t calculate_iters_fixed(const uint8_t x, const uint8_t y) {
  uint32_t zx = 0;
  uint32_t zy = 0;

  uint32_t zx_sqr = 0;
  uint32_t zy_sqr = 0;

  auto iter = 0;

  while ((zx_sqr + zy_sqr) < (4 << 26) && iter < MAX_ITERS) {
    const uint32_t zxzy = zx + zy;

    zy = ((uint64_t)((uint64_t)zxzy * (uint64_t)zxzy) >> 26) - zx_sqr - zy_sqr +
         (y << 26);
    zx = zx_sqr - zy_sqr + (x << 26);

    zx_sqr = (uint64_t)((uint64_t)zx * (uint64_t)zx) >> 26;
    zy_sqr = (uint64_t)((uint64_t)zy * (uint64_t)zy) >> 26;

    iter++;
  }

  return iter;
}

template <std::size_t R, std::size_t C>
void fractal(std::array<std::array<uint8_t, R>, C> &src,
             std::function<uint8_t(uint8_t, uint8_t)> calc_iter) {

  for (uint8_t i = 0; i < src.size(); ++i) {
    for (uint8_t j = 0; j < src[i].size(); ++j) {
      src[i][j] = calc_iter(i, j);
    }
  }
}

template <std::size_t R, std::size_t C>
void print(const std::array<std::array<uint8_t, R>, C> &src) {
  for (const auto &x : src) {
    for (const auto &y : x) {
      std::cout << std::to_string(y) << std::endl;
    }
  }
}
} // namespace mandelbrot

int main() {
  using namespace mandelbrot;
  auto fn_vec = {calculate_iters_complex, calculate_iters_float,
                 calculate_iters_fixed};

  for (auto &fn : fn_vec) {
    std::array<std::array<uint8_t, 200>, 200> mat = {{}};
    auto started = std::chrono::high_resolution_clock::now();
    fractal(mat, fn);
    auto done = std::chrono::high_resolution_clock::now();
    auto delta_ms =
        std::chrono::duration_cast<std::chrono::microseconds>(done - started)
            .count();
    // print(mat);
    std::cout << "time (us): " << delta_ms << std::endl;
  }

  return 0;
}
