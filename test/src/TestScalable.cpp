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

#include <TestGraphene.h>
#include <Scalable.h>
#include <Vec3.h>

class TestScalable: public CppUnit::TestFixture {
public:
    void testScale() {
        Graphene::Scalable scalable;
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(1.0f, 1.0f, 1.0f));

        scalable.scaleX(2.4f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(2.4f, 1.0f, 1.0f));

        scalable.scaleY(0.8f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(2.4f, 0.8f, 1.0f));

        scalable.scaleZ(1.1f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(2.4f, 0.8f, 1.1f));

        scalable.scale(0.2f, 0.3f, 0.4f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(0.48f, 0.24f, 0.44f));

        scalable.scale(0.2f, 1.0f, 0.2f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(0.096f, 0.24f, 0.088f));

        scalable.scale(1.0f, 1.0f, 1.0f);
        ASSERT_VEC3_EQUAL(scalable.getScalingFactors(), Math::Vec3(0.096f, 0.24f, 0.088f));
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestScalable");
    suite->addTest(new CppUnit::TestCaller<TestScalable>("testScale", &TestScalable::testScale));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
