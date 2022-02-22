#pragma once

#include <iostream>

// This is neccessary so we can check that a T at compile-time can be passed into std::to_string.

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
inline constexpr bool can_to_string_v = can_to_string<T>::value;

// This is neccessary so we can check that T at compile-time has the member function .to_string()

template <typename C>
struct has_to_string                                                                                                
{                                                                                                                                         
private:                                                                                                                                  
    template <typename T>                                                                                     
    static constexpr auto check(T *) -> typename std::is_same<decltype(std::declval<T>().to_string()), std::string>::type; 
    template <typename>
    static constexpr std::false_type check(...);                                                                                          
    typedef decltype(check<C>(0)) type;                                                                                                   
                                                                                                                                              
public:                                                                                                                                   
    static constexpr bool value = type::value;                                                                                            
};

template <typename T>
inline constexpr bool has_to_string_v = has_to_string<T>::value; 