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

#ifndef MAT4_H
#define MAT4_H

#include <Mat3.h>
#include <Vec4.h>
#include <cmath>
#include <algorithm>

namespace Math {

/*!
 * \brief 4x4 two dimentional matrix.
 * \details Mat4 implements basic operations that are:
 *          * matrix-matrix addition, difference, multiplication;
 *          * matrix-vector multiplication;
 *          * transposition, LU decomposition, inversion.
 */
class Mat4 {
public:
    /*!
     * \brief Default constructor.
     * \details Constructs the identity matrix.
     */
    Mat4() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                this->matrix[i][j] = 0.0f;
            }
        }

        this->matrix[0][0] = 1.0f;
        this->matrix[1][1] = 1.0f;
        this->matrix[2][2] = 1.0f;
        this->matrix[3][3] = 1.0f;
    }

    /*!
     * \brief Matrices multiplication.
     * \param matrix Matrix multiplier.
     * \return Product matrix.
     */
    Mat4 operator *(const Mat4& matrix) const {
        Mat4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.set(i, j, this->matrix[i][0] * matrix.get(0, j) +
                                 this->matrix[i][1] * matrix.get(1, j) +
                                 this->matrix[i][2] * matrix.get(2, j) +
                                 this->matrix[i][3] * matrix.get(3, j));
            }
        }

        return result;
    }

    /*!
     * \brief Matrix by vector multiplication.
     * \param vector Vector multiplier.
     * \return Product vector.
     */
    Vec4 operator *(const Vec4& vector) const {
        Vec4 result;

        for (int i = 0; i < 4; i++) {
            result.set(i, this->matrix[i][0] * vector.get(Vec4::X) +
                          this->matrix[i][1] * vector.get(Vec4::Y) +
                          this->matrix[i][2] * vector.get(Vec4::Z) +
                          this->matrix[i][3] * vector.get(Vec4::W));
        }

        return result;
    }

    /*!
     * \brief Matrix by scalar multiplication.
     * \param scalar Scalar multiplier.
     * \return Product matrix.
     */
    Mat4 operator *(float scalar) const {
        Mat4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.set(i, j, this->matrix[i][j] * scalar);
            }
        }

        return result;
    }

    /*!
     * \brief Matrices addition.
     * \param matrix Summand matrix.
     * \return Sum matrix.
     */
    Mat4 operator +(const Mat4& matrix) const {
        Mat4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.set(i, j, this->matrix[i][j] + matrix.get(i, j));
            }
        }

        return result;
    }

    /*!
     * \brief Matrices substraction.
     * \param matrix Substracted matrix.
     * \return Difference matrix.
     */
    Mat4 operator -(const Mat4& matrix) const {
        Mat4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.set(i, j, this->matrix[i][j] - matrix.get(i, j));
            }
        }

        return result;
    }

    /*!
     * \brief Matrices equalty check.
     * \param matrix Compared matrix.
     * \return true if matrices are equal, false otherwise.
     */
    bool operator ==(const Mat4& matrix) const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (this->matrix[i][j] != matrix.get(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }

    /*!
     * \brief Matrices inequalty check.
     * \param matrix Compared matrix.
     * \return false if matrices are equal, true otherwise.
     */
    bool operator !=(const Mat4& matrix) const {
        return !(*this == matrix);
    }

    /*!
     * \brief Matrix transposition.
     * \return Transposed matrix.
     * \note Method has a side-effect.
     */
    Mat4& transpose() {
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 4; j++) {
                std::swap(this->matrix[j][i], this->matrix[i][j]);
            }
        }

        return *this;
    }

    /*!
     * \brief Matrix LU decomposition.
     * \details This method implements doolitle LU decomposition argorithm. References:
     *           * http://www.engr.colostate.edu/~thompson/hPage/CourseMat/Tutorials/CompMethods/doolittle.pdf
     *           * https://vismor.com/documents/network_analysis/matrix_algorithms/S4.SS2.php
     *           * http://en.wikipedia.org/wiki/LU_decomposition#Doolittle_algorithm
     *
     * \param lower Lower triangular matrix.
     * \param upper Upper triangular matrix.
     */
    void decompose(Mat4& lower, Mat4& upper) const;

    /*!
     * \brief Matrix inversion.
     * \details This methos finds inverse matrix by LU-decomposition and further equation solving:
     *           -# L * Z = I
     *           -# U * X = Z
     *           -# L * U * X = I
     *
     *           Where:
     *            * L and U are lower and upper triangular matrices got by decompose(),
     *            * X and I are respective columns of inverse and identity matrices.
     *
     * \return Inverted matrix.
     */
    Mat4& invert();

    /*!
     * \brief Solve matrix equation with a lower triangular matrix.
     * \details Performs forward substitution for lower triangular marix. References:
     *           * http://en.wikipedia.org/wiki/Triangular_matrix#Forward_and_back_substitution
     *
     * \param absolute Vector of absolute values.
     * \return Vector of unknown values.
     * \note Matrix is assumed to be triangular, no check is performed.
     */
    Vec4 solveL(const Vec4& absolute) const {
        Vec4 solution;

        for (int i = 0; i < 4; i++) {
            solution.set(i, absolute.get(i));
            for (int j = 0; j < i; j++) {
                solution.set(i, solution.get(i) - this->matrix[i][j] * solution.get(j));
            }
            solution.set(i, solution.get(i) / this->matrix[i][i]);
        }

        return solution;
    }

    /*!
     * \brief Solve matrix equation with an upper triangular matrix.
     * \details Performs backward substitution for upper triangular marix. References:
     *           * http://en.wikipedia.org/wiki/Triangular_matrix#Forward_and_back_substitution
     *
     * \param absolute Vector of absolute values.
     * \return Vector of unknown values.
     * \note Matrix is assumed to be triangular, no check is performed.
     */
    Vec4 solveU(const Vec4& absolute) const {
        Vec4 solution;

        for (int i = 3; i > -1; i--) {
            solution.set(i, absolute.get(i));
            for (int j = 3; j > i; j--) {
                solution.set(i, solution.get(i) - this->matrix[i][j] * solution.get(j));
            }
            solution.set(i, solution.get(i) / this->matrix[i][i]);
        }

        return solution;
    }

    /*!
     * \brief Matrix's element selector.
     * \param row Element's row.
     * \param column Element's column.
     * \return Element's value.
     */
    float get(int row, int column) const {
        if (column < 0 || column > 3 || row < 0 || row > 3) {
            return NAN;
        }

        return this->matrix[row][column];
    }

    /*!
     * \brief Matrix's element mutator.
     * \param row Element's row.
     * \param column Element's column.
     * \param value Element's new value.
     */
    void set(int row, int column, float value) {
        if (column < 0 || column > 3 || row < 0 || row > 3) {
            return;
        }

        this->matrix[row][column] = value;
    }

    /*!
     * \brief Matrix's data accessor.
     * \return Matrix's data pointer.
     */
    const float* data() const  {
        return (float*)&this->matrix;
    }

    /*!
     * \brief Mat3 matrix extraction.
     * \details Composes Mat3 from first three rows and columns.
     * \return 3x3 two dimetional matrix.
     */
    Mat3 extractMat3() const {
        Mat3 result;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.set(i, j, this->matrix[i][j]);
            }
        }

        return result;
    }

private:
    float matrix[4][4];
};

}  // namespace Math

#endif  // MAT4_H
