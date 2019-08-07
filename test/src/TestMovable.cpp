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
#include <Movable.h>
#include <Vec3.h>

class TestMovable: public CppUnit::TestFixture {
public:
    void testTranslate() {
        Graphene::Movable movable;
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::ZERO);

        movable.translate(Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::UNIT_X);

        movable.translate(Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::UNIT_Y);

        movable.translate(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(movable.getPosition(), (Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X));

        movable.translate(1.0f, -2.3f, 0.7f);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3(1.0f, -2.3f, 0.7f));

        movable.translate(Math::Vec3::ZERO);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::ZERO);

        movable.translate(-0.2f, 0.3f, 5.1f);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3(-0.2f, 0.3f, 5.1f));

        movable.translate(-movable.getPosition());
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3(0.2f, -0.3f, -5.1f));
    }

    void testMove() {
        Graphene::Movable movable;
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::ZERO);

        movable.move(Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::UNIT_X);

        movable.move(Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(movable.getPosition(), (Math::Vec3::UNIT_X + Math::Vec3::UNIT_Y));

        movable.move(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(movable.getPosition(), (Math::Vec3::UNIT_Y + Math::Vec3::UNIT_Z));

        movable.move(1.0f, -2.3f, 0.7f);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        movable.move(Math::Vec3::ZERO);
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        movable.move(-movable.getPosition());
        ASSERT_VEC3_EQUAL(movable.getPosition(), Math::Vec3::ZERO);
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestMovable");
    suite->addTest(new CppUnit::TestCaller<TestMovable>("testTranslate", &TestMovable::testTranslate));
    suite->addTest(new CppUnit::TestCaller<TestMovable>("testMove", &TestMovable::testMove));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
