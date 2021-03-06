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
#pragma once

#include <Chaos/Base/UnCopyable.h>
#include <Neptune/TcpClient.h>
#include <Neptune/TcpServer.h>

class EchoClient : private Chaos::UnCopyable {
  Neptune::TcpClient client_;

  void on_connection(const Neptune::TcpConnectionPtr& conn);
  void on_message(const Neptune::TcpConnectionPtr& conn,
      Neptune::Buffer* buf, Chaos::Timestamp time);
public:
  EchoClient(Neptune::EventLoop* loop, const Neptune::InetAddress& server_addr);
  void connect(void);
};

class EchoServer : private Chaos::UnCopyable {
  Neptune::TcpServer server_;

  void on_connection(const Neptune::TcpConnectionPtr& conn);
  void on_message(const Neptune::TcpConnectionPtr& conn,
      Neptune::Buffer* buf, Chaos::Timestamp time);
public:
  EchoServer(Neptune::EventLoop* loop, const Neptune::InetAddress& listen_addr);
  void start(void);
};
