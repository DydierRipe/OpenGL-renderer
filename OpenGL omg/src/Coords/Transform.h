#pragma once
#include "CoordAttributes.h"

class Transform
{
public:
	Transform(Position vPosition, Rotation vRotation, Size vSize) : position(vPosition), rotation(vRotation), size(vSize), bNeedSize(true) {
		setTransform();
	}

	Transform(Position vPosition, Rotation vRotation) : position(vPosition), rotation(vRotation), size(0.0f), bNeedSize(false) {
		setTransform();
	}

	~Transform() {};

	inline Position getPosition() const { return position; }
	inline Rotation getRotation() const{ return rotation; }
	inline Size getSize() const { return size; }
	inline glm::vec3 getVectorPosition() const { return glm::vec3(position.x, position.y, position.z); }
	inline glm::vec3 getVectorRotation() const { return glm::vec3(rotation.yaw, rotation.pitch, rotation.roll); }
	inline glm::vec3 getVectorSize() const { return glm::vec3(size.x, size.y, size.z); }
	inline glm::mat4 getTransformMatrix() const { return transformMat; }

	void setPosition(float x, float y, float z) {
		position = Position(x, y, z);
		setTransform();
	}

	void setPosition(Position pos) {
		position = pos;
		setTransform();
	}

	inline void setPositionx(float x) { setPosition(x, position.y, position.z); }
	inline void setPositiony(float y) { setPosition(position.x, y, position.z); }
	inline void setPositionz(float z) { setPosition(position.x, position.y, z); }

	inline void setPositionxy(float x, float y) { setPosition(x, y, position.z); }
	inline void setPositionyz(float y, float z) { setPosition(position.x, y, z); }
	inline void setPositionxz(float x, float z) { setPosition(x, position.y, z); }

	void setRotation(float yaw, float pitch, float roll) {
		rotation = Rotation(yaw, pitch, roll);
		setTransform();
	}

	void setRotation(Rotation rot) {
		rotation = rot;
		setTransform();
	}

	inline void setRotationr(float r) { setRotation(rotation.yaw, rotation.pitch, r); }
	inline void setRotationp(float p) { setRotation(rotation.yaw, p, rotation.roll); }
	inline void setRotationy(float y) { setRotation(y, rotation.pitch, rotation.roll); }

	inline void setRotationpr(float p, float r) { setRotation(rotation.yaw, p, r); }
	inline void setRotationyp(float y, float p) { setRotation(y, p, rotation.roll); }
	inline void setRotationry(float y, float r) { setRotation(r, rotation.pitch, y); }

	void setSize(float x, float y, float z) {
		size = Size(x, y, z);
		setTransform();
	}

	void setSize(Size siz) {
		size = siz;
		setTransform();
	}

	inline void setSizex(float x) { setSize(x, size.y, size.z); }
	inline void setSizey(float y) { setSize(size.x, y, size.z); }
	inline void setSizez(float z) { setSize(size.x, size.y, z); }

	inline void setSizexy(float x, float y) { setSize(x, y, size.z); }
	inline void setSizeyz(float y, float z) { setSize(size.x, y, z); }
	inline void setSizexz(float x, float z) { setSize(x, size.y, z); }

	static glm::mat4 getRotateMatrix(Rotation rot) {
		glm::mat4 finRot = glm::mat4(1.0f);

		finRot = glm::rotate(finRot, glm::radians(rot.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		finRot = glm::rotate(finRot, glm::radians(rot.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		finRot = glm::rotate(finRot, glm::radians(rot.roll), glm::vec3(0.0f, 0.0f, 1.0f));

		return finRot;
	}

private:

	bool bNeedSize;

	Position position;
	Rotation rotation;
	Size size;

	glm::mat4 transformMat;

	void setTransform() {
		glm::mat4 transTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));

		glm::mat4 transRot = getRotateMatrix(rotation);

		if (bNeedSize) {
			glm::mat4 transSize = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));
			transformMat = transTranslate * transRot * transSize;
		} else transformMat = transTranslate * transRot;
	}

};
