#ifndef RENDERER_H //RENDERER_H
#define RENDERER_H

#include "GLCommon.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer{
private:

public:
    Renderer();
    ~Renderer();

    void Draw(const VertexArray &VAO, const IndexBuffer &IBO, const Shader &shader) const;
    void Clear() const;
    void Clear(float r, float g, float b, float a) const;

};

#endif //RENDERER_H