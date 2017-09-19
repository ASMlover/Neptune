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
#include <cstring>
#include <iostream>
#include <Chaos/Base/Types.h>
#include <Neptune/InetAddress.h>
#include <Neptune/EventLoop.h>
#include "echo.h"

int main(int argc, char* argv[]) {
  CHAOS_UNUSED(argc), CHAOS_UNUSED(argv);

  auto help_fn = [](void) {
    std::cout
      << "USAGE: echo [option]\n\n"
      << "\ts - start echo server\n"
      << "\tc - start echo client" << std::endl;
  };

  if (argc < 2) {
    help_fn();
    return 0;
  }

  if (std::strcmp(argv[1], "s") == 0) {
    Neptune::EventLoop loop;
    Neptune::InetAddress listen_addr(5555);
    EchoServer server(&loop, listen_addr);
    server.start();
    loop.loop();
  }
  else if (std::strcmp(argv[1], "c") == 0) {
    Neptune::EventLoop loop;
    Neptune::InetAddress server_addr("127.0.0.1", 5555);
    EchoClient client(&loop, server_addr);
    client.connect();
    loop.loop();
  }
  else {
    help_fn();
  }

  return 0;
}
