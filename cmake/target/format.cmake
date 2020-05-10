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

    # Find all *.h and *.cpp in git tracked files including untracked and cached files but
    # excluding git ignored files and submodules.
    execute_process( COMMAND
        git ls-files --cached --others --exclude-standard -- *.hpp *.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE SOURCES_RESULT
        OUTPUT_VARIABLE SOURCES
    )
    if (NOT SOURCES_RESULT EQUAL 0)
        message(FATAL_ERROR "command `git ls-files ...` returned code ${SOURCES_RESULT}")
    endif()

    if(NOT "${SOURCES}" STREQUAL "")
        string(REPLACE "\n" ";" SOURCES ${SOURCES})

        list(TRANSFORM SOURCES PREPEND "${CMAKE_SOURCE_DIR}/")
        # Transform prepend seems to add an extra element at the end
        list(POP_BACK SOURCES EXTRA)
        if (NOT "${EXTRA}" STREQUAL "${CMAKE_SOURCE_DIR}/")
            message(FATAL_ERROR "${EXTRA} != ${CMAKE_SOURCE_DIR}/") #
        endif()

        add_custom_command(TARGET format
            COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${BF_CLANG_FORMAT} --verbose -i ${SOURCES}
        )
    endif()

    add_custom_command(TARGET format
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "done"
    )
endif()
