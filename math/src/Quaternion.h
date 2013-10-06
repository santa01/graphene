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

#ifndef QUATERNION_H
#define QUATERNION_H

#include <Vec3.h>
#include <Mat4.h>
#include <cmath>

namespace Math {

/*!
 * \brief %Quaternion representation.
 * \details Quaternion implements basic operations that are:
 *          * multiplication;
 *          * normalization;
 *          * euler angles and Mat4 rotation matrix extraction.
 */
class Quaternion {
public:
    enum {
        X = 0,  /*!< X component index. */
        Y = 1,  /*!< Y component index. */
        Z = 2,  /*!< Z component index. */
        W = 3   /*!< W component index. */
    };

    /*!
     * \brief Default constructor.
     * \details Constructs a unit quaternion initializing every but W component with zero.
     */
    Quaternion() {
        this->vector[X] = 0.0f;
        this->vector[Y] = 0.0f;
        this->vector[Z] = 0.0f;
        this->vector[W] = 1.0f;
    }

    /*!
     * \brief Per-component constructor.
     * \details Constructs arbitrary quaternion based on x, y, z, w values.
     * \param x X component.
     * \param y Y component.
     * \param z Z component.
     * \param w W component.
     */
    Quaternion(float x, float y, float z, float w) {
        this->vector[X] = x;
        this->vector[Y] = y;
        this->vector[Z] = z;
        this->vector[W] = w;
    }

    /*!
     * \brief Axis-angle based constructor.
     * \details Constructs a quaternion describing arbitrary rotation around axis vector on angle radians.
     * \param axis Rotation axis vector.
     * \param angle Rotation angle in radians.
     */
    Quaternion(const Vec3& axis, float angle) {
        float sinAngle = sinf(angle / 2);

        this->vector[X] = axis.get(Vec3::X) * sinAngle;
        this->vector[Y] = axis.get(Vec3::Y) * sinAngle;
        this->vector[Z] = axis.get(Vec3::Z) * sinAngle;
        this->vector[W] = cosf(angle / 2);
    }

    /*!
     * \brief Quaternions multiplication.
     * \param quaternion %Quaternion multiplier.
     * \return Product quaternion.
     */
    Quaternion operator *(const Quaternion& quaternion) const;

    /*!
     * \brief %Quaternion normalization.
     * \return Normalized quaternion.
     * \note Method has a side-effect.
     */
    Quaternion& normalize() {
        float length = this->length();
        this->vector[X] /= length;
        this->vector[Y] /= length;
        this->vector[Z] /= length;
        this->vector[W] /= length;
        return *this;
    }

    /*!
     * \brief %Quaternion's length calculation.
     * \return %Quaternion length.
     */
    float length() const {
        return sqrtf(this->vector[X] * this->vector[X] +
                     this->vector[Y] * this->vector[Y] +
                     this->vector[Z] * this->vector[Z] +
                     this->vector[W] * this->vector[W]);
    }

    /*!
     * \brief %Quaternion's component selector.
     * \param index Component's index.
     * \return Component's value.
     * \note You are advised to use #X, #Y, #Z, #W constants as indices.
     */
    float get(int index) const {
        switch (index) {
            case X:
            case Y:
            case Z:
            case W:
                return this->vector[index];

            default:
                return NAN;
        }
    }

    /*!
     * \brief %Quaternion's component mutator.
     * \param index Component's index.
     * \param value Component's new value.
     * \note You are advised to use #X, #Y, #Z, #W constants as indices.
     */
    void set(int index, float value) {
        switch (index) {
            case X:
            case Y:
            case Z:
            case W:
                this->vector[index] = value;
                break;
        }
    }

    /*!
     * \brief Mat4 matrix extraction.
     * \details Composes Mat4 rotation matrix.
     * \return 4x4 two dimetional matrix.
     */
    Mat4 extractMat4() const;

    /*!
     * \brief Euler angles extraction.
     * \details Derives roll, yaw, pitch angle values in radians.
     * \param xAngle Rotation angle around X axis.
     * \param yAngle Rotation angle around Y axis.
     * \param zAngle Rotation angle around Z axis.
     */
    void extractEulerAngles(float& xAngle, float& yAngle, float& zAngle) const {
        xAngle = asinf(2 * (this->vector[X] * this->vector[Y] + this->vector[Z] * this->vector[W]));
        yAngle = atan2f(2 * (this->vector[Y] * this->vector[W] - this->vector[X] * this->vector[Z]),
                        1 - 2 * (this->vector[Y] * this->vector[Y] - this->vector[Z] * this->vector[Z]));
        zAngle = atan2f(2 * (this->vector[X] * this->vector[W] - this->vector[Y] * this->vector[Z]),
                        1 - 2 * (this->vector[X] * this->vector[X] - this->vector[Z] * this->vector[Z]));
    }

private:
    float vector[4];
};

}  // namespace Math

#endif  // QUATERNION_H
