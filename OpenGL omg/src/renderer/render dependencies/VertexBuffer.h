#pragma once
#include <GL/glew.h>

class VertexBuffer {
private:
	unsigned int rendererID;
public:
	VertexBuffer() : rendererID(0) {};
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getID() const { return rendererID; }
};

