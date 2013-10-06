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

#include <Quaternion.h>

namespace Math {

Quaternion Quaternion::operator *(const Quaternion& quaternion) const {
    Quaternion result;

    result.set(W, this->vector[W] * quaternion.get(W) -
                  this->vector[X] * quaternion.get(X) -
                  this->vector[Y] * quaternion.get(Y) -
                  this->vector[Z] * quaternion.get(Z));

    result.set(X, this->vector[W] * quaternion.get(X) +
                  this->vector[X] * quaternion.get(W) +
                  this->vector[Y] * quaternion.get(Z) -
                  this->vector[Z] * quaternion.get(Y));

    result.set(Y, this->vector[W] * quaternion.get(Y) +
                  this->vector[Y] * quaternion.get(W) +
                  this->vector[Z] * quaternion.get(X) -
                  this->vector[X] * quaternion.get(Z));

    result.set(Z, this->vector[W] * quaternion.get(Z) +
                  this->vector[Z] * quaternion.get(W) +
                  this->vector[X] * quaternion.get(Y) -
                  this->vector[Y] * quaternion.get(X));

    result.normalize();
    return result;
}

Mat4 Quaternion::extractMat4() const {
    Mat4 result;

    result.set(0, 0, 1 - 2 * this->vector[Y] * this->vector[Y] -
                         2 * this->vector[Z] * this->vector[Z]);
    result.set(0, 1, 2 * this->vector[X] * this->vector[Y] -
                     2 * this->vector[Z] * this->vector[W]);
    result.set(0, 2, 2 * this->vector[X] * this->vector[Y] +
                     2 * this->vector[Y] * this->vector[W]);

    result.set(1, 0, 2 * this->vector[X] * this->vector[Y] +
                     2 * this->vector[Z] * this->vector[W]);
    result.set(1, 1, 1 - 2 * this->vector[X] * this->vector[X] -
                         2 * this->vector[Z] * this->vector[Z]);
    result.set(1, 2, 2 * this->vector[Y] * this->vector[Z] -
                     2 * this->vector[X] * this->vector[W]);

    result.set(2, 0, 2 * this->vector[X] * this->vector[Z] -
                     2 * this->vector[Y] * this->vector[W]);
    result.set(2, 1, 2 * this->vector[Y] * this->vector[Z] +
                     2 * this->vector[X] * this->vector[W]);
    result.set(2, 2, 1 - 2 * this->vector[X] * this->vector[X] -
                         2 * this->vector[Y] * this->vector[Y]);

    return result;
}

}  // namespace Math
