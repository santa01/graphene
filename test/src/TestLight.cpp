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
#include <Light.h>
#include <Vec3.h>

class TestLight: public CppUnit::TestFixture {
public:
    void testRoll() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        light.roll(30.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(30.0f, 0.0f, 0.0f));

        light.roll(60.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(90.0f, 0.0f, 0.0f));

        light.roll(90.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(180.0f, 0.0f, 0.0f));

        light.roll(120.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(300.0f, 0.0f, 0.0f));
    }

    void testYaw() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        light.yaw(30.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.866f, 0.0f, -0.5f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 30.0f, 0.0f));

        light.yaw(60.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        light.yaw(90.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 180.0f, 0.0f));

        light.yaw(120.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.5f, 0.0f, 0.866f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 300.0f, 0.0f));
    }

    void testPitch() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        light.pitch(30.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.866f, 0.5f, 0.0f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 30.0f));

        light.pitch(60.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 90.0f));

        light.pitch(89.9999f); // 90.0f gives asinf(1.00000012f) -> nan
        ASSERT_VECTORS_EQUAL(light.getDirection(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 180.0f));

        /*
         * NOTE: asinf() returns [-M_PI / 2.0f; M_PI / 2.0f] ranged value.
         * Quaternion::extractEulerAngles() gives 120.0f - 180.0f == -60.0f angle.
         */
        light.pitch(120.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.5f, -0.866f, 0.0f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 240.0f));
    }

    void testRotate() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        light.rotate(Math::Vec3(1.0f, 2.0f, 3.0f), 30.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.8755f, 0.4200f, -0.2385f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(4.0493f, 12.8253f, 24.8365f));

        light.rotate(Math::Vec3(-2.0f, 1.0f, 2.5f), 45.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.4292f, 0.6077f, -0.6558f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(-20.6431f, 29.5240f, 53.1943f));

        light.rotate(Math::Vec3(2.0f, -7.3f, -1.8f), 20.0f);
        ASSERT_VECTORS_EQUAL(light.getDirection(), Math::Vec3(0.6642f, 0.6122f, -0.4208f));
        ASSERT_VECTORS_EQUAL(light.getRotationAngles(), Math::Vec3(-16.3604f, 11.0556f, 47.8192f));
    }

    void testTranslate() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::ZERO);

        light.translate(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::UNIT_X);

        light.translate(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::UNIT_Y);

        light.translate(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getPosition(), (Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X));

        light.translate(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3(1.0f, -2.3f, 0.7f));

        light.translate(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::ZERO);

        light.translate(-0.2f, 0.3f, 5.1f);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3(-0.2f, 0.3f, 5.1f));

        light.translate(-light.getPosition());
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3(0.2f, -0.3f, -5.1f));
    }

    void testMove() {
        Graphene::Light light;
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::ZERO);

        light.move(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::UNIT_X);

        light.move(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(light.getPosition(), (Math::Vec3::UNIT_X + Math::Vec3::UNIT_Y));

        light.move(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(light.getPosition(), (Math::Vec3::UNIT_Y + Math::Vec3::UNIT_Z));

        light.move(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        light.move(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        light.move(-light.getPosition());
        ASSERT_VECTORS_EQUAL(light.getPosition(), Math::Vec3::ZERO);
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestLight");
    suite->addTest(new CppUnit::TestCaller<TestLight>("testRoll", &TestLight::testRoll));
    suite->addTest(new CppUnit::TestCaller<TestLight>("testYaw", &TestLight::testYaw));
    suite->addTest(new CppUnit::TestCaller<TestLight>("testPitch", &TestLight::testPitch));
    suite->addTest(new CppUnit::TestCaller<TestLight>("testRotate", &TestLight::testRotate));
    suite->addTest(new CppUnit::TestCaller<TestLight>("testTranslate", &TestLight::testTranslate));
    suite->addTest(new CppUnit::TestCaller<TestLight>("testMove", &TestLight::testMove));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
