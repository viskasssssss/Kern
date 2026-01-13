#include "vertexlayout.h"
#include "vectors.h"
#include <cstring>

size_t kern::VertexElement::getSize() const {
    switch (type) {
        case VertexElementType::Float:    return sizeof(float);
        case VertexElementType::Float2:   return sizeof(float) * 2;
        case VertexElementType::Float3:   return sizeof(float) * 3;
        case VertexElementType::Float4:   return sizeof(float) * 4;
        default: return 0;
    }
}

size_t kern::VertexElement::getTypeComponentCount() const {
    switch (type) {
        case VertexElementType::Float:    return 1;
        case VertexElementType::Float2:   return 2;
        case VertexElementType::Float3:   return 3;
        case VertexElementType::Float4:   return 4;
        default: return 0;
    }
}

template<>
kern::VertexLayout& kern::VertexLayout::add<kern::Vector2>(const std::string& name) {
    elements.push_back({ name, VertexElementType::Float2, stride, static_cast<int>(elements.size()) });
    stride += sizeof(kern::Vector2);
    return *this;
}

template<>
kern::VertexLayout& kern::VertexLayout::add<kern::Vector3>(const std::string& name) {
    elements.push_back({ name, VertexElementType::Float3, stride, static_cast<int>(elements.size()) });
    stride += sizeof(kern::Vector3);
    return *this;
}

template<>
kern::VertexLayout& kern::VertexLayout::add<float>(const std::string& name) {
    elements.push_back({ name, VertexElementType::Float, stride, static_cast<int>(elements.size()) });
    stride += sizeof(float);
    return *this;
}