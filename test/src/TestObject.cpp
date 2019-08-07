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
#include <Object.h>
#include <Vec3.h>

class TestObject: public CppUnit::TestFixture {
public:
    void testTargetAt() {
        Graphene::Object object(Graphene::ObjectType::ENTITY);
        ASSERT_VEC3_EQUAL(object.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(object.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        object.targetAt(Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(object.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(object.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        object.targetAt(Math::Vec3::UNIT_Y - Math::Vec3(0.0005f, 0.0f, 0.0f));  // UNIT_Y gives nan for Z angle rotation
        ASSERT_VEC3_EQUAL(object.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(object.getUp(), -Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 89.9720f));

        object.targetAt(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(object.getRight(), Math::Vec3(-0.5f, 0.7071f, -0.5f));
        ASSERT_VEC3_EQUAL(object.getUp(), Math::Vec3(-0.5f, -0.7071f, -0.5f));
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3(-0.7071f, 0.0f, 0.7071f));
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(35.2739f, 108.4430f, 134.9517f));

        object.targetAt(1.0f, 2.0f, 3.0f);
        ASSERT_VEC3_EQUAL(object.getRight(), Math::Vec3(-0.6617f, 0.6412f, -0.3884f));
        ASSERT_VEC3_EQUAL(object.getUp(), Math::Vec3(-0.6356f, -0.3534f, 0.4993f));
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3(0.0473f, 0.5345f, 0.8017f));
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(4.9330f, 159.7304f, 125.1279f));
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestObject");
    suite->addTest(new CppUnit::TestCaller<TestObject>("testTargetAt", &TestObject::testTargetAt));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
