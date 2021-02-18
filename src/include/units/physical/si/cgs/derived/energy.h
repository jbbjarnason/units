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

#pragma once

#include <units/physical/dimensions/energy.h>
#include <units/physical/si/cgs/derived/force.h>
#include <units/physical/si/prefixes.h>
#include <units/quantity.h>

namespace units::physical::si::cgs {

struct erg : named_unit<erg, "erg", si::prefix> {};

struct dim_energy : physical::dim_energy<dim_energy, erg, dim_force, dim_length> {};

template<UnitOf<dim_energy> U, QuantityValue Rep = double>
using energy = quantity<dim_energy, U, Rep>;

inline namespace literals {

// erg
constexpr auto operator"" _q_erg(unsigned long long l) { gsl_ExpectsAudit(std::in_range<std::int64_t>(l)); return energy<erg, std::int64_t>(static_cast<std::int64_t>(l)); }
constexpr auto operator"" _q_erg(long double l) { return energy<erg, long double>(l); }

}  // namespace literals

namespace unit_constants {

inline constexpr auto erg = energy<cgs::erg, one_rep>{};

}  // namespace unit_constants

}  // namespace units::physical::si::cgs
