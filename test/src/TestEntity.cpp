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
#include <Entity.h>
#include <Vec3.h>

class TestEntity: public CppUnit::TestFixture {
public:
    void testRoll() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        entity.roll(30.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(30.0f, 0.0f, 0.0f));

        entity.roll(60.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(90.0f, 0.0f, 0.0f));

        entity.roll(90.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(180.0f, 0.0f, 0.0f));

        entity.roll(120.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(300.0f, 0.0f, 0.0f));
    }

    void testYaw() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        entity.yaw(30.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 30.0f, 0.0f));

        entity.yaw(60.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 90.0f, 0.0f));

        entity.yaw(90.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 180.0f, 0.0f));

        entity.yaw(120.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 300.0f, 0.0f));
    }

    void testPitch() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        entity.pitch(30.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 30.0f));

        entity.pitch(60.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 90.0f));

        entity.pitch(89.9999f); // 90.0f gives asinf(1.00000012f) -> nan
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 180.0f));

        /*
         * NOTE: asinf() returns [-M_PI / 2.0f; M_PI / 2.0f] ranged value.
         * Quaternion::extractEulerAngles() gives 120.0f - 180.0f == -60.0f angle.
         */
        entity.pitch(120.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 240.0f));
    }

    void testRotate() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(0.0f, 0.0f, 0.0f));

        entity.rotate(Math::Vec3(1.0f, 2.0f, 3.0f), 30.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(4.0493f, 12.8253f, 24.8365f));

        entity.rotate(Math::Vec3(-2.0f, 1.0f, 2.5f), 45.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(-20.6431f, 29.5240f, 53.1943f));

        entity.rotate(Math::Vec3(2.0f, -7.3f, -1.8f), 20.0f);
        ASSERT_VECTORS_EQUAL(entity.getRotationAngles(), Math::Vec3(-16.3604f, 11.0556f, 47.8192f));
    }

    void testScale() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(1.0f, 1.0f, 1.0f));

        entity.scaleX(2.4f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(2.4f, 1.0f, 1.0f));

        entity.scaleY(0.8f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(2.4f, 0.8f, 1.0f));

        entity.scaleZ(1.1f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(2.4f, 0.8f, 1.1f));

        entity.scale(0.2f, 0.3f, 0.4f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(0.48f, 0.24f, 0.44f));

        entity.scale(0.2f, 1.0f, 0.2f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(0.096f, 0.24f, 0.088f));

        entity.scale(1.0f, 1.0f, 1.0f);
        ASSERT_VECTORS_EQUAL(entity.getScalingFactors(), Math::Vec3(0.096f, 0.24f, 0.088f));
    }

    void testTranslate() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::ZERO);

        entity.translate(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::UNIT_X);

        entity.translate(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::UNIT_Y);

        entity.translate(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), (Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X));

        entity.translate(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3(1.0f, -2.3f, 0.7f));

        entity.translate(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::ZERO);

        entity.translate(-0.2f, 0.3f, 5.1f);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3(-0.2f, 0.3f, 5.1f));

        entity.translate(-entity.getPosition());
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3(0.2f, -0.3f, -5.1f));
    }

    void testMove() {
        Graphene::Entity entity;
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::ZERO);

        entity.move(Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::UNIT_X);

        entity.move(Math::Vec3::UNIT_Y);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), (Math::Vec3::UNIT_X + Math::Vec3::UNIT_Y));

        entity.move(Math::Vec3::UNIT_Z - Math::Vec3::UNIT_X);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), (Math::Vec3::UNIT_Y + Math::Vec3::UNIT_Z));

        entity.move(1.0f, -2.3f, 0.7f);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        entity.move(Math::Vec3::ZERO);
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3(1.0f, -1.3f, 1.7f));

        entity.move(-entity.getPosition());
        ASSERT_VECTORS_EQUAL(entity.getPosition(), Math::Vec3::ZERO);
    }
};

int main() {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("TestEntity");
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testRoll", &TestEntity::testRoll));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testYaw", &TestEntity::testYaw));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testPitch", &TestEntity::testPitch));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testRotate", &TestEntity::testRotate));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testScale", &TestEntity::testScale));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testTranslate", &TestEntity::testTranslate));
    suite->addTest(new CppUnit::TestCaller<TestEntity>("testMove", &TestEntity::testMove));

    CppUnit::TextTestRunner runner;
    runner.addTest(suite);

    return runner.run() ? 0 : 1;
}