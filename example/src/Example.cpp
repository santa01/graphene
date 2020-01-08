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

#include <Example.h>
#include <EngineConfig.h>
#include <ObjectManager.h>
#include <RenderManager.h>
#include <Camera.h>
#include <Vec3.h>

Example::Example() {
    Graphene::GetEngineConfig().setDataDirectory("example");
}

void Example::onMouseMotion(int x, int y) {
    // Window origin is at the top left corder
    float xAngle = y * MOUSE_SPEED;  // Positive y: mouse moved top to bottom
    float yAngle = x * MOUSE_SPEED;  // Positive x: mouse moved left to right
    static float roll = 0.0f;

    // Left handed world coordinate system, positive angle rotations are clockwise
    if (roll + xAngle > 90.0f) {
        xAngle = 90.0f - roll;
    } else if (roll + xAngle < -90.0f) {
        xAngle = -90.0f - roll;
    }
    roll += xAngle;

    // Moving mouse left to right (positive yAngle) rotates camera to the right.
    this->player->rotate(Math::Vec3::UNIT_Y, yAngle);

    // Moving mouse top to bottom (positive xAngle) rotates camera down.
    if (xAngle != 0) {
        this->player->roll(xAngle);
    }
}

void Example::onKeyboardKey(Graphene::KeyboardKey key, bool state) {
    if (state) {
        if (key == Graphene::KeyboardKey::KEY_1) {
            this->getWindow()->setFullscreen(true);
        }
        if (key == Graphene::KeyboardKey::KEY_2) {
            this->getWindow()->setFullscreen(false);
        }
    }
}

void Example::onSetup() {

    /* Setup render */

    auto& renderManager = Graphene::GetRenderManager();
    renderManager.setLightPass(true);

    /* Create objects */

    auto& objectManager = Graphene::GetObjectManager();

    auto camera = objectManager.createCamera(Graphene::ProjectionType::PERSPECTIVE);
    auto flashLight = objectManager.createLight(Graphene::LightType::SPOT);

    flashLight->setBlend(0.25f);
    flashLight->setAngle(20.0f);
    flashLight->setFalloff(8.0f);
    flashLight->move(0.2f, -0.2f, 0.0f);

    /* Populate scene with objects */

    auto scene = this->createScene("worlds/desert.scene");

    this->player = scene->getPlayer();
    this->player->attachObject(camera);
    this->player->attachObject(flashLight);

    /* Keep mouse inside the window */

    auto window = this->getWindow();
    window->captureMouse(true);

    /* Update default viewport with camera */

    auto viewport = window->createViewport(0, 0, window->getWidth(), window->getHeight());
    viewport->setCamera(camera);
}

void Example::onIdle() {
    const Graphene::KeyboardState& keyStates = this->getWindow()->getKeyboardState();

    if (keyStates[Graphene::KeyboardKey::KEY_ESCAPE]) {
        this->exit(0);
    }

    float correction = this->getFrameTime() * MOVE_SPEED;
    Math::Vec3 forward(this->player->getTarget() * correction);
    Math::Vec3 right(this->player->getRight() * correction);

    if (keyStates[Graphene::KeyboardKey::KEY_W]) {
        this->player->move(forward);
    }

    if (keyStates[Graphene::KeyboardKey::KEY_S]) {
        this->player->move(-forward);
    }

    if (keyStates[Graphene::KeyboardKey::KEY_D]) {
        this->player->move(right);
    }

    if (keyStates[Graphene::KeyboardKey::KEY_A]) {
        this->player->move(-right);
    }
}
