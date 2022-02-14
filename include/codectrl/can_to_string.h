#pragma once

#include <iostream>

// This is neccessary so we can check that a T at compile-time can be passed
// into std::to_string.

template <typename T>
struct can_to_string : std::false_type {};
template <>
struct can_to_string<int> : std::true_type {};
template <>
struct can_to_string<long> : std::true_type {};
template <>
struct can_to_string<long long> : std::true_type {};
template <>
struct can_to_string<unsigned> : std::true_type {};
template <>
struct can_to_string<unsigned long> : std::true_type {};
template <>
struct can_to_string<unsigned long long> : std::true_type {};
template <>
struct can_to_string<float> : std::true_type {};
template <>
struct can_to_string<double> : std::true_type {};
template <>
struct can_to_string<long double> : std::true_type {};

template <typename T>
constexpr inline bool can_to_string_v = can_to_string<T>::value;
