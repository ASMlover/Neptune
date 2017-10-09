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
#ifndef NEPTUNE_KERN_NETOPS_H
#define NEPTUNE_KERN_NETOPS_H

#include <string>
#include <Neptune/Kern/NetTypes.h>

namespace Neptune { namespace NetOps {

namespace socket {
  static constexpr int SHUT_READ = 0;
  static constexpr int SHUT_WRIT = 1;
  static constexpr int SHUT_BOTH = 2;

  // socket operations wrapper
  socket_t open(sa_family_t family, int socket_type, int protocol);
  socket_t open_tcp(sa_family_t family);
  socket_t open_udp(sa_family_t family);
  int shutdown(socket_t sockfd, int how);
  int close(socket_t sockfd);
  int bind(socket_t sockfd, const struct sockaddr* addr);
  int listen(socket_t sockfd);
  socket_t accept(socket_t sockfd, struct sockaddr_in* addr4);
  socket_t accept(socket_t sockfd, struct sockaddr_in6* addr6);
  int connect(socket_t sockfd, const struct sockaddr* addr);
  ssize_t read(socket_t sockfd, std::size_t len, void* buf);
  ssize_t write(socket_t sockfd, const void* buf, std::size_t len);
  void set_iovec(Iovec_t& vec, char* buf, std::size_t len);
  ssize_t readv(socket_t sockfd, int niov, Iovec_t* iov);
  void set_non_blocking(socket_t sockfd, bool mode = true);
  int set_option(socket_t sockfd, int level, int optname, int optval);
  int get_option(socket_t sockfd,
      int level, int optname, int* optval, socklen_t* optlen);
  int set_option(socket_t sockfd,
      int level, int optname, const void* optval, socklen_t optlen);
  int get_option(socket_t sockfd,
      int level, int optname, void* optval, socklen_t* optlen);
  int get_errno(socket_t sockfd);
  struct sockaddr_in6 get_local(socket_t sockfd);
  struct sockaddr_in6 get_peer(socket_t sockfd);
  bool is_self_connect(socket_t sockfd);
}

namespace addr {
  std::string to_string(const struct sockaddr* addr, bool is_only);
  std::string to_string(const struct sockaddr* addr);
  void get_address(
      const char* ip, std::uint16_t port, struct sockaddr_in* addr);
  void get_address(
      const char* ip, std::uint16_t port, struct sockaddr_in6* addr);
  struct sockaddr* cast(struct sockaddr_in* addr);
  const struct sockaddr* cast(const struct sockaddr_in* addr);
  struct sockaddr* cast(struct sockaddr_in6* addr);
  const struct sockaddr* cast(const struct sockaddr_in6* addr);
  const struct sockaddr_in* to_v4(const struct sockaddr* addr);
  const struct sockaddr_in6* to_v6(const struct sockaddr* addr);
}

int poll(Pollfd_t fds[], std::uint32_t nfds, int timeout);

}}

#endif // NEPTUNE_KERN_NETOPS_H
