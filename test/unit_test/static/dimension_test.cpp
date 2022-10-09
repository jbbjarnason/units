// The MIT License (MIT)
//
// Copyright (c) 2018 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <units/dimension.h>

using namespace units;

namespace {

template<auto V, typename T>
inline constexpr bool is_of_type = std::is_same_v<std::remove_cvref_t<decltype(V)>, T>;

using one_dim_ = struct one_dim;

// clang-format off
inline constexpr struct length_dim_ : base_dimension<"L"> {} length_dim;
inline constexpr struct time_dim_ : base_dimension<"T"> {} time_dim;

inline constexpr struct frequency_dim_ : decltype(1 / time_dim) {} frequency_dim;
inline constexpr struct action_dim_ : decltype(1 / time_dim) {} action_dim;
inline constexpr struct area_dim_ : decltype(length_dim * length_dim) {} area_dim;
inline constexpr struct volume_dim_ : decltype(area_dim * length_dim) {} volume_dim;
inline constexpr struct speed_dim_ : decltype(length_dim / time_dim) {} speed_dim;
inline constexpr struct velocity_dim_ : speed_dim_ {} velocity_dim;
inline constexpr struct acceleration_dim_ : decltype(speed_dim / time_dim) {} acceleration_dim;
// clang-format on

// concepts verification
static_assert(BaseDimension<length_dim_>);
static_assert(!BaseDimension<frequency_dim_>);
static_assert(!DerivedDimension<length_dim_>);
static_assert(DerivedDimension<frequency_dim_>);
static_assert(Dimension<length_dim_>);
static_assert(Dimension<frequency_dim_>);

static_assert(DerivedDimension<one_dim_>);
static_assert(DerivedDimension<decltype(length_dim / length_dim)>);  // one_dim
static_assert(BaseDimension<decltype(speed_dim * time_dim)>);        // length_dim

// derived dimension expression template syntax verification
static_assert(is_of_type<1 / time_dim, derived_dimension<one_dim_, per<time_dim_>>>);
static_assert(is_of_type<1 / (1 / time_dim), time_dim_>);

static_assert(is_of_type<one_dim * time_dim, time_dim_>);
static_assert(is_of_type<time_dim * one_dim, time_dim_>);
static_assert(is_of_type<one_dim * (1 / time_dim), derived_dimension<one_dim_, per<time_dim_>>>);
static_assert(is_of_type<1 / time_dim * one_dim, derived_dimension<one_dim_, per<time_dim_>>>);

static_assert(is_of_type<length_dim * time_dim, derived_dimension<length_dim_, time_dim_>>);
static_assert(is_of_type<length_dim * length_dim, derived_dimension<units::power<length_dim_, 2>>>);

static_assert(
  is_of_type<length_dim * length_dim * time_dim, derived_dimension<units::power<length_dim_, 2>, time_dim_>>);
static_assert(
  is_of_type<length_dim * time_dim * length_dim, derived_dimension<units::power<length_dim_, 2>, time_dim_>>);

static_assert(
  is_of_type<length_dim*(time_dim* length_dim), derived_dimension<units::power<length_dim_, 2>, time_dim_>>);
static_assert(
  is_of_type<time_dim*(length_dim* length_dim), derived_dimension<units::power<length_dim_, 2>, time_dim_>>);

static_assert(is_of_type<1 / time_dim * length_dim, derived_dimension<length_dim_, per<time_dim_>>>);
static_assert(is_of_type<1 / time_dim * time_dim, one_dim_>);

static_assert(is_of_type<time_dim / one_dim, time_dim_>);
static_assert(is_of_type<1 / time_dim / one_dim, derived_dimension<one_dim_, per<time_dim_>>>);

static_assert(is_of_type<length_dim / time_dim * time_dim, length_dim_>);
static_assert(is_of_type<1 / time_dim * (1 / time_dim), derived_dimension<one_dim_, per<units::power<time_dim_, 2>>>>);
static_assert(is_of_type<1 / (time_dim * time_dim), derived_dimension<one_dim_, per<units::power<time_dim_, 2>>>>);
static_assert(is_of_type<1 / (1 / (time_dim * time_dim)), derived_dimension<units::power<time_dim_, 2>>>);

static_assert(
  is_of_type<length_dim / time_dim * (1 / time_dim), derived_dimension<length_dim_, per<units::power<time_dim_, 2>>>>);
static_assert(is_of_type<length_dim / time_dim*(length_dim / time_dim),
                         derived_dimension<units::power<length_dim_, 2>, per<units::power<time_dim_, 2>>>>);
static_assert(is_of_type<length_dim / time_dim*(time_dim / length_dim), one_dim_>);

static_assert(is_of_type<speed_dim / acceleration_dim, time_dim_>);
static_assert(is_of_type<acceleration_dim / speed_dim, derived_dimension<one_dim_, per<time_dim_>>>);
static_assert(
  is_of_type<speed_dim * speed_dim / length_dim, derived_dimension<length_dim_, per<units::power<time_dim_, 2>>>>);
static_assert(is_of_type<1 / (speed_dim * speed_dim) * length_dim,
                         derived_dimension<units::power<time_dim_, 2>, per<length_dim_>>>);

// comparisons of equivalent dimensions
static_assert(length_dim / length_dim == one_dim);

static_assert(1 / time_dim != frequency_dim);
static_assert(equivalent(1 / time_dim, frequency_dim));
static_assert(convertible(1 / time_dim, frequency_dim));
static_assert(1 / frequency_dim == time_dim);
static_assert(frequency_dim * time_dim == one_dim);
static_assert(std::is_same_v<std::common_type_t<decltype(1 / time_dim), decltype(frequency_dim)>, frequency_dim_>);
static_assert(std::is_same_v<std::common_type_t<decltype(frequency_dim), decltype(1 / time_dim)>, frequency_dim_>);

static_assert(length_dim * length_dim != area_dim);
static_assert(equivalent(length_dim * length_dim, area_dim));
static_assert(convertible(length_dim * length_dim, area_dim));
static_assert(length_dim * length_dim != volume_dim);
static_assert(area_dim / length_dim == length_dim);
static_assert(std::is_same_v<std::common_type_t<decltype(length_dim * length_dim), decltype(area_dim)>, area_dim_>);
static_assert(std::is_same_v<std::common_type_t<decltype(area_dim), decltype(length_dim * length_dim)>, area_dim_>);

static_assert(length_dim * length_dim * length_dim != volume_dim);
static_assert(equivalent(length_dim * length_dim * length_dim, volume_dim));
static_assert(area_dim * length_dim != volume_dim);
static_assert(equivalent(area_dim * length_dim, volume_dim));
static_assert(volume_dim / length_dim != area_dim);
static_assert(equivalent(volume_dim / length_dim, area_dim));
static_assert(volume_dim / length_dim / length_dim == length_dim);
static_assert(area_dim * area_dim / length_dim != volume_dim);
static_assert(equivalent(area_dim * area_dim / length_dim, volume_dim));
static_assert(area_dim * (area_dim / length_dim) != volume_dim);
static_assert(equivalent(area_dim * (area_dim / length_dim), volume_dim));
static_assert(volume_dim / (length_dim * length_dim) == length_dim);

static_assert(length_dim / time_dim != speed_dim);
static_assert(equivalent(length_dim / time_dim, speed_dim));
static_assert(length_dim * time_dim != speed_dim);
static_assert(length_dim / time_dim / time_dim != speed_dim);
static_assert(length_dim / speed_dim == time_dim);
static_assert(speed_dim * time_dim == length_dim);
static_assert(std::is_same_v<std::common_type_t<decltype(length_dim / time_dim), decltype(speed_dim)>, speed_dim_>);
static_assert(std::is_same_v<std::common_type_t<decltype(speed_dim), decltype(length_dim / time_dim)>, speed_dim_>);
static_assert(std::is_same_v<std::common_type_t<decltype(length_dim / time_dim), decltype(length_dim / time_dim)>,
                             decltype(length_dim / time_dim)>);

static_assert(length_dim / time_dim / time_dim != acceleration_dim);
static_assert(equivalent(length_dim / time_dim / time_dim, acceleration_dim));
static_assert(length_dim / (time_dim * time_dim) != acceleration_dim);
static_assert(equivalent(length_dim / (time_dim * time_dim), acceleration_dim));
static_assert(speed_dim / time_dim != acceleration_dim);
static_assert(equivalent(speed_dim / time_dim, acceleration_dim));
static_assert(speed_dim / acceleration_dim == time_dim);
static_assert(acceleration_dim * time_dim != speed_dim);
static_assert(equivalent(acceleration_dim * time_dim, speed_dim));
static_assert(acceleration_dim * (time_dim * time_dim) == length_dim);
static_assert(acceleration_dim / speed_dim != frequency_dim);
static_assert(equivalent(acceleration_dim / speed_dim, frequency_dim));

static_assert(frequency_dim != action_dim);
static_assert(equivalent(frequency_dim, action_dim));
static_assert(!convertible(frequency_dim, action_dim));
template<auto T1, auto T2>
concept no_common_type = requires {
                           requires !requires { typename std::common_type_t<decltype(T1), decltype(T2)>; };
                           requires !requires { typename std::common_type_t<decltype(T2), decltype(T1)>; };
                         };
static_assert(no_common_type<frequency_dim, action_dim>);

static_assert(velocity_dim != speed_dim);
static_assert(equivalent(velocity_dim, speed_dim));
static_assert(convertible(speed_dim, velocity_dim));
static_assert(std::is_same_v<std::common_type_t<decltype(velocity_dim), decltype(speed_dim)>, velocity_dim_>);
static_assert(std::is_same_v<std::common_type_t<decltype(speed_dim), decltype(velocity_dim)>, velocity_dim_>);

}  // namespace