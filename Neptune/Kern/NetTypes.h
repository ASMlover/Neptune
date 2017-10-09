// Copyright (c) 2017 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef NEPTUNE_KERN_NETTYPES_H
#define NEPTUNE_KERN_NETTYPES_H

#include <cstdint>
#include <Chaos/Base/Platform.h>

#if defined(CHAOS_WINDOWS)
# if defined(CHAOS_ARCH64)
    using socket_t = std::uint64_t;
    using ssize_t = std::int64_t;
# else
    using socket_t = std::uint32_t;
    using ssize_t = long;
# endif
  using socklen_t = int;
  using sa_family_t = int;
  using in_addr_t = std::uint32_t;

  struct __WSABUF;
  struct pollfd;
#else
# include <sys/types.h>
# include <sys/socket.h>
  using socket_t = int;

  struct iovec;
  struct pollfd;
#endif
struct sockaddr;
struct sockaddr_in;
struct sockaddr_in6;

namespace Neptune { namespace NetOps {

static constexpr socket_t kInvalidSocket = (socket_t)(~0);
static constexpr int kSocketError = -1;

#if defined(CHAOS_WINDOWS)
  using Iovec_t = struct __WSABUF;
  using Pollfd_t = struct pollfd;
#else
  using Iovec_t = struct iovec;
  using Pollfd_t = struct pollfd;
#endif

}}

#endif // NEPTUNE_KERN_NETTYPES_H
