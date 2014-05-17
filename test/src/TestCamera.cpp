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
#include <Camera.h>
#include <Vec3.h>

class TestCamera: public CppUnit::TestFixture {
public:
    void testRoll() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        camera.roll(30.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(0.0f, 0.866f, 0.5f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.0f, -0.5f, 0.866f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(30.0f, 0.0f, 0.0f));

        camera.roll(60.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), -Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(90.0f, 0.0f, 0.0f));

        camera.roll(90.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), -Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(180.0f, 0.0f, 0.0f));

        camera.roll(120.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(0.0f, 0.5f, -0.866f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.0f, 0.866f, 0.5f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(300.0f, 0.0f, 0.0f));
    }

    void testYaw() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        camera.yaw(30.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.866f, 0.0f, -0.5f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.5f, 0.0f, 0.866f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 30.0f, 0.0f));

        camera.yaw(60.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        camera.yaw(90.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 180.0f, 0.0f));

        camera.yaw(120.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.5f, 0.0f, 0.866f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(-0.866f, 0.0f, 0.5f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 300.0f, 0.0f));
    }

    void testPitch() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        camera.pitch(30.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.866f, 0.5f, 0.0f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(-0.5f, 0.866f, 0.0f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 30.0f));

        camera.pitch(60.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getUp(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 90.0f));

        camera.pitch(89.9999f); // 90.0f gives asinf(1.00000012f) -> nan
        ASSERT_VECTORS_EQUAL(camera.getRight(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), -Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 180.0f));

        /*
         * NOTE: asinf() returns [-M_PI / 2.0f; M_PI / 2.0f] ranged value.
         * Quaternion::extractEulerAngles() gives 120.0f - 180.0f == -60.0f angle.
         */
        camera.pitch(120.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.5f, -0.866f, 0.0f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(0.866f, 0.5f, 0.0f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 240.0f));
    }

    void testRotate() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        camera.rotate(Math::Vec3(1.0f, 2.0f, 3.0f), 30.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.8780f, 0.4193f, -0.2305f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(-0.3817f, 0.9043f, 0.1910f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.2864f, -0.0762f, 0.9521f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(4.0493f, 12.8253f, 24.8365f));

        camera.rotate(Math::Vec3(-2.0f, 1.0f, 2.5f), 45.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.4441f, 0.5901f, -0.6740f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(-0.8030f, 0.5746f, -0.0260f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.4276f, 0.5435f, 0.7577f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(-20.6431f, 29.5240f, 53.1943f));

        camera.rotate(Math::Vec3(2.0f, -7.3f, -1.8f), 20.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.6769f, 0.5971f, -0.4302f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(-0.7119f, 0.6476f, -0.2213f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.1844f, 0.4429f, 0.9050f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(-16.3604f, 11.0556f, 47.8192f));
    }

    void testLookAt() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        camera.lookAt(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        camera.lookAt(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getRight(), -Math::Vec3::UNIT_Z);
        ASSERT_VECTORS_EQUAL(camera.getUp(), -Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 90.0f));

        camera.lookAt(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.7071f, 0.0f, 0.7071f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(-0.7071f, 0.0f, 0.7071f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(90.0f, 45.0f, 135.0f));

        camera.lookAt(1.0f, 2.0f, 3.0f);
        ASSERT_VECTORS_EQUAL(camera.getRight(), Math::Vec3(0.9486f, 0.0f, -0.3162f));
        ASSERT_VECTORS_EQUAL(camera.getUp(), Math::Vec3(-0.1690f, 0.8451f, -0.5070f));
        ASSERT_VECTORS_EQUAL(camera.getTarget(), Math::Vec3(0.2672f, 0.5345f, 0.8017f));
        ASSERT_VECTORS_EQUAL(camera.getRotationAngles(), Math::Vec3(98.5572f, 150.7379f, 78.1453f));
    }

    void testTranslate() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::ZERO);

        camera.translate(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::UNIT_X);

        camera.translate(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::UNIT_Y);

        camera.translate(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), (Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X));

        camera.translate(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3(1.0f, -2.3f, 0.7f));

        camera.translate(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::ZERO);

        camera.translate(-0.2f, 0.3f, 5.1f);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3(-0.2f, 0.3f, 5.1f));

        camera.translate(-camera.getPosition());
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3(0.2f, -0.3f, -5.1f));
    }

    void testMove() {
        Graphene::Camera camera;
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::ZERO);

        camera.move(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::UNIT_X);

        camera.move(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), (Math::Vec3::UNIT_X + Math::Vec3::UNIT_Y));

        camera.move(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), (Math::Vec3::UNIT_Y + Math::Vec3::UNIT_Z));

        camera.move(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        camera.move(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        camera.move(-camera.getPosition());
        ASSERT_VECTORS_EQUAL(camera.getPosition(), Math::Vec3::ZERO);
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestCamera");
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testRoll", &TestCamera::testRoll));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testYaw", &TestCamera::testYaw));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testPitch", &TestCamera::testPitch));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testRotate", &TestCamera::testRotate));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testLookAt", &TestCamera::testLookAt));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testTranslate", &TestCamera::testTranslate));
    suite->addTest(new CppUnit::TestCaller<TestCamera>("testMove", &TestCamera::testMove));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}
