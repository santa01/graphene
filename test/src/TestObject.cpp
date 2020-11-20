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
#include <MetaObject.h>
#include <Object.h>
#include <Vec3.h>

class GrapheneObject: public Graphene::MetaObject<GrapheneObject>, public Graphene::Object {
public:
    // Graphene::Object() is protected, sub-class to test
    GrapheneObject():
            Graphene::Object(GrapheneObject::ID) {
    }
};

class TestObject: public CppUnit::TestFixture {
public:
    void testTargetAt() {
        GrapheneObject object;
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
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(35.2548f, 71.5812f, 134.9922f));

        object.targetAt(1.0f, 2.0f, 3.0f);
        ASSERT_VEC3_EQUAL(object.getRight(), Math::Vec3(-0.6617f, 0.6412f, -0.3884f));
        ASSERT_VEC3_EQUAL(object.getUp(), Math::Vec3(-0.7205f, -0.4006f, 0.5659f));
        ASSERT_VEC3_EQUAL(object.getTarget(), Math::Vec3(0.0490f, 0.5540f, 0.8310f));
        ASSERT_VEC3_EQUAL(object.getRotationAngles(), Math::Vec3(25.5723f, 115.0127f, 99.1674f));
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestObject");
    suite->addTest(new CppUnit::TestCaller<TestObject>("testTargetAt", &TestObject::testTargetAt));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
