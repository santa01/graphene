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
#include <Rotatable.h>
#include <Vec3.h>

class TestRotatable: public CppUnit::TestFixture {
public:
    void testRoll() {
        Graphene::Rotatable rotatable;
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        rotatable.roll(30.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(0.0f, 0.866f, 0.5f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.0f, -0.5f, 0.866f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(30.0f, 0.0f, 0.0f));

        rotatable.roll(60.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), -Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(90.0f, 0.0f, 0.0f));

        rotatable.roll(90.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), -Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), -Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(180.0f, 0.0f, 0.0f));

        rotatable.roll(120.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(0.0f, 0.5f, -0.866f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.0f, 0.866f, 0.5f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(300.0f, 0.0f, 0.0f));
    }

    void testYaw() {
        Graphene::Rotatable rotatable;
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        rotatable.yaw(30.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.866f, 0.0f, -0.5f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.5f, 0.0f, 0.866f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 30.0f, 0.0f));

        rotatable.yaw(60.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        rotatable.yaw(90.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), -Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), -Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 180.0f, 0.0f));

        rotatable.yaw(120.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.5f, 0.0f, 0.866f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(-0.866f, 0.0f, 0.5f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 300.0f, 0.0f));
    }

    void testPitch() {
        Graphene::Rotatable rotatable;
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        rotatable.pitch(30.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.866f, 0.5f, 0.0f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(-0.5f, 0.866f, 0.0f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 30.0f));

        rotatable.pitch(60.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), -Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 90.0f));

        rotatable.pitch(89.9999f);  // 90.0f gives asinf(1.00000012f) -> nan
        ASSERT_VEC3_EQUAL(rotatable.getRight(), -Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), -Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 180.0f));

        /*
         * NOTE: asinf() returns [-M_PI / 2.0f; M_PI / 2.0f] ranged value.
         * Quaternion::extractEulerAngles() gives 120.0f - 180.0f == -60.0f angle.
         */
        rotatable.pitch(120.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.5f, -0.866f, 0.0f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(0.866f, 0.5f, 0.0f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 240.0f));
    }

    void testRotate() {
        Graphene::Rotatable rotatable;
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        rotatable.rotate(Math::Vec3(1.0f, 2.0f, 3.0f), 30.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.8780f, 0.4193f, -0.2305f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(-0.3817f, 0.9043f, 0.1910f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.2864f, -0.0762f, 0.9548f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(4.0493f, 12.8253f, 24.8365f));

        rotatable.rotate(Math::Vec3(-2.0f, 1.0f, 2.5f), 45.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.4441f, 0.5901f, -0.6740f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(-0.8129f, 0.5817f, -0.0260f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.4168f, 0.5298f, 0.7385f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(-20.6431f, 29.5240f, 53.1943f));

        rotatable.rotate(Math::Vec3(2.0f, -7.3f, -1.8f), 20.0f);
        ASSERT_VEC3_EQUAL(rotatable.getRight(), Math::Vec3(0.6769f, 0.5971f, -0.4302f));
        ASSERT_VEC3_EQUAL(rotatable.getUp(), Math::Vec3(-0.7208f, 0.6557f, -0.2241f));
        ASSERT_VEC3_EQUAL(rotatable.getTarget(), Math::Vec3(0.1800f, 0.4323f, 0.8835f));
        ASSERT_VEC3_EQUAL(rotatable.getRotationAngles(), Math::Vec3(-16.3604f, 11.0556f, 47.8192f));
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestRotatable");
    suite->addTest(new CppUnit::TestCaller<TestRotatable>("testRoll", &TestRotatable::testRoll));
    suite->addTest(new CppUnit::TestCaller<TestRotatable>("testYaw", &TestRotatable::testYaw));
    suite->addTest(new CppUnit::TestCaller<TestRotatable>("testPitch", &TestRotatable::testPitch));
    suite->addTest(new CppUnit::TestCaller<TestRotatable>("testRotate", &TestRotatable::testRotate));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
