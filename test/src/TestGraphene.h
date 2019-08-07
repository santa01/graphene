/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TESTGRAPHENE_H
#define TESTGRAPHENE_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TextTestRunner.h>

#define ASSERT_VEC3_EQUAL(actual, expected) \
        do { \
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected.get(Math::Vec3::X), actual.get(Math::Vec3::X), 0.001f); \
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected.get(Math::Vec3::Y), actual.get(Math::Vec3::Y), 0.001f); \
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected.get(Math::Vec3::Z), actual.get(Math::Vec3::Z), 0.001f); \
        } while (false)

#endif  // TESTGRAPHENE_H
