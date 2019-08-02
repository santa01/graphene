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

#include <Camera.h>
#include <cmath>

namespace Graphene {

Camera::Camera(ProjectionType type):
        Object(ObjectType::CAMERA),
        type(type) {
    this->updateProjection();
}

ProjectionType Camera::getProjectionType() const {
    return this->type;
}

void Camera::setProjectionType(ProjectionType type) {
    this->type = type;
    this->updateProjection();
}

float Camera::getAspectRatio() const {
    return this->aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    this->updateProjection();
}

float Camera::getNearPlane() const {
    return this->nearPlane;
}

void Camera::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    this->updateProjection();
}

float Camera::getFarPlane() const {
    return this->farPlane;
}

void Camera::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    this->updateProjection();
}

float Camera::getFov() const {
    return this->fov;
}

void Camera::setFov(float fov) {
    this->fov = fov;
    this->updateProjection();
}

const Math::Mat4& Camera::getProjection() const {
    return this->projection;
}

void Camera::lookAt(float x, float y, float z) {
    this->lookAt(Math::Vec3(x, y, z));
}

void Camera::lookAt(const Math::Vec3& vector) {
    this->targetAt(vector);
}

void Camera::updateProjection() {
    /*
     * References:
     *     http://www.songho.ca/opengl/gl_projectionmatrix.html
     *     https://softwareengineering.stackexchange.com/a/88776
     *
     * Frustum dimentions:
     *     N - near plane
     *     F - far plane
     *
     * Near plane dimentions:
     *     R - right
     *     L - left
     *     T - top
     *     B - botton
     *
     * Prespective projection for eye space to NDC transformation
     * (left handed coordinate system to left handed coordinate system):
     *     | 2 * N / (R - L) ;        0        ; - (R + L) / (R - L) ;          0          |
     *     |        0        ; 2 * N / (T - B) ; - (T + B) / (T - B) ;          0          |
     *     |        0        ;        0        ; - (N + F) / (N - F) ; 2 * N * F / (N - F) |
     *     |        0        ;        0        ;           1         ;          0          |
     *
     * Where:
     *     L = -R
     *     B = -T
     *     AR = (2 * R) / (2 * T)
     *     R - L = 2 * R = 2 * N * tan(FOV / 2) * AR
     *     R + L = 0
     *     T - B = 2 * T = 2 * N * tan(FOV / 2)
     *     T + B = 0
     *
     * Orthographic projection for eye space to NDC transformation
     * (left handed coordinate system to left handed coordinate system):
     *     | 2 / (R - L) ;      0      ;           0         ; - (R + L) / (R - L) |
     *     |      0      ; 2 / (T - B) ;           0         ; - (T + B) / (T - B) |
     *     |      0      ;      0      ;      2 / (F - N)    ; - (F + N) / (F - N) |
     *     |      0      ;      0      ;           0         ;          1          |
     *
     * Where:
     *     L = -R
     *     B = -T
     *     AR = (2 * R) / (2 * T)
     *     R = T = (F + N) / 2
     *     R - L = (F - N) * AR
     *     R + L = 0
     *     T - B = F - N
     *     T + B = 0
     */

    this->projection = Math::Mat4();

    float pi = static_cast<float>(M_PI);
    float fovRadian = this->fov * pi / 180.0f;

    switch (this->type) {
        case ProjectionType::PERSPECTIVE:
            this->projection.set(0, 0, 1.0f / (tanf(fovRadian / 2.0f) * this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (tanf(fovRadian / 2.0f)));
            this->projection.set(2, 2, - (this->nearPlane + this->farPlane) / (this->nearPlane - this->farPlane));
            this->projection.set(2, 3, 2.0f * this->nearPlane * this->farPlane / (this->nearPlane - this->farPlane));
            this->projection.set(3, 2, 1.0f);
            this->projection.set(3, 3, 0.0f);
            break;
        case ProjectionType::ORTHOGRAPHIC:
            this->projection.set(0, 0, 1.0f / ((this->farPlane - this->nearPlane) * this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 2, 2.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 3, - (this->farPlane + this->nearPlane) / (this->farPlane - this->nearPlane));
            break;
    }
}

}  // namespace Graphene
