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
        if (key == Graphene::KeyboardKey::KEY_PLUS) {
            this->crate3->yaw(10.0f);
            this->crates->pitch(10.0f);
        }
        if (key == Graphene::KeyboardKey::KEY_MINUS) {
            this->crate3->yaw(-10.0f);
            this->crates->pitch(-10.0f);
        }
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

    this->crate1 = objectManager.createEntity("assets/crate.entity");
    this->crate2 = objectManager.createEntity("assets/crate.entity");
    this->crate3 = objectManager.createEntity("assets/crate.entity");
    auto ground = objectManager.createEntity("assets/ground.entity");
    auto skybox = objectManager.createSkybox("textures/skybox");

    auto flashLight = objectManager.createLight(Graphene::LightType::SPOT);
    auto lightBulb = objectManager.createLight(Graphene::LightType::POINT);
    auto camera = objectManager.createCamera(Graphene::ProjectionType::PERSPECTIVE);

    this->crate2->move(1.25f, 0.0f, 0.0f);
    this->crate3->move(0.5f, 1.0f, 0.0f);
    this->crate3->yaw(30.0f);

    flashLight->setBlend(0.05f);
    flashLight->setAngle(15.0f);
    flashLight->setFalloff(8.0f);
    flashLight->move(0.2f, -0.2f, 0.0f);

    lightBulb->setEnergy(0.8f);
    lightBulb->move(5.0f, 20.0f, 0.0f);

    /* Populate scene with objects */

    auto scene = this->createScene();
    scene->setAmbientEnergy(0.3f);
    scene->setSkybox(skybox);

    this->player = scene->createNode();
    this->player->attachObject(camera);
    this->player->attachObject(flashLight);
    this->player->move(0.0f, 1.0f, -3.0f);

    this->crates = scene->createNode();
    this->crates->attachObject(this->crate1);
    this->crates->attachObject(this->crate2);
    this->crates->attachObject(this->crate3);
    this->crates->attachObject(lightBulb);
    this->crates->move(0.0f, 0.5f, 0.0f);

    auto sceneRoot = scene->getRootNode();
    sceneRoot->attachNode(this->player);
    sceneRoot->attachNode(this->crates);
    sceneRoot->attachObject(ground);

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
