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

#ifndef BSL_DETAILS_IFMAP_LINUX_HPP
#define BSL_DETAILS_IFMAP_LINUX_HPP

#include "../byte.hpp"
#include "../convert.hpp"
#include "../cstdint.hpp"
#include "../debug.hpp"
#include "../discard.hpp"
#include "../safe_integral.hpp"
#include "../span.hpp"
#include "../string_view.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace bsl
{
    /// @class bsl::ifmap
    ///
    /// <!-- description -->
    ///   @brief Maps a file as read-only, and returns a pointer to the file
    ///     via data() as well as the size of the mapped file via size().
    ///   @include example_ifmap_overview.hpp
    ///
    class ifmap final
    {
        /// @brief stores a handle to the mapped file.
        bsl::int32 m_file{};
        /// @brief stores a view of the file that is mapped.
        span<byte> m_data{};

    public:
        /// @brief alias for: void
        using value_type = void;
        /// @brief alias for: safe_uintmax
        using size_type = safe_uintmax;
        /// @brief alias for: safe_uintmax
        using difference_type = safe_uintmax;
        /// @brief alias for: void *
        using pointer_type = void *;
        /// @brief alias for: void const *
        using const_pointer_type = void const *;

        /// <!-- description -->
        ///   @brief Creates a bsl::ifmap given a the filename and path of
        ///     the file to map as read-only.
        ///   @include ifmap/example_ifmap_constructor.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param filename the filename and path of the file to map
        ///
        explicit ifmap(string_view const &filename) noexcept
        {
            using stat_t = struct stat;

            m_file = open(filename.data(), O_RDONLY);    // NOLINT
            if (m_file == -1) {
                bsl::alert() << "failed to open read-only file: "    // --
                             << filename                             // --
                             << bsl::endl;
                close(m_file);
                return;
            }

            stat_t s{};
            if (fstat(m_file, &s) == -1) {
                bsl::alert() << "failed to get the size of the read-only file: "    // --
                             << filename                                            // --
                             << bsl::endl;
                close(m_file);
            }

            pointer_type const ptr{mmap(
                nullptr,
                static_cast<bsl::uintmax>(s.st_size),
                PROT_READ,
                MAP_SHARED | MAP_POPULATE,    // NOLINT
                m_file,
                0)};

            if (ptr == MAP_FAILED) {                                // NOLINT
                bsl::alert() << "failed to map read-only file: "    // --
                             << filename                            // --
                             << bsl::endl;
                close(m_file);
            }

            m_data = as_writable_bytes(ptr, to_umax(s.st_size));
        }

        /// <!-- description -->
        ///   @brief Destructor unmaps a previously mapped file.
        ///
        ~ifmap() noexcept
        {
            munmap(m_data.data(), m_data.size().get());
            close(m_file);
        }

        /// <!-- description -->
        ///   @brief copy constructor
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being copied
        ///
        constexpr ifmap(ifmap const &o) noexcept = default;

        /// <!-- description -->
        ///   @brief move constructor
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being moved
        ///
        constexpr ifmap(ifmap &&o) noexcept = default;

        /// <!-- description -->
        ///   @brief copy assignment
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being copied
        ///   @return a reference to *this
        ///
        ifmap &operator=(ifmap const &o) &noexcept = default;

        /// <!-- description -->
        ///   @brief move assignment
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being moved
        ///   @return a reference to *this
        ///
        ifmap &operator=(ifmap &&o) &noexcept = default;

        /// <!-- description -->
        ///   @brief Returns a pointer to the read-only mapped file.
        ///   @include ifmap/example_ifmap_data.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns a pointer to the read-only mapped file.
        ///
        [[nodiscard]] constexpr const_pointer_type
        data() const noexcept
        {
            return m_data.data();
        }

        /// <!-- description -->
        ///   @brief Returns true if the file failed to be mapped, false
        ///     otherwise.
        ///   @include ifmap/example_ifmap_empty.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the file failed to be mapped, false
        ///     otherwise.
        ///
        [[nodiscard]] constexpr bool
        empty() const noexcept
        {
            return m_data.empty();
        }

        /// <!-- description -->
        ///   @brief Returns !empty()
        ///   @include ifmap/example_ifmap_operator_bool.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns !empty()
        ///
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return !this->empty();
        }

        /// <!-- description -->
        ///   @brief Returns the number of bytes in the file being
        ///     mapped.
        ///   @include ifmap/example_ifmap_size.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the number of bytes in the file being
        ///     mapped.
        ///
        [[nodiscard]] constexpr size_type const &
        size() const noexcept
        {
            return m_data.size();
        }

        /// <!-- description -->
        ///   @brief Returns the max number of bytes the BSL supports.
        ///   @include ifmap/example_ifmap_max_size.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the max number of bytes the BSL supports.
        ///
        [[nodiscard]] static constexpr size_type
        max_size() noexcept
        {
            return to_umax(size_type::max());
        }

        /// <!-- description -->
        ///   @brief Returns the number of bytes in the file being
        ///     mapped.
        ///   @include ifmap/example_ifmap_size_bytes.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the number of bytes in the file being
        ///     mapped.
        ///
        [[nodiscard]] constexpr size_type const &
        size_bytes() const noexcept
        {
            return m_data.size();
        }
    };
}

#endif
