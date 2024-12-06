#pragma once

class VertexBuffer {
private:
	unsigned int rendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getID() { return rendererID; }
};

