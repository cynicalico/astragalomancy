#include "gloo/vertex_array.hpp"

#include "astra/core/log.hpp"

gloo::VertexArray::VertexArray(const GLuint id)
    : id(id) {}

gloo::VertexArray::~VertexArray() {
    if (id != 0) {
        glDeleteVertexArrays(1, &id);
        ASTRA_LOG_TRACE("Deleted vertex array (id={})", id);
    }
}

gloo::VertexArray::VertexArray(VertexArray &&other) noexcept
    : id(other.id) {
    other.id = 0;
}

gloo::VertexArray &gloo::VertexArray::operator=(VertexArray &&other) noexcept {
    if (this != &other) {
        id = other.id;
        other.id = 0;
    }
    return *this;
}

void gloo::VertexArray::bind() const {
    glBindVertexArray(id);
}

void gloo::VertexArray::unbind() const {
    glBindVertexArray(0);
}

gloo::VertexArrayBuilder::VertexArrayBuilder() {
    glCreateVertexArrays(1, &id_);
    ASTRA_LOG_TRACE("Created vertex array (id={})", id_);
}

gloo::VertexArrayBuilder::~VertexArrayBuilder() {
    if (id_ != 0) {
        glDeleteVertexArrays(1, &id_);
        ASTRA_LOG_TRACE("Deleted unfinished vertex array (id={})", id_);
    }
}

gloo::VertexArrayBuilder &gloo::VertexArrayBuilder::attrib(
        const GLuint index,
        const GLint size,
        const GLenum type,
        const GLboolean normalized,
        const GLuint offset,
        const GLuint binding_index) {
    glEnableVertexArrayAttrib(id_, index);
    glVertexArrayAttribFormat(id_, index, size, type, normalized, offset);
    glVertexArrayAttribBinding(id_, index, binding_index);
    return *this;
}

std::unique_ptr<gloo::VertexArray> gloo::VertexArrayBuilder::build() {
    auto vao = std::unique_ptr<VertexArray>(new VertexArray(id_));
    id_ = 0; // don't delete the vao when we go out of scope
    return vao;
}
