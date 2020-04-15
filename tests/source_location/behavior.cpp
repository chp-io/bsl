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

#include <bsl/source_location.hpp>
#include <bsl/ut.hpp>

/// <!-- description -->
///   @brief Used to execute the actual checks. We put the checks in this
///     function so that we can validate the tests both at compile-time
///     and at run-time. If a bsl::ut_check fails, the tests will either
///     fail fast at run-time, or will produce a compile-time error.
///
/// <!-- inputs/outputs -->
///   @return Always returns bsl::exit_success.
///
constexpr bsl::exit_code
tests() noexcept
{
    bsl::ut_scenario{"empty source location"} = []() {
        bsl::ut_given{} = []() {
            bsl::source_location sloc{};
            bsl::ut_then{} = [&sloc]() {
                bsl::ut_check(sloc.file_name() != nullptr);
                bsl::ut_check(sloc.function_name() != nullptr);
                bsl::ut_check(sloc.line() == -1);
            };
        };
    };

    bsl::ut_scenario{"source location"} = []() {
        bsl::ut_given{} = []() {
            bsl::source_location sloc = bsl::here();
            bsl::ut_then{} = [&sloc]() {
                bsl::ut_check(sloc.file_name() != nullptr);
                bsl::ut_check(sloc.function_name() != nullptr);
                bsl::ut_check(sloc.line() == 53);    // NOLINT
            };
        };
    };

    bsl::ut_scenario{"output doesn't crash"} = []() {
        bsl::ut_given{} = []() {
            bsl::source_location sloc{};
            bsl::ut_then{} = [&sloc]() {
                bsl::debug() << sloc;
            };
        };

        bsl::ut_given{} = []() {
            bsl::source_location sloc = bsl::here();
            bsl::ut_then{} = [&sloc]() {
                bsl::debug() << sloc;
            };
        };
    };

    return bsl::ut_success();
}

/// <!-- description -->
///   @brief Main function for this unit test. If a call to ut_check() fails
///     the application will fast fail. If all calls to ut_check() pass, this
///     function will successfully return with bsl::exit_success.
///
/// <!-- inputs/outputs -->
///   @return Always returns bsl::exit_success.
///
bsl::exit_code
main() noexcept
{
    static_assert(tests() == bsl::ut_success());
    return tests();
}
