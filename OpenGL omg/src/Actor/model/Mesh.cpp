#include "Mesh.h"

Mesh::Mesh(vector<Vertex>vVertices, vector<unsigned int> vIndices, vector<Texture> vTextures)
{
	vertices = vVertices;
	indices = vIndices;
	textures = vTextures;

	//std::cout << "T:  " << textures[0].id << "  T: " << textures[1].id << std::endl;
	if (textures.size() > 16) textures.resize(16);

	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == "texture_diffuse") diffuseTC++;
		else if (textures[i].type == "texture_specular") specularTC++;
		else if (textures[i].type == "texture_emission") emissionTC++;
		else textures.erase(textures.begin() + i);
	}

	createMesh();
}

void Mesh::setupTexture(Shader& shader)
{
	size_t diff = 1, spec = 1, emis = 1;

	for (size_t i = 0; i < textures.size(); i++) {
		string number, name = textures[i].type;
		if (name == "texture_diffuse") number = std::to_string(diff++);
		if (name == "texture_specular") number = std::to_string(spec++);
		if (name == "texture_emission") number = std::to_string(emis++);

		shader.setUniform1i("u_material." + name + number, i);
	}

	bNeedToChangeTex = false;
}

void Mesh::draw(Shader &shader)
{
	if (bNeedToChangeTex) setupTexture(shader);

	for (size_t i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id); // Vincular texturas
	}

	glActiveTexture(GL_TEXTURE0);
	
	if (textures.size() == 0) shader.setUniform1i("u_material.bIsActive", false);
	else shader.setUniform1i("u_material.bIsActive", true);
	
	/*VAO.bind();
	EBO.bind();*/

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	/*VAO.unbind();
	EBO.unbind(); */
}

void Mesh::createMesh()
{
	if (vertices.empty()) {
		std::cerr << "ERROR: Vertices is empty." << std::endl;
		return;
	}
	if (indices.empty()) {
		std::cerr << "ERROR: Indices is empty." << std::endl;
		return;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);

	/*
	VBO = VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	EBO = IndexBuffer(&indices[0], indices.size());

	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);

	VAO.addBuffer(VBO, layout);
	VAO.unbind();
	VBO.unbind();
	EBO.unbind();
	*/
}

/*

Plan mensual de gastos para independizacion Anthonella y Dydier :

Este plan esta construido a base de unas cifras exageradas, el valor real va a ser mostrado frente a su exageracion,
tomando en cuenta un maximo a gastar el cual no va a ser alcanzado en la mayoria de los casos :

Disposicion en Salario :
2 Smmv 2024 (1.300.000 * 2 = 2.600.000)

Arriendo :
	Rango estimado : 700.000 - 1.000.000 (el mayor precio a pagar sera tomado)

	Alimentacion :
		Dato real (gasto en alimentacion actual en la casa de Antho) : 200.000
		Exageracion : real * 3 = 600.000

	Servicios :
		-Basicos :
		Se incluye internet, gas, agua y luz
			Dato casa Karina : 130.000
			Dato casa Antho : 300.000 (el mayor sera tomado)
		- Aledaños :
			Transporte :
			Suponiendo que Antho y Dydier gasten 3 pasajes (P) cada uno (I) todos los dias del mes (D)
			(en caso de haber cifras luego del . seran tomadas como irrelevantes)
				Precio del Pasaje : 3.000
				Exageracion : (P * I * D * 3) = (3.000 * 2 * 31 * 3)
				Total : 560.000
			Universidad :
				Dato real : 2.000.000 el semestre dejando con un gasto de 340.000 aprox (S / 6)
				Exageracion : se infla el precio un 1.2% dejando 2.400.000 / 6 = 400.000
				Total : 400.000
			Datos Moviles :
			Se toma el plan de datos pospago mas barato de Movistar como referencia
				Dato real : 42.000
				Exageracion : 45.000 * 2 (plan para ambos)
				Total : 90.000
			Total : 1.050.000
		Total : 1.350.000
	Total: 1.000.000 + 600.000 + 1.350.000 = 2.950.000

											Deficit: 350.000

*/