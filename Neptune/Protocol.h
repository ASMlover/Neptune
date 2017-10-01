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
#ifndef NEPTUNE_PROTOCOL_H
#define NEPTUNE_PROTOCOL_H

#include <Neptune/Kern/NetOps.h>

namespace Neptune { namespace Protocol {

class Tcp {
  sa_family_t family_{};

  explicit Tcp(sa_family_t family)
    : family_(family) {
  }
public:
  static Tcp v4(void) {
    return Tcp(AF_INET);
  }

  static Tcp v6(void) {
    return Tcp(AF_INET6);
  }

  static Tcp get_protocol(sa_family_t family) {
    return Tcp(family);
  }

  sa_family_t family(void) const {
    return family_;
  }

  int socket_type(void) const {
    return SOCK_STREAM;
  }

  int protocol(void) const {
    return IPPROTO_TCP;
  }

  friend bool operator==(const Tcp& x, const Tcp& y) {
    return x.family_ == y.family_;
  }

  friend bool operator!=(const Tcp& x, const Tcp& y) {
    return x.family_ != y.family_;
  }
};

class Udp {
  sa_family_t family_{};

  explicit Udp(sa_family_t family)
    : family_(family) {
  }
public:
  static Udp v4(void) {
    return Udp(AF_INET);
  }

  static Udp v6(void) {
    return Udp(AF_INET6);
  }

  static Udp get_protocol(sa_family_t family) {
    return Udp(family);
  }

  sa_family_t family(void) const {
    return family_;
  }

  int socket_type(void) const {
    return SOCK_DGRAM;
  }

  int protocol(void) const {
    return IPPROTO_UDP;
  }

  friend bool operator==(const Udp& x, const Udp& y) {
    return x.family_ == y.family_;
  }

  friend bool operator!=(const Udp& x, const Udp& y) {
    return x.family_ != y.family_;
  }
};

}}

#endif // NEPTUNE_PROTOCOL_H
