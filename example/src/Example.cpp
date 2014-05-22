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
#include <Vec3.h>

Example::Example(int width, int height):
        Graphene::Engine(width, height) {

    /* Setup scene */

    auto sceneManager = *this->getSceneManagers().begin();
    sceneManager->setAmbientEnergy(0.2f);
    sceneManager->setLightPass(true);

    this->player = sceneManager->createNode();
    sceneManager->getRootNode()->attachNode(this->player);

    auto node = sceneManager->createNode();
    sceneManager->getRootNode()->attachNode(node);

    /* Populate scene with objects */

    auto objectManager = *this->getObjectManagers().begin();

    this->camera = objectManager->createCamera();
    this->player->attachObject(this->camera);

    auto light = objectManager->createLight();
    light->setEnergy(0.5f);
    node->attachObject(light);

    auto entity = objectManager->createEntity("assets/example.entity");
    node->attachObject(entity);

    /* Update default viewport with camera */

    auto viewport = *this->getWindow()->getViewports().begin();
    viewport->setCamera(camera);

    /* Update scene */

    this->player->translate(0.0f, 0.0f, -3.0f);
    light->translate(0.0f, 2.0f, 0.0f);

    /* Keep mouse inside the window */

    this->getWindow()->captureMouse(true);
}

void Example::onMouseMotion(int x, int y) {
    float xAngle = y * MOUSE_SPEED;
    static float roll = 0.0f;

    if (roll + xAngle > 90.0f) {
        xAngle = 90.0f - roll;
    } else if (roll + xAngle < -90.0f) {
        xAngle = -90.0f - roll;
    }
    roll += xAngle;

    this->player->yaw(x * MOUSE_SPEED);
    if (xAngle != 0) {
        this->player->rotate(this->camera->getRight(), xAngle);
    }
}

void Example::onIdle() {
    std::vector<bool> keyStates = this->getWindow()->getKeyboardState();

    if (keyStates[KEY_ESC]) {
        this->exit(0);
    }

    float correction = this->getFrameTime() * MOVE_SPEED;
    Math::Vec3 forward(this->camera->getTarget() * correction);
    Math::Vec3 right(this->camera->getRight() * correction);

    if (keyStates[KEY_W]) {
        this->player->move(forward);
    }

    if (keyStates[KEY_S]) {
        this->player->move(-forward);
    }

    if (keyStates[KEY_D]) {
        this->player->move(right);
    }

    if (keyStates[KEY_A]) {
        this->player->move(-right);
    }
}
