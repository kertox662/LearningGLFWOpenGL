#ifndef VERTEXBUFFERLAYOUT_H //VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include "GLCommon.hpp"
#include <cassert>

struct LayoutElement{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetTypeSize(unsigned int type){
        switch(type){
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        return 0;
    }
};

class VertexBufferLayout{
private:
    std::vector<LayoutElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout():m_Stride(0){

    }
    ~VertexBufferLayout(){}

    template<typename T>
    void Push(int count){
        assert(false);
    }

    const std::vector<LayoutElement>& GetElements(){return m_Elements;}
    inline unsigned int GetStride(){return m_Stride;}

};

template<>
inline void VertexBufferLayout::Push<float>(int count){
    LayoutElement le = {GL_FLOAT, count, GL_FALSE};
    m_Elements.push_back(le);
    m_Stride += sizeof(GLfloat) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(int count){
    LayoutElement le = {GL_UNSIGNED_INT, count, GL_TRUE};
    m_Elements.push_back(le);
    m_Stride += sizeof(GLuint) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(int count){
    LayoutElement le = {GL_UNSIGNED_BYTE, count, GL_TRUE};
    m_Elements.push_back(le);
    m_Stride += sizeof(GLbyte) * count;
}

#endif //VERTEXBUFFERLAYOUT_H