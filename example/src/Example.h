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

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <Engine.h>
#include <Input.h>
#include <SceneNode.h>
#include <Entity.h>
#include <memory>

#define MOVE_SPEED  4.0f
#define MOUSE_SPEED 0.1f

class Example: public Graphene::Engine {
public:
    Example();

private:
    void onMouseMotion(int x, int y);
    void onKeyboardKey(Graphene::KeyboardKey key, bool state);
    void onSetup();
    void onIdle();

    std::shared_ptr<Graphene::SceneNode> player;
    std::shared_ptr<Graphene::SceneNode> crates;

    std::shared_ptr<Graphene::Entity> crate1;
    std::shared_ptr<Graphene::Entity> crate2;
    std::shared_ptr<Graphene::Entity> crate3;
};

#endif  // EXAMPLE_H
