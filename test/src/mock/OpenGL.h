/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software, to deal
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

#ifndef OPENGL_H
#define OPENGL_H

#define GLuint                      unsigned int

#define GL_UNIFORM_BUFFER           0
#define GL_DYNAMIC_DRAW             0
#define GL_TRIANGLES                0
#define GL_TEXTURE_2D               0
#define GL_UNSIGNED_INT             0
#define GL_UNSIGNED_BYTE            0
#define GL_RGBA                     0
#define GL_RGBA16F                  0
#define GL_LINEAR                   0
#define GL_LINEAR_MIPMAP_LINEAR     0
#define GL_TEXTURE_MIN_FILTER       0
#define GL_TEXTURE_MAG_FILTER       0
#define GL_TEXTURE0                 0

#define glGenBuffers(...)           mock(__VA_ARGS__)
#define glDeleteBuffers(...)        mock(__VA_ARGS__)
#define glGenTextures(...)          mock(__VA_ARGS__)
#define glDeleteTextures(...)       mock(__VA_ARGS__)
#define glActiveTexture(...)        mock(__VA_ARGS__)
#define glDeleteVertexArrays(...)   mock(__VA_ARGS__)
#define glBindVertexArray(...)      mock(__VA_ARGS__)
#define glBindBuffer(...)           mock(__VA_ARGS__)
#define glBindBufferBase(...)       mock(__VA_ARGS__)
#define glBufferData(...)           mock(__VA_ARGS__)
#define glBufferSubData(...)        mock(__VA_ARGS__)
#define glDrawElements(...)         mock(__VA_ARGS__)
#define glBindTexture(...)          mock(__VA_ARGS__)
#define glTexParameteri(...)        mock(__VA_ARGS__)
#define glGenerateMipmap(...)       mock(__VA_ARGS__)
#define glTexStorage2D(...)         mock(__VA_ARGS__)

template <typename T>
void mock(T arg) { (void)arg; }

template <typename T, typename... Args>
void mock(T arg, Args... args) { mock(arg); mock(args...); }

#endif  // OPENGL_H
