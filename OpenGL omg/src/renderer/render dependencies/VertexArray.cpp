#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rendererID);
	std::cout << "VAO: " << rendererID << std::endl;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();

	for (const auto& element : layout.getElements()) {
		std::cout << "Type: " << element.type
			<< ", Count: " << element.count
			<< ", Normalized: " << element.normalized
			<< ", Stride: " << layout.getStride()
			<< std::endl;
	}

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		std::cout << i << ": " << offset << std::endl;
		offset += element.count * getTypeSize(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(rendererID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
