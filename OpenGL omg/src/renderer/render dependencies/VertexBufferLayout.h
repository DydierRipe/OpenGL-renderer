#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

struct  LayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	LayoutElement(unsigned int t, unsigned int c, unsigned char n) :
		count(c), type(t), normalized(n) { }
};

static unsigned int getTypeSize(GLenum type) {
	switch (type) {
	case GL_FLOAT: return sizeof(float);
	case GL_UNSIGNED_INT: return sizeof(unsigned int);
	case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
	default: return 0; // Maneja posibles errores
	}
}

class VertexBufferLayout
{
private:
	std::vector<LayoutElement> vBElements;
	unsigned int stride;
public:
	VertexBufferLayout() : stride(0) {}

	template<typename T>
	void push(int count) {
		static_assert(false);
	}

	template<>
	void push<float>(int count) {
		vBElements.push_back(LayoutElement({ GL_FLOAT, (unsigned int)count, GL_FALSE }));
		stride += sizeof(float) * count;
	}

	template<>
	void push<unsigned int>(int count) {
		vBElements.push_back(LayoutElement({ GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE }));
		stride += sizeof(unsigned int) * count;
	}

	template<>
	void push<unsigned char>(int count) {
		vBElements.push_back(LayoutElement({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_TRUE }));
		stride += sizeof(unsigned char) * count;
	}

	inline const std::vector<LayoutElement> getElements() const& { return vBElements; }
	inline unsigned int getStride() const { return stride; }
};
