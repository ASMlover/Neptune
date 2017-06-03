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
#include <functional>
#include <string>
#include <Chaos/Logging/Logging.h>
#include <Neptune/Buffer.h>
#include <Neptune/TcpConnection.h>
#include "echo.h"

EchoServer::EchoServer(
    Neptune::EventLoop* loop, const Neptune::InetAddress& listen_addr)
  : server_(loop, listen_addr, "EchoServer") {
  server_.bind_connection_functor(
      std::bind(&EchoServer::on_connection, this, std::placeholders::_1));
  server_.bind_message_functor(std::bind(&EchoServer::on_message, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void EchoServer::start(void) {
  server_.start();
}

void EchoServer::on_connection(const Neptune::TcpConnectionPtr& conn) {
  CHAOSLOG_INFO
    << "EchoServer - " << conn->get_peer_address().get_host_port()
    << " -> " << conn->get_local_address().get_host_port()
    << " is " << (conn->is_connected() ? "UP" : "DOWN");
}

void EchoServer::on_message(
    const Neptune::TcpConnectionPtr& conn,
    Neptune::Buffer* buf, Chaos::Timestamp time) {
  std::string msg(buf->retrieve_all_to_string());
  CHAOSLOG_INFO << conn->get_name() << " echo " << msg.size() << " bytes, "
    << "data received at " << time.to_string();
  conn->write(msg);
}
