#include "headers/VertexArray.hpp"

VertexArray::VertexArray(){
    glGenVertexArrays(1,&m_RendererID);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind(){
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind(){
    glBindVertexArray(0);
}

const void VertexArray::AddBuffer(const VertexBuffer &vb,VertexBufferLayout& layout){
    Bind();
    vb.Bind();

    const std::vector<LayoutElement> &elements = layout.GetElements();
    unsigned long offset = 0;

    for(int i = 0; i < elements.size(); i++){
        const LayoutElement& e = elements[i];
        glVertexAttribPointer(i,e.count, e.type, (e.normalized)?GL_TRUE:GL_FALSE, layout.GetStride(), (const void*)(offset)); //Sets what attributes are where and how big they are
        glEnableVertexAttribArray(i);

        offset += e.count * LayoutElement::GetTypeSize(e.type);
    }
}