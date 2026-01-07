#pragma once

#include "astra/core/log.hpp"
#include "gloo/gl.hpp"

#include <cstdint>
#include <vector>

namespace gloo {
enum class BufferFillDirection {
    Forward,
    Backward,
};

template<typename T>
class Buffer {
public:
    GLuint id{0};

    explicit Buffer(std::size_t capacity, BufferFillDirection fill_direction = BufferFillDirection::Forward);
    ~Buffer();

    Buffer(const Buffer &other) = delete;
    Buffer &operator=(const Buffer &other) = delete;

    Buffer(Buffer &&other) noexcept;
    Buffer &operator=(Buffer &&other) noexcept;

    void clear();
    bool has_capacity(std::size_t count) const;
    bool add(const std::vector<T> &data);

    void sync();
    GLintptr front() const;
    GLsizei size() const;

    void bind(GLuint binding_index, GLintptr offset, GLsizei stride) const;
    void unbind(GLuint binding_index) const;

private:
    std::vector<T> data_;
    std::size_t data_pos_;
    BufferFillDirection fill_direction_;
    GLsizeiptr buf_pos_;
};
} // namespace gloo

template<typename T>
gloo::Buffer<T>::Buffer(std::size_t capacity, BufferFillDirection fill_direction)
    : data_(capacity),
      data_pos_(fill_direction == BufferFillDirection::Forward ? 0 : capacity),
      fill_direction_(fill_direction),
      buf_pos_(data_pos_) {
    glCreateBuffers(1, &id);
    ASTRA_LOG_TRACE("Created buffer (id={})", id);
    glNamedBufferStorage(id, data_.size() * sizeof(T), data_.data(), GL_DYNAMIC_STORAGE_BIT);
}

template<typename T>
gloo::Buffer<T>::~Buffer() {
    if (id != 0) {
        glDeleteBuffers(1, &id);
        ASTRA_LOG_TRACE("Deleted buffer (id={})", id);
    }
}

template<typename T>
gloo::Buffer<T>::Buffer(Buffer &&other) noexcept
    : id(other.id),
      data_(std::move(other.data_)),
      data_pos_(other.data_pos_),
      fill_direction_(other.fill_direction_),
      buf_pos_(other.buf_pos_) {
    other.id = 0;
}

template<typename T>
gloo::Buffer<T> &gloo::Buffer<T>::operator=(Buffer &&other) noexcept {
    if (this != &other) {
        id = other.id;
        data_ = std::move(other.data_);
        data_pos_ = other.data_pos_;
        fill_direction_ = other.fill_direction_;
        buf_pos_ = other.buf_pos_;
        other.id = 0;
    }
    return *this;
}

template<typename T>
void gloo::Buffer<T>::clear() {
    data_pos_ = fill_direction_ == BufferFillDirection::Forward ? 0 : data_.size();
    buf_pos_ = data_pos_;
}

template<typename T>
bool gloo::Buffer<T>::has_capacity(std::size_t count) const {
    switch (fill_direction_) {
    case BufferFillDirection::Forward: return data_pos_ + count <= data_.size();
    case BufferFillDirection::Backward: return data_pos_ >= count;
    default: std::unreachable();
    }
}

template<typename T>
bool gloo::Buffer<T>::add(const std::vector<T> &data) {
    switch (fill_direction_) {
    case BufferFillDirection::Forward:
        if (!has_capacity(data.size())) return false;
        std::copy(data.begin(), data.end(), data_.begin() + data_pos_);
        data_pos_ += data.size();
        return true;
    case BufferFillDirection::Backward:
        if (!has_capacity(data.size())) return false;
        data_pos_ -= data.size();
        std::copy(data.begin(), data.end(), data_.begin() + data_pos_);
        return true;
    default: std::unreachable();
    }
}

template<typename T>
void gloo::Buffer<T>::sync() {
    if (buf_pos_ == data_pos_) return;

    switch (fill_direction_) {
    case BufferFillDirection::Forward:
        glNamedBufferSubData(id, buf_pos_ * sizeof(T), (data_pos_ - buf_pos_) * sizeof(T), data_.data() + buf_pos_);
        break;
    case BufferFillDirection::Backward:
        glNamedBufferSubData(id, data_pos_ * sizeof(T), (buf_pos_ - data_pos_) * sizeof(T), data_.data() + data_pos_);
        break;
    default: std::unreachable();
    }
    buf_pos_ = data_pos_;
}

template<typename T>
GLintptr gloo::Buffer<T>::front() const {
    switch (fill_direction_) {
    case BufferFillDirection::Forward: return 0;
    case BufferFillDirection::Backward: return buf_pos_;
    default: std::unreachable();
    }
}

template<typename T>
GLsizei gloo::Buffer<T>::size() const {
    switch (fill_direction_) {
    case BufferFillDirection::Forward: return buf_pos_;
    case BufferFillDirection::Backward: return data_.size() - buf_pos_;
    default: std::unreachable();
    }
}

template<typename T>
void gloo::Buffer<T>::bind(const GLuint binding_index, const GLintptr offset, const GLsizei stride) const {
    glBindVertexBuffer(binding_index, id, offset, stride);
}

template<typename T>
void gloo::Buffer<T>::unbind(GLuint binding_index) const {
    glBindVertexBuffer(binding_index, 0, 0, 0);
}
