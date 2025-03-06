#pragma once

class IndexBuffer {
private:
	unsigned int rendererID;
	unsigned int count;
public:
	IndexBuffer() : rendererID(0), count(0) {}
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
	inline unsigned int getID() const { return rendererID; }
};

