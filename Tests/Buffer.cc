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
#include <Chaos/Unittest/TestHarness.h>
#include <Neptune/Buffer.h>

CHAOS_TEST(BufferAppendRetrieve, Chaos::FakeTester) {
  namespace nep = ::Neptune;
  nep::Buffer buf;

  CHAOS_CHECK_EQ(buf.readable_bytes(), 0);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  const std::string s(200, 'x');
  buf.append(s);
  CHAOS_CHECK_EQ(buf.readable_bytes(), s.size());
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - s.size());
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  const std::string s2{buf.retrieve_to_string(50)};
  CHAOS_CHECK_EQ(s2.size(), 50);
  CHAOS_CHECK_EQ(buf.readable_bytes(), s.size() - s2.size());
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - s.size());
  CHAOS_CHECK_EQ(buf.prependable_bytes(),
      nep::Buffer::kCheapPrepend + s2.size());
  CHAOS_CHECK_EQ(s2, std::string(50, 'x'));

  buf.append(s);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 2 * s.size() - s2.size());
  CHAOS_CHECK_EQ(buf.writable_bytes(),
      nep::Buffer::kInitialBytes - 2 * s.size());
  CHAOS_CHECK_EQ(buf.prependable_bytes(),
      nep::Buffer::kCheapPrepend + s2.size());

  const std::string s3{buf.retrieve_all_to_string()};
  CHAOS_CHECK_EQ(s3.size(), 350);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
  CHAOS_CHECK_EQ(s3, std::string(350, 'x'));
}

CHAOS_TEST(BufferGrow, Chaos::FakeTester) {
  namespace nep = ::Neptune;

  nep::Buffer buf;
  buf.append(std::string(400, 'y'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 400);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 400);

  buf.retrieve(50);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 350);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 400);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 50);

  buf.append(std::string(1000, 'z'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 1350);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 0);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 50);

  buf.retrieve_all();
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 1400);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
}

CHAOS_TEST(BufferInsideGrow, Chaos::FakeTester) {
  namespace nep = ::Neptune;

  nep::Buffer buf;
  buf.append(std::string(800, 'x'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 800);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 800);

  buf.retrieve(500);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 300);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 800);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 500);

  buf.append(std::string(300, 'y'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 600);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 600);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
}
