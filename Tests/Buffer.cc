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
#include <Chaos/Platform.h>
#include <Chaos/Unittest/TestHarness.h>
#include <Neptune/Buffer.h>

#if defined(CHAOS_DARWIN)
namespace Neptune {
  constexpr std::size_t Buffer::kCheapPrepend;
  constexpr std::size_t Buffer::kInitialBytes;
}
#endif

namespace nep = ::Neptune;

CHAOS_TEST(BufferAppendRetrieve, Chaos::FakeTester) {
  nep::Buffer buf;
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  const std::string s(200, 'x');
  buf.append(s);
  CHAOS_CHECK_EQ(buf.readable_bytes(), s.size());
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - s.size());
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  const std::string s2{buf.retrieve_to_string(50)};
  CHAOS_CHECK_EQ(s2.size(), 50u);
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
  CHAOS_CHECK_EQ(s3.size(), 350u);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
  CHAOS_CHECK_EQ(s3, std::string(350, 'x'));
}

CHAOS_TEST(BufferGrow, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append(std::string(400, 'y'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 400u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 400);

  buf.retrieve(50);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 350u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 400);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 50);

  buf.append(std::string(1000, 'z'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 1350u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 50);

  buf.retrieve_all();
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 1400u);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
}

CHAOS_TEST(BufferInsideGrow, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append(std::string(800, 'x'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 800u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 800);

  buf.retrieve(500);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 300u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 800);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 500);

  buf.append(std::string(300, 'y'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 600u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 600);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
}

CHAOS_TEST(BufferShrink, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append(std::string(2000, 'x'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 2000u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  buf.retrieve(1500);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 500u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend + 1500);

  buf.shrink(0);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 500u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 500);
  CHAOS_CHECK_EQ(buf.retrieve_all_to_string(), std::string(500, 'x'));
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);
}

CHAOS_TEST(BufferPrepend, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append(std::string(200, 'x'));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 200u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 200);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend);

  int x{1024};
  buf.prepend(&x, sizeof(x));
  CHAOS_CHECK_EQ(buf.readable_bytes(), 204u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes - 200);
  CHAOS_CHECK_EQ(buf.prependable_bytes(), nep::Buffer::kCheapPrepend - 4);
}

CHAOS_TEST(BufferReadInt, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append("NEPTUNE");

  CHAOS_CHECK_EQ(buf.readable_bytes(), 7u);
  CHAOS_CHECK_EQ(buf.peek_int8(), 'N');
  std::int16_t i16 = buf.peek_int16();
  CHAOS_CHECK_EQ(i16, 'N' * 256 + 'E');
  CHAOS_CHECK_EQ(buf.peek_int32(), i16 * 65536 + 'P' * 256 + 'T');

  CHAOS_CHECK_EQ(buf.read_int8(), 'N');
  CHAOS_CHECK_EQ(buf.read_int16(), 'E' * 256 + 'P');
  CHAOS_CHECK_EQ(buf.read_int8(), 'T');
  CHAOS_CHECK_EQ(buf.read_int8(), 'U');
  CHAOS_CHECK_EQ(buf.read_int16(), 'N' * 256 + 'E');
  CHAOS_CHECK_EQ(buf.readable_bytes(), 0u);
  CHAOS_CHECK_EQ(buf.writable_bytes(), nep::Buffer::kInitialBytes);

  buf.append_int8(-1);
  buf.append_int16(-2);
  buf.append_int32(-3);
  CHAOS_CHECK_EQ(buf.readable_bytes(), 7u);
  CHAOS_CHECK_EQ(buf.read_int8(), -1);
  CHAOS_CHECK_EQ(buf.read_int16(), -2);
  CHAOS_CHECK_EQ(buf.read_int32(), -3);
}

CHAOS_TEST(BufferEOL, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append(std::string(4096, 'x'));
  const char* null{};
  CHAOS_CHECK_EQ(buf.find_eol(), null);
  CHAOS_CHECK_EQ(buf.find_eol(buf.peek() + 4000), null);
}

CHAOS_TEST(BufferMove, Chaos::FakeTester) {
  nep::Buffer buf;
  buf.append("NEPTUNE", 7);
  const char* inner = buf.peek();
  [](const nep::Buffer& b, const void* inner) {
    CHAOS_CHECK_EQ(inner, static_cast<const void*>(b.peek()));
  }(buf, inner);
}
