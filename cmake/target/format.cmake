#
# Copyright (C) 2020 Assured Information Security, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

if(ENABLE_CLANG_FORMAT)
    add_custom_target(
        format
    )

    set(FORMAT_FILE_EXTENTIONS *.hpp         *.cpp         *.h         *.c        )
    set(FORMAT_FILE_TYPES      "C++ Headers" "C++ Sources" "C Headers" "C Sources")
    set(FORMAT_FILE_INDEX_STOP 3)
    set(FORMAT_FILE_LIST_PATH "${CMAKE_BINARY_DIR}/clang-format-files.txt")
    set(FORMAT_PRINT "${CMAKE_BINARY_DIR}/clang-format-print.txt")

    file(TOUCH ${FORMAT_FILE_LIST_PATH})

    # Find all files of interest using git ls-files for tracked, untracked and
    # cached files but excluding git ignored files and submodules.
    # Root level hidden directories (i.e. dot folders) are also ignored.
    add_custom_command(TARGET format
        # git ls-files
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} git ls-files --cached --others
            --exclude-standard -- ${FORMAT_FILE_EXTENTIONS} > ${FORMAT_FILE_LIST_PATH}
        # remove hidden directories
        COMMAND sed -i '/^\\./d' ${FORMAT_FILE_LIST_PATH}
        # create printable output
        COMMAND ${CMAKE_COMMAND} -E copy ${FORMAT_FILE_LIST_PATH} ${FORMAT_PRINT}
        # only keep directory name and file extension
        COMMAND sed -i 's/\\/.*\\./ /' ${FORMAT_PRINT}
        COMMAND sort -u -o ${FORMAT_PRINT} ${FORMAT_PRINT}
        COMMAND sed -i 's/^/ - formatting in /' ${FORMAT_PRINT}
        COMMAND cat ${FORMAT_PRINT}
    )

    # Do clang format
    add_custom_command(TARGET format
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cat ${FORMAT_FILE_LIST_PATH} |
            ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} xargs -d '\\n' ${BF_CLANG_FORMAT} -i
    )

    add_custom_command(TARGET format
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "done"
    )
endif()
