#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__

#include <iostream>
#include <tuple>
#include "Lib/Part2/Common/CommonExceptions.h"

template<typename T>
class LightfieldCoordinate {
 protected:
  std::tuple<T, T, T, T> dimensions;

 public:
  static_assert(std::is_integral<T>::value,
      "The type of the lightfield coordinates must be integral.");


  LightfieldCoordinate(const T t, const T s, const T v, const T u)
      : dimensions({t, s, v, u}){};


  LightfieldCoordinate(const std::tuple<T, T, T, T>& dimensions)
      : dimensions(dimensions){};


  LightfieldCoordinate(const LightfieldCoordinate<T>& other)
      : dimensions(other.dimensions){};


  ~LightfieldCoordinate() = default;


  bool operator==(const LightfieldCoordinate<T>& other) const {
    return this->dimensions == other.dimensions;
  }


  bool operator!=(const LightfieldCoordinate<T>& other) const {
    return this->dimensions != other.dimensions;
  }


  LightfieldCoordinate<T> operator-(
      const LightfieldCoordinate<T>& other) const {
    if constexpr (std::is_unsigned<T>::value) {
      if ((this->get_t() < other.get_t()) || (this->get_s() < other.get_s()) ||
          (this->get_v() < other.get_v()) || (this->get_u() < other.get_u())) {
        throw LightfieldCoordinateExceptions::UnsignedUnderflowException();
      }
    }

    T t = this->get_t() - other.get_t();
    T s = this->get_s() - other.get_s();
    T v = this->get_v() - other.get_v();
    T u = this->get_u() - other.get_u();
    return LightfieldCoordinate<T>(t, s, v, u);
  }


  LightfieldCoordinate<T> operator+(
      const LightfieldCoordinate<T>& other) const {
    //checking for overflow
    auto max_val = std::numeric_limits<T>::max();
    auto max_other_t = max_val - this->get_t();
    auto max_other_s = max_val - this->get_s();
    auto max_other_v = max_val - this->get_v();
    auto max_other_u = max_val - this->get_u();
    if ((other.get_t() > max_other_t) || (other.get_s() > max_other_s) ||
        (other.get_v() > max_other_v) || (other.get_u() > max_other_u)) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }


    T t = this->get_t() + other.get_t();
    T s = this->get_s() + other.get_s();
    T v = this->get_v() + other.get_v();
    T u = this->get_u() + other.get_u();
    return LightfieldCoordinate<T>(t, s, v, u);
  }


  T get_t() const noexcept {
    return std::get<0>(dimensions);
  }


  T get_s() const noexcept {
    return std::get<1>(dimensions);
  }


  T get_v() const noexcept {
    return std::get<2>(dimensions);
  }


  T get_u() const noexcept {
    return std::get<3>(dimensions);
  }


  std::pair<T, T> get_t_and_s() const noexcept {
    return std::make_pair(get_t(), get_s());
  }


  std::pair<T, T> get_v_and_u() const noexcept {
    return std::make_pair(get_v(), get_u());
  }


  auto get() const noexcept {
    return dimensions;
  }


  template<std::size_t N>
  auto get() const {  //for std::get<N>(dimension) access
    if constexpr (N == 0)
      return get_t();
    else if constexpr (N == 1)
      return get_s();
    else if constexpr (N == 2)
      return get_v();
    else if constexpr (N == 3)
      return get_u();
  }


  void set_t(const T t) noexcept {
    std::get<0>(dimensions) = t;
  }


  void set_s(const T s) noexcept {
    std::get<1>(dimensions) = s;
  }


  void set_v(const T v) noexcept {
    std::get<2>(dimensions) = v;
  }


  void set_u(const T u) noexcept {
    std::get<3>(dimensions) = u;
  }


  void set(const std::tuple<T, T, T, T>& other) {
    this->dimensions = other;
  }


  void print() const {
    std::cout << get_t() << ", " << get_s() << ", " << get_v() << ", "
              << get_u() << std::endl;
  }
};


namespace std {
template<typename T>
struct tuple_size<LightfieldCoordinate<T>>
    : std::integral_constant<std::size_t, 4> {};

template<std::size_t I, typename T>
struct tuple_element<I, LightfieldCoordinate<T>> {
  using type = T;
};
}  // namespace std

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__ */