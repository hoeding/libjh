#ifndef LIBJH_ENUM_FLAG_HPP
#define LIBJH_ENUM_FLAG_HPP
#include <type_traits>

#define IS_FLAG_ENUM(enum_name)                                                \
  constexpr bool is_flag(enum_name) { return true; }

namespace river::flags {
template <typename T>
concept unsigned_enum =
    std::is_enum<T>::value && std::is_unsigned<std::underlying_type_t<T>>::value;

namespace details {
  template <unsigned_enum T> constexpr bool is_flag() { return is_flag(T{}); }
} // namespace details

template <typename T>
concept Flag = unsigned_enum<T> && details::is_flag<T>();

template <Flag T> constexpr auto underlying_value(T enum_value) {
  return static_cast<std::underlying_type_t<T>>(enum_value);
}

template <Flag T> constexpr auto operator~(T value) {
  return static_cast<std::underlying_type_t<T>>(~underlying_value(value));
}

template <Flag T> constexpr auto operator|(T lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) |
                                                underlying_value(rhs));
}
template <Flag T>
constexpr auto operator|(std::underlying_type_t<T> lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(lhs | underlying_value(rhs));
}
template <Flag T>
constexpr auto operator|(T lhs, std::underlying_type_t<T> rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) | rhs);
}
template <Flag T>
constexpr auto operator|=(std::underlying_type_t<T> &value, T const flag) {
  return value = value | flag;
}

template <Flag T> constexpr auto operator&(T lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) &
                                                underlying_value(rhs));
}
template <Flag T>
constexpr auto operator&(std::underlying_type_t<T> lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(lhs & underlying_value(rhs));
}
template <Flag T>
constexpr auto operator&(T lhs, std::underlying_type_t<T> rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) & rhs);
}
template <Flag T>
constexpr auto operator&=(std::underlying_type_t<T> &value, T const flag) {
  return value = value & flag;
}

template <Flag T> constexpr auto operator^(T lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) ^
                                                underlying_value(rhs));
}
template <Flag T>
constexpr auto operator^(std::underlying_type_t<T> lhs, T rhs) {
  return static_cast<std::underlying_type_t<T>>(lhs ^ underlying_value(rhs));
}
template <Flag T>
constexpr auto operator^(T lhs, std::underlying_type_t<T> rhs) {
  return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) ^ rhs);
}
template <Flag T>
constexpr auto operator^=(std::underlying_type_t<T> &value, T const flag) {
  return value = value ^ flag;
}

template <auto mask>
constexpr bool has(std::underlying_type_t<decltype(mask)> value) {
  return (value & mask) == underlying_value(mask);
}
} // namespace river::flags

#endif