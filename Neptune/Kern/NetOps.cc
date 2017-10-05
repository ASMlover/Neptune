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
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <Chaos/Base/Platform.h>
#if defined(CHAOS_WINDOWS)
# include <WS2tcpip.h>
# if !defined(_WINDOWS_)
#   include <WinSock2.h>
# endif
#endif
#include <Chaos/Base/Types.h>
#include <Chaos/Logging/Logging.h>
#include <Neptune/Endian.h>
#include <Neptune/Kern/NetOps.h>

namespace Neptune { namespace NetOps {

namespace socket {
  socket_t open(sa_family_t family, int socket_type, int protocol) {
    auto sockfd = ::socket(family, socket_type, protocol);
    if (sockfd == kInvalidSocket)
      CHAOSLOG_SYSFATAL << "NetOps::socket::open - call socket failed";
    return sockfd;
  }

  socket_t open_tcp(sa_family_t family) {
    auto sockfd = socket::open(family, SOCK_STREAM, IPPROTO_TCP);
    socket::set_non_blocking(sockfd, true);
    return sockfd;
  }

  socket_t open_udp(sa_family_t family) {
    auto sockfd = socket::open(family, SOCK_DGRAM, IPPROTO_UDP);
    socket::set_non_blocking(sockfd, true);

    return sockfd;
  }

  int shutdown(socket_t sockfd, int how) {
    if (::shutdown(sockfd, how) < 0) {
      CHAOSLOG_SYSERR
        << "NetOps::socket::shutdown - errno=" << get_errno(sockfd);
    }
    return 0;
  }

  int bind(socket_t sockfd, const struct sockaddr* addr) {
    socklen_t addrlen{};
    if (addr->sa_family == AF_INET6)
      addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in6));
    else
      addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    if (::bind(sockfd, addr, addrlen) < 0) {
      CHAOSLOG_SYSFATAL
        << "NetOps::socket::bind - errno=" << socket::get_errno(sockfd);
    }
    return 0;
  }

  int listen(socket_t sockfd) {
    if (::listen(sockfd, SOMAXCONN) < 0) {
      CHAOSLOG_SYSFATAL
        << "NetOps::socket::listen - errno=" << socket::get_errno(sockfd);
    }
    return 0;
  }

  socket_t accept(socket_t sockfd, struct sockaddr_in* addr4) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr4));
    socket_t connfd = ::accept(sockfd, addr::cast(addr4), &addrlen);
    if (connfd == kInvalidSocket) {
      int saved_errno = errno;
      CHAOSLOG_SYSERR
        << "NetOps::socket::accept - accept failed, errno=" << saved_errno;
      switch (saved_errno) {
      case EAGAIN:
      case EINTR:
      case EPERM:
      case EMFILE:
      case ECONNABORTED:
      case EPROTO:
        errno = saved_errno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOMEM:
      case ENOBUFS:
      case ENOTSOCK:
      case EOPNOTSUPP:
        CHAOSLOG_SYSFATAL
          << "NetOps::socket::accept - unexpected errno=" << saved_errno;
        break;
      default:
        CHAOSLOG_SYSFATAL
          << "NetOps::socket::accept - unknown errno=" << saved_errno;
        break;
      }
    }
    socket::set_non_blocking(connfd, true);

    return connfd;
  }

  socket_t accept(socket_t sockfd, struct sockaddr_in6* addr6) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr6));
    socket_t connfd = ::accept(sockfd, addr::cast(addr6), &addrlen);
    if (connfd == kInvalidSocket) {
      int saved_errno = errno;
      CHAOSLOG_SYSERR
        << "NetOps::socket::accept - accept failed, errno=" << saved_errno;
      switch (saved_errno) {
      case EAGAIN:
      case EINTR:
      case EPERM:
      case EMFILE:
      case ECONNABORTED:
      case EPROTO:
        errno = saved_errno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOMEM:
      case ENOBUFS:
      case ENOTSOCK:
      case EOPNOTSUPP:
        CHAOSLOG_SYSFATAL
          << "NetOps::socket::accept - unexpected errno=" << saved_errno;
        break;
      default:
        CHAOSLOG_SYSFATAL
          << "NetOps::socket::accept - unknown errno=" << saved_errno;
        break;
      }
    }
    socket::set_non_blocking(connfd, true);

    return connfd;
  }

  int connect(socket_t sockfd, const struct sockaddr* addr) {
    socklen_t addrlen{};
    if (addr->sa_family == AF_INET6)
      addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in6));
    else
      addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    return ::connect(sockfd, addr, addrlen);
  }

  int get_errno(socket_t sockfd) {
    int optval{};
    socklen_t optlen = sizeof(optval);
    if (!socket::get_option(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen))
      return errno;
    return optval;
  }

  struct sockaddr_in6 get_local(socket_t sockfd) {
    struct sockaddr_in6 addr{};
    socklen_t addrlen = static_cast<socklen_t>(sizeof(addr));
    if (::getsockname(sockfd, addr::cast(&addr), &addrlen) < 0)
      CHAOSLOG_SYSERR << "NetOps::socket::get_local - failed";
    return addr;
  }

  struct sockaddr_in6 get_peer(socket_t sockfd) {
    struct sockaddr_in6 addr{};
    socklen_t addrlen = static_cast<socklen_t>(sizeof(addr));
    if (::getpeername(sockfd, addr::cast(&addr), &addrlen) < 0)
      CHAOSLOG_SYSERR << "NetOps::socket::get_peer - failed";
    return addr;
  }

  bool is_self_connect(socket_t sockfd) {
    struct sockaddr_in6 local = socket::get_local(sockfd);
    struct sockaddr_in6 peer = socket::get_peer(sockfd);
    if (local.sin6_family == AF_INET) {
      const struct sockaddr_in* laddr4 =
        reinterpret_cast<struct sockaddr_in*>(&local);
      const struct sockaddr_in* raddr4 =
        reinterpret_cast<struct sockaddr_in*>(&peer);
      return laddr4->sin_port == raddr4->sin_port
        && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
    }
    else if (local.sin6_family == AF_INET6) {
      return local.sin6_port == peer.sin6_port
        && std::memcmp(&local.sin6_addr,
            &peer.sin6_addr, sizeof(local.sin6_addr)) == 0;
    }
    return false;
  }
}

namespace addr {
  std::string to_string(const struct sockaddr* addr) {
    std::string ip = to_string(addr, true);
    const struct sockaddr_in* addr4 = to_v4(addr);
    std::uint16_t port = Neptune::n2h16(addr4->sin_port);

    char buf[64]{};
    std::snprintf(buf, sizeof(buf), "%s:%u", ip.c_str(), port);
    return buf;
  }

  struct sockaddr* cast(struct sockaddr_in* addr) {
    return static_cast<struct sockaddr*>(Chaos::implicit_cast<void*>(addr));
  }

  const struct sockaddr* cast(const struct sockaddr_in* addr) {
    return static_cast<const struct sockaddr*>(
        Chaos::implicit_cast<const void*>(addr));
  }

  struct sockaddr* cast(struct sockaddr_in6* addr) {
    return static_cast<struct sockaddr*>(Chaos::implicit_cast<void*>(addr));
  }

  const struct sockaddr* cast(const struct sockaddr_in6* addr) {
    return static_cast<const struct sockaddr*>(
        Chaos::implicit_cast<const void*>(addr));
  }

  const struct sockaddr_in* to_v4(const struct sockaddr* addr) {
    return static_cast<const struct sockaddr_in*>(
        Chaos::implicit_cast<const void*>(addr));
  }

  const struct sockaddr_in6* to_v6(const struct sockaddr* addr) {
    return static_cast<const struct sockaddr_in6*>(
        Chaos::implicit_cast<const void*>(addr));
  }
}

}}
