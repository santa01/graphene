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

#ifndef VEC4_H
#define VEC4_H

#include <Vec3.h>
#include <cmath>

namespace Math {

/*!
 * \brief Four component vector.
 * \details Vec4 implements basic operations that are:
 *          * vector-vector addition, difference (both one and two operand);
 *          * vector-scalar multiplication (both one and two operand);
 *          * dot product calculation.
 */
class Vec4 {
public:
    enum {
        X = 0,  /*!< X component index. */
        Y = 1,  /*!< Y component index. */
        Z = 2,  /*!< Z component index. */
        W = 3   /*!< W component index. */
    };

    static const Vec4 ZERO;  /*!< Zero length vector. */

    /*!
     * \brief Default constructor.
     * \details Constructs a unit vector initializing every but W component with zero.
     */
    Vec4() {
        this->vector[X] = 0.0f;
        this->vector[Y] = 0.0f;
        this->vector[Z] = 0.0f;
        this->vector[W] = 1.0f;
    }

    /*!
     * \brief Per-component constructor.
     * \details Constructs arbitrary vector based on x, y, z, w values.
     * \param x X component.
     * \param y Y component.
     * \param z Z component.
     * \param w W component.
     */
    Vec4(float x, float y, float z, float w) {
        this->vector[X] = x;
        this->vector[Y] = y;
        this->vector[Z] = z;
        this->vector[W] = w;
    }

    /*!
     * \brief Vec3-based constructor.
     * \details Constructs arbitrary vector initializing x, y, z components
     *          from Vec3 vector and W component based on w value.
     * \param vector Source three component vector.
     * \param w W component.
     */
    Vec4(const Vec3& vector, float w) {
        this->vector[X] = vector.get(Vec3::X);
        this->vector[Y] = vector.get(Vec3::Y);
        this->vector[Z] = vector.get(Vec3::Z);
        this->vector[W] = w;
    }

    /*!
     * \brief Vectors difference.
     * \param vector Substructed vector.
     * \return Difference vector.
     */
    Vec4 operator -(const Vec4& vector) const {
        Vec4 me(*this);
        return me -= vector;
    }

    /*!
     * \brief Vectors addition.
     * \param vector Summand vector.
     * \return Sum vector.
     */
    Vec4 operator +(const Vec4& vector) const {
        Vec4 me(*this);
        return me += vector;
    }

    /*!
     * \brief Vector byt scalar multiplication.
     * \param scalar Scalar multiplier.
     * \return Product vector.
     */
    Vec4 operator *(float scalar) const {
        Vec4 me(*this);
        return me *= scalar;
    }

    /*!
     * \brief Vector substruction.
     * \param vector Substructed vector.
     * \return Difference vector.
     * \note Method has a side-effect.
     */
    Vec4& operator -=(const Vec4& vector) {
        this->vector[X] -= vector.get(X);
        this->vector[Y] -= vector.get(Y);
        this->vector[Z] -= vector.get(Z);
        this->vector[W] -= vector.get(W);
        return *this;
    }

    /*!
     * \brief Vector addition.
     * \param vector Summand vector.
     * \return Sum vector.
     * \note Method has a side-effect.
     */
    Vec4& operator +=(const Vec4& vector) {
        this->vector[X] += vector.get(X);
        this->vector[Y] += vector.get(Y);
        this->vector[Z] += vector.get(Z);
        this->vector[W] += vector.get(W);
        return *this;
    }

    /*!
     * \brief Scalar multiplication.
     * \param scalar Scalar multiplier.
     * \return Product vector.
     * \note Method has a side-effect.
     */
    Vec4& operator *=(float scalar) {
        this->vector[X] *= scalar;
        this->vector[Y] *= scalar;
        this->vector[Z] *= scalar;
        this->vector[W] *= scalar;
        return *this;
    }

    /*!
     * \brief Vectors equalty check.
     * \param vector Compared vector.
     * \return true if vectors are equal, false otherwise.
     */
    bool operator ==(const Vec4& vector) const {
        return (this->vector[X] == vector.get(X)) &&
               (this->vector[Y] == vector.get(Y)) &&
               (this->vector[Z] == vector.get(Z)) &&
               (this->vector[W] == vector.get(W));
    }

    /*!
     * \brief Vectors inequality check.
     * \param vector Compared vector.
     * \return false if vectors are equal, true otherwise.
     */
    bool operator !=(const Vec4& vector) const {
        return !(*this == vector);
    }

    /*!
     * \brief Vector inversion.
     * \return Inverted vector.
     * \note Method has a side-effect.
     */
    Vec4 operator -() const {
        return Vec4(-this->vector[X],
                    -this->vector[Y],
                    -this->vector[Z],
                    -this->vector[W]);
    }

    /*!
     * \brief Dot product calculation.
     * \param vector Vector mutliplier.
     * \return Scalar (dot) product.
     */
    float dot(const Vec4& vector) const {
        return this->vector[X] * vector.get(X) +
               this->vector[Y] * vector.get(Y) +
               this->vector[Z] * vector.get(Z) +
               this->vector[W] * vector.get(W);
    }

    /*!
     * \brief Vector's component selector.
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
     * \brief Vector's component mutator.
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
     * \brief Vector's data accessor.
     * \return Vector's data pointer.
     */
    const float* data() const  {
        return (float*)&this->vector;
    }

    /*!
     * \brief Vec3 vector extraction.
     * \details Composes Vec3 from x, y, z Vec4 components.
     * \return Three dimentional vector.
     */
    Vec3 extractVec3() const {
        return Vec3(this->vector[X],
                    this->vector[Y],
                    this->vector[Z]);
    }

private:
    float vector[4];
};

}  // namespace Math

#endif  // VEC4_H
