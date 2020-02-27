#include "headers/Renderer.hpp"
#include "headers/ErrorChecking.hpp"

#include <cassert>

Renderer::Renderer(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // GLClearError();
    glEnable(GL_DEPTH_TEST);
    // GLCheckError();
}

Renderer::~Renderer(){}

void Renderer::Draw(const VertexArray &VAO, const IndexBuffer &IBO, const Shader &shader) const{
    VAO.Bind();
    IBO.Bind();
    shader.Bind();

    GLClearError();
    glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
    assert(!GLCheckError());
}

void Renderer::Clear() const{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float r, float g, float b, float a) const{
    glClearColor(r,g,b,a);
    // GLClearError();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // GLCheckError();
}