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

#ifndef VEC3_H
#define VEC3_H

#include <cmath>

namespace Math {

/*!
 * \brief Three component vector.
 * \details Vec3 implements basic operations that are:
 *          * vector-vector addition, difference (both one and two operand);
 *          * vector-scalar multiplication (both one and two operand);
 *          * dot and cross product, normalization;
 *          * length, square length calculation.
 */
class Vec3 {
public:
    enum {
        X = 0,  /*!< X component index. */
        Y = 1,  /*!< Y component index. */
        Z = 2   /*!< Z component index. */
    };

    static const Vec3 UNIT_X;  /*!< X unit vector. */
    static const Vec3 UNIT_Y;  /*!< Y unit vector. */
    static const Vec3 UNIT_Z;  /*!< Z unit vector. */
    static const Vec3 ZERO;    /*!< Zero length vector. */

    /*!
     * \brief Default constructor.
     * \details Constructs zero-length vector.
     */
    Vec3() {
        for (int i = 0; i < 3; i++) {
            this->vector[i] = 0.0f;
        }
    }

    /*!
     * \brief Per-component constructor.
     * \details Constructs arbitrary vector based on x, y, z values.
     * \param x X component.
     * \param y Y component.
     * \param z Z component.
     */
    Vec3(float x, float y, float z) {
        this->vector[X] = x;
        this->vector[Y] = y;
        this->vector[Z] = z;
    }

    /*!
     * \brief Vectors substraction.
     * \param vector Substructed vector.
     * \return Difference vector.
     */
    Vec3 operator -(const Vec3& vector) const {
        Vec3 me(*this);
        return me -= vector;
    }

    /*!
     * \brief Vectors addition.
     * \param vector Summand vector.
     * \return Sum vector.
     */
    Vec3 operator +(const Vec3& vector) const {
        Vec3 me(*this);
        return me += vector;
    }

    /*!
     * \brief Vector by scalar multiplication.
     * \param scalar Scalar multiplier.
     * \return Product vector.
     */
    Vec3 operator *(float scalar) const {
        Vec3 me(*this);
        return me *= scalar;
    }

    /*!
     * \brief Vector substruction.
     * \param vector Substructed vector.
     * \return Difference vector.
     * \note Method has a side-effect.
     */
    Vec3& operator -=(const Vec3& vector) {
        this->vector[X] -= vector.get(X);
        this->vector[Y] -= vector.get(Y);
        this->vector[Z] -= vector.get(Z);
        return *this;
    }

    /*!
     * \brief Vector addition.
     * \param vector Summand vector.
     * \return Sum vector.
     * \note Method has a side-effect.
     */
    Vec3& operator +=(const Vec3& vector) {
        this->vector[X] += vector.get(X);
        this->vector[Y] += vector.get(Y);
        this->vector[Z] += vector.get(Z);
        return *this;
    }

    /*!
     * \brief Scalar multiplication.
     * \param scalar Scalar multiplier.
     * \return Product vector.
     * \note Method has a side-effect.
     */
    Vec3& operator *=(float scalar) {
        this->vector[X] *= scalar;
        this->vector[Y] *= scalar;
        this->vector[Z] *= scalar;
        return *this;
    }

    /*!
     * \brief Vectors equalty check.
     * \param vector Compared vector.
     * \return true if vectors are equal, false otherwise.
     */
    bool operator ==(const Vec3& vector) const {
        return (this->vector[X] == vector.get(X)) &&
               (this->vector[Y] == vector.get(Y)) &&
               (this->vector[Z] == vector.get(Z));
    }

    /*!
     * \brief Vectors inequalty check.
     * \param vector Compared vector.
     * \return false if vectors are equal, true otherwise.
     */
    bool operator !=(const Vec3& vector) const {
        return !(*this == vector);
    }

    /*!
     * \brief Vector inversion.
     * \return Inverted vector.
     * \note Method has a side-effect.
     */
    Vec3 operator -() const {
        return Vec3(-this->vector[X],
                    -this->vector[Y],
                    -this->vector[Z]);
    }

    /*!
     * \brief Dot product calculation.
     * \param vector Vector mutliplier.
     * \return Scalar (dot) product.
     */
    float dot(const Vec3& vector) const {
        return this->vector[X] * vector.get(X) +
               this->vector[Y] * vector.get(Y) +
               this->vector[Z] * vector.get(Z);
    }

    /*!
     * \brief Cross product calculation.
     * \param vector Vector mutliplier.
     * \return Vector (cross) product.
     */
    Vec3 cross(const Vec3& vector) const {
        return Vec3(this->vector[Y] * vector.get(Z) - this->vector[Z] * vector.get(Y),
                    this->vector[Z] * vector.get(X) - this->vector[X] * vector.get(Z),
                    this->vector[X] * vector.get(Y) - this->vector[Y] * vector.get(X));
    }

    /*!
     * \brief Vector normalization.
     * \return Normalized (unit) vector.
     * \note Method has a side-effect.
     */
    Vec3& normalize() {
        float length = this->length();
        this->vector[X] /= length;
        this->vector[Y] /= length;
        this->vector[Z] /= length;
        return *this;
    }

    /*!
     * \brief Vector's length calculation.
     * \return Vector length.
     */
    float length() const {
        return sqrtf(this->squareLength());
    }

    /*!
     * \brief Vector's square length calculation.
     * \return Vector square length.
     */
    float squareLength() const {
        return this->vector[X] * this->vector[X] +
               this->vector[Y] * this->vector[Y] +
               this->vector[Z] * this->vector[Z];
    }

    /*!
     * \brief Vector's component selector.
     * \param index Component's index.
     * \return Component's value.
     * \note You are advised to use #X, #Y, #Z constants as indices.
     */
    float get(int index) const {
        switch (index) {
            case X:
            case Y:
            case Z:
                return this->vector[index];

            default:
                return NAN;
        }
    }

    /*!
     * \brief Vector's component mutator.
     * \param index Component's index.
     * \param value Component's new value.
     * \note You are advised to use #X, #Y, #Z constants as indices.
     */
    void set(int index, float value) {
        switch (index) {
            case X:
            case Y:
            case Z:
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

private:
    float vector[3];
};

}  // namespace Math

#endif  // VEC3_H
