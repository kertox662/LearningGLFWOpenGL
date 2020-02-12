#ifndef VERTEXARRAY_H //VERTEXARRAY_H
#define VERTEXARRAY_H

#include "GLCommon.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    void Bind() const;
    void Unbind() const;

    const void AddBuffer(const VertexBuffer &vb,VertexBufferLayout &layout);
};

#endif //VERTEXARRAY_H