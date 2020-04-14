/// @copyright
/// Copyright (C) 2020 Assured Information Security, Inc.
///
/// @copyright
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// @copyright
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// @copyright
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <bsl/array.hpp>
#include <bsl/debug.hpp>

namespace bsl
{
    /// <!-- description -->
    ///   @brief Provides the example's main function
    ///
    inline void
    example_span_not_equals() noexcept
    {
        constexpr bsl::uintmax size{6U};
        constexpr bsl::uintmax val1{4U};
        constexpr bsl::uintmax val2{8U};
        constexpr bsl::uintmax val3{15U};
        constexpr bsl::uintmax val4{16U};
        constexpr bsl::uintmax val5{23U};
        constexpr bsl::uintmax val6{42U};

        bsl::array<bsl::uintmax, size> const arr1{val1, val2, val3, val4, val5, val6};
        bsl::array<bsl::uintmax, size> const arr2{val1, val2, val3, val4, val5, val6};
        bsl::array<bsl::uintmax, size> const arr3{val1, val1, val1, val1, val1, val1};

        bsl::span const spn1{arr1.data(), arr1.size()};
        bsl::span const spn2{arr2.data(), arr2.size()};
        bsl::span const spn3{arr3.data(), arr3.size()};

        if (spn1 == spn2) {
            bsl::print() << "success\n";
        }

        if (spn1 != spn3) {
            bsl::print() << "success\n";
        }
    }
}
