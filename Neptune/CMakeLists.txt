# Copyright (c) 2017 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set(NEPTUNELIB_NAME "neptune")

file(GLOB NEPTUNE_SOURCES *.cc Kern/*.cc)
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  file(GLOB_RECURSE NEPTUNE_WINDOWS_SOURCES
    Kern/Windows/*.cc
    Windows/*.cc
    )
  set(NEPTUNE_SOURCES ${NEPTUNE_SOURCES} ${NEPTUNE_WINDOWS_SOURCES})
else()
  file(GLOB_RECURSE NEPTUNE_POSIX_SOURCES Kern/Posix/*.cc)
  set(NEPTUNE_SOURCES ${NEPTUNE_SOURCES} ${NEPTUNE_POSIX_SOURCES})
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    file(GLOB_RECURSE NEPTUNE_LINUX_SOURCES Linux/*.cc)
    set(NEPTUNE_SOURCES ${NEPTUNE_SOURCES} ${NEPTUNE_LINUX_SOURCES})
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    file(GLOB_RECURSE NEPTUNE_DARWIN_SOURCES Darwin/*.cc)
    set(NEPTUNE_SOURCES ${NEPTUNE_SOURCES} ${NEPTUNE_DARWIN_SOURCES})
  endif()
endif()
message(STATUS "`NeptunePROJ` NEPTUNE_SOURCES: ${NEPTUNE_SOURCES}")

add_library(${NEPTUNELIB_NAME} STATIC ${NEPTUNE_SOURCES})
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(${NEPTUNELIB_NAME} ws2_32.lib)
endif()

if (NEPTUNE_LINK_CHAOS)
  target_link_libraries(${NEPTUNELIB_NAME} chaos)
endif()
