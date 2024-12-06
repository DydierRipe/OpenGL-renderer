#pragma once
#include "CoordAttributes.h"
#include <string>
#include <vector>
#include <array>

#include <fstream>

class ModelProcessor
{
public:
	struct TBVertexValues {
		std::vector<float> coords;
		std::vector<int> index;
	};

	struct VertexValues {
		std::vector<float> coords;
		std::vector<std::array<float, 3>> normal;
		std::vector<std::array<float, 4>> color;
		std::vector<std::array<float, 2>> texture;
		std::vector<int> index;

		int vertexNum = 0;
	};

	template< typename T >
	static size_t insertSorted(std::vector<T>& vec, T const& item) {
		auto it = vec.insert
		(
			std::upper_bound(vec.begin(), vec.end(), item),
			item
		);

		return std::distance(vec.begin(), it);
	}

	template <typename T>
	static int binarySearch(std::vector<T> arr, int low, int high, T x)
	{
		while (low <= high) {
			int mid = low + (high - low) / 2;

			if (arr.at(mid) == x) return mid;

			if (arr.at(mid) < x) low = mid + 1;

			else high = mid - 1;
		}
		return -1;
	}

	template <typename T>
	static std::vector<T> getDividedString(std::vector<T> arr, std::string line, std::string divChar = ",", std::string type = "int") {
		size_t start = 0, end = 0;

		while ((end = line.find(divChar, start)) != std::string::npos) {
			std::string val = line.substr(start, end - start);

			if (type == "int") arr.push_back(std::stoi(val));
			else if (type == "float") arr.push_back(std::stof(val));

			start = end + 1;
		}

		if (start < line.size()) {
			if (type == "int") arr.push_back(std::stoi(line.substr(start)));
			else if (type == "float") arr.push_back(std::stof(line.substr(start)));
		}

		return arr;
	}

	template <size_t N>
	static void formatFileDataIntoArray(std::vector<std::array<float, N>>& col, int vertexNum, std::string line) {
		col.resize(vertexNum);

		std::string num, color;
		size_t pos = line.find(':');

		if (pos != std::string::npos) {
			num = line.substr(0, pos);
			color = line.substr(pos + 1);
		}
		else std::cout << "No : in \"" << line << "\"" << std::endl;

		std::vector<int> indices = getDividedString(indices, num);
		std::vector<float> values = getDividedString(values, color, ",", "float");
		std::array<float, N> v;

		for (int i = 0; i < N; i++) v[i] = values.at(i);

		for (int i = 0; i < indices.size(); i++) col[indices.at(i)] = v;
	}

	static glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 edge2 = v3 - v1;
		return glm::normalize(glm::cross(edge1, edge2));
	}

	static std::vector<glm::vec3> getNormal(size_t vertNum, std::vector<float> coords, std::vector<int> ind) {
		std::vector<glm::vec3> normals(vertNum, glm::vec3(0));

		for (size_t i = 0; i < ind.size(); i += 3)
		{
			glm::vec3 v1 = glm::vec3(coords[ind[i] * 3], coords[ind[i] * 3 + 1], coords[ind[i] * 3 + 2]);
			glm::vec3 v2 = glm::vec3(coords[ind[i + 1] * 3], coords[ind[i + 1] * 3 + 1], coords[ind[i + 1] * 3 + 2]);
			glm::vec3 v3 = glm::vec3(coords[ind[i + 2] * 3], coords[ind[i + 2] * 3 + 1], coords[ind[i + 2] * 3 + 2]);

			glm::vec3 normal = calcNormal(v1, v2, v3);

			normals[ind[i]] += normal;
			normals[ind[i + 1]] += normal;
			normals[ind[i + 2]] += normal;
		}

		for (auto& normal : normals) normal = glm::normalize(normal);

		return normals;
	}

	static VertexValues processData(std::string path) {
		VertexValues vValues;

		std::ifstream stream(path);

		enum class DataType {
			NONE = -1, VCOORDS = 0, TCOORDS = 1, INDICES = 2, NORMAL = 3
		};

		int vertexNum = 0;
		std::string line;
		DataType type = DataType::NONE;

		while (getline(stream, line)) {
			if (line.find("#Model") != std::string::npos) {
				if (line.find("vertex-coords") != std::string::npos) type = DataType::VCOORDS;

				else if (line.find("tex-coords") != std::string::npos) type = DataType::TCOORDS;

				else if (line.find("indices") != std::string::npos) type = DataType::INDICES;

				else if (line.find("normal") != std::string::npos) type = DataType::NORMAL;
			}
			else if (!line.empty()) {
				switch ((int)type)
				{
				case 0:
					vValues.coords = getDividedString(vValues.coords, line, ",", "float");
					vertexNum = vValues.coords.size() / 3;
					break;
				case 1: formatFileDataIntoArray(vValues.texture, vertexNum, line);
					break;
				case 2: vValues.index = getDividedString(vValues.index, line);
					break;
				case 3: formatFileDataIntoArray(vValues.normal, vertexNum, line);
				}
			}
		}
		vValues.vertexNum = vertexNum;

		return vValues;
	}

	static TBVertexValues arrayData(std::string path, float texNum, float indexNum, bool softenNormal) {
		VertexValues vv = processData(path);

		std::vector<float> arr;
		std::vector<int> index;
		std::vector<glm::vec3> normals;

		if (softenNormal) normals = getNormal(vv.vertexNum, vv.coords, vv.index);

		for (int i = 0; i < vv.vertexNum; i++) {
			for (int j = 0; j < 3; j++) arr.push_back(vv.coords.at(i * 3 + j));

			if (softenNormal) {
				arr.push_back(normals.at(i)[0]);
				arr.push_back(normals.at(i)[1]);
				arr.push_back(normals.at(i)[2]);
			}
			else {
				for (int j = 0; j < 3; j++) arr.push_back(vv.normal.at(i).at(j));
			}

			for (int j = 0; j < 2; j++) arr.push_back(vv.texture.at(i).at(j));

			arr.push_back(texNum);
			arr.push_back(indexNum);
		}

		for (int i = 0; i < vv.index.size(); i++) index.push_back(vv.index.at(i));

		return { arr, index };
	}
};


