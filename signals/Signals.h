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

#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include <vector>

namespace Signals {

template<typename... Types>
class Slot {
public:
    Slot(const std::function<void(Types...)>& callable):
            callable(callable) {
    }

    Slot& operator =(const std::function<void(Types...)>& callable) {
        this->callable = callable;
    }

    void operator() (Types... args) {
        this->callable(args...);
    }

private:
    std::function<void(Types...)> callable;
};

template<typename... types>
class Signal {
public:
    void operator() (types... args) {
        for (auto& callable: this->callables) {
            callable(args...);
        }
    }

    int connect(const std::function<void(types...)>& callable) {
        this->callables.push_back(Slot<types...>(callable));
        return this->callables.size() - 1;
    }

    void disconnect(int handle) {
        this->callables.erase(this->callables.begin() + handle);
    }

    void disconnectAll() {
        this->callables.clear();
    }

private:
    std::vector<Slot<types...>> callables;
};

}  // namespace Signals

#endif  // SIGNALS_H
