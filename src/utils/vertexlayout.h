// src/utils/vertexlayout.h
#pragma once
#include <vector>
#include <string>
#include "vectors.h"

namespace kern {

enum class VertexElementType {
    Float, Float2, Float3, Float4
};

struct VertexElement {
    std::string name;
    VertexElementType type;
    size_t offset;
    int index; 

    VertexElement(const std::string& name, VertexElementType type, size_t offset, int index)
        : name(name), type(type), offset(offset), index(index) {}

    size_t getSize() const;
    size_t getTypeComponentCount() const;
};

class VertexLayout {
public:
    template<typename T>
    VertexLayout& add(const std::string& name);

    const std::vector<VertexElement>& getElements() const { return elements; }
    size_t getStride() const { return stride; }

private:
    std::vector<VertexElement> elements;
    size_t stride = 0;
};

// Helper to register common types
template<> VertexLayout& VertexLayout::add<Vector2>(const std::string& name);
template<> VertexLayout& VertexLayout::add<float>(const std::string& name);

} // namespace kern
