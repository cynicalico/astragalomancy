#pragma once

#include "gloo/gl.hpp"

#include <memory>

namespace gloo {
class VertexArray {
    friend class VertexArrayBuilder;

    explicit VertexArray(GLuint id);

public:
    GLuint id{0};

    ~VertexArray();

    VertexArray(const VertexArray &other) = delete;
    VertexArray &operator=(const VertexArray &other) = delete;

    VertexArray(VertexArray &&other) noexcept;
    VertexArray &operator=(VertexArray &&other) noexcept;

    void bind() const;
    void unbind() const;
};

class VertexArrayBuilder {
public:
    VertexArrayBuilder();
    ~VertexArrayBuilder();

    VertexArrayBuilder(const VertexArrayBuilder &other) = delete;
    VertexArrayBuilder &operator=(const VertexArrayBuilder &other) = delete;

    VertexArrayBuilder(VertexArrayBuilder &&other) noexcept = delete;
    VertexArrayBuilder &operator=(VertexArrayBuilder &&other) noexcept = delete;

    VertexArrayBuilder &
    attrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint offset, GLuint binding_index);

    std::unique_ptr<VertexArray> build();

private:
    GLuint id_;
};
} // namespace gloo
