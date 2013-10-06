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

#include <Mat3.h>

namespace Math {

void Mat3::decompose(Mat3& lower, Mat3& upper) const {
    for (int i = 0; i < 3; i++) {
        for (int j = i; j < 3; j++) {
            lower.set(i, j, (i == j) ? 1.0f : 0.0f);
            upper.set(i, j, 0.0f);
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i; j < 3; j++) {
            upper.set(i, j, this->matrix[i][j]);
            for (int k = 0; k < i; k++) {
                upper.set(i, j, upper.get(i, j) - lower.get(i, k) * upper.get(k, j));
            }
            upper.set(i, j, upper.get(i, j) / lower.get(i, i));
        }

        for (int j = i + 1; j < 3; j++) {
            lower.set(j, i, this->matrix[j][i]);
            for (int k = 0; k < i; k++) {
                lower.set(j, i, lower.get(j, i) - lower.get(j, k) * upper.get(k, i));
            }
            lower.set(j, i, lower.get(j, i) / upper.get(i, i));
        }
    }
}

Mat3& Mat3::invert() {
    Mat3 lower;
    Mat3 upper;
    this->decompose(lower, upper);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->matrix[i][j] = 0.0f;
        }
    }

    Vec3 identity[] = {
        Vec3(1.0f, 0.0f, 0.0f),
        Vec3(0.0f, 1.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f)
    };

    for (int i = 0; i < 3; i++) {
        Vec3 z(lower.solveL(identity[i]));
        Vec3 x(upper.solveU(z));
        for (int j = 0; j < 3; j++) {
            this->matrix[j][i] = x.get(j);
        }
    }

    return *this;
}

}  // namespace Math
