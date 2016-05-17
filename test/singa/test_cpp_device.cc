/************************************************************
*
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership.  The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations
* under the License.
*
*************************************************************/

#include "gtest/gtest.h"
#include  "singa/core/device.h"
#include "singa/proto/core.pb.h"

using singa::CppDevice;
using singa::Blob;
TEST(CppDevice, Constructor) {
  CppDevice dev(0, 1);
  EXPECT_EQ(0, dev.id());
}

TEST(CppDevice, MemoryMallocFree) {
  CppDevice dev(0, 1);
  Blob* b = dev.NewBlob(4);
  EXPECT_NE(nullptr, b);
  EXPECT_EQ(4, b->size());
  dev.FreeBlob(b);
}

TEST(CppDevice, Exec) {
  CppDevice dev(0, 1);
  Blob* b = dev.NewBlob(4);
  int x = 1, y =3, z = 0;
  dev.Exec([x, y, &z](singa::Context *ctx) {
      z = x + y;
      }, {b}, {b}, false);
  EXPECT_EQ(x + y, z);
}

TEST(CppDevice, CopyData) {
  CppDevice dev(0, 1);
  Blob* b = dev.NewBlob(4);
  char s[] = {'a', 'b', 'c', 'x'};
  dev.CopyDataFromHostPtr(b, s, 4);
  const char* bstr = static_cast<const char*>(b->data());
  EXPECT_EQ('a', bstr[0]);
  EXPECT_EQ('b', bstr[1]);
  EXPECT_EQ('x', bstr[3]);

  Blob* c = dev.NewBlob(4);
  dev.CopyDataToFrom(c, b, 4, singa::kHostToHost, 0, 0);
  const char* cstr = static_cast<const char*>(c->data());

  EXPECT_EQ('a', cstr[0]);
  EXPECT_EQ('b', cstr[1]);
  EXPECT_EQ('x', cstr[3]);
  dev.FreeBlob(b);
  dev.FreeBlob(c);
}

