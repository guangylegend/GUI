#ifndef VECTOR3D_H
#define VECTOR3D_H

class CVector3
{
public:
	// ��ʼ��
	CVector3(void) : x(0.0f), y(0.0f), z(0.0f) {}
	CVector3(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ) {}
	CVector3(const float * rhs)	: x(*rhs), y(*(rhs+1)), z(*(rhs+2)) {}
	CVector3(const CVector3 & rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	// �����������
	~CVector3() {}

	// ������������������
	void Set(float newX, float newY, float newZ);

	// ���������Ĳ��
	CVector3 CrossProduct(const CVector3 &rhs) const;

	// ���������ĵ��
	float DotProduct(const CVector3 &rhs) const;

	// ������λ��
	void Normalize();
	CVector3 GetNormalized() const;

	// ���������ĳ���
	float GetLength() const;

	// ���������ĳ��ȵ�ƽ��
	float GetSquaredLength() const;

	// ��������ת
	void RotateX(double angle);
	CVector3 GetRotatedX(double angle) const;
	void RotateY(double angle);
	CVector3 GetRotatedY(double angle) const;
	void RotateZ(double angle);
	CVector3 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const CVector3 &axis);
	CVector3 GetRotatedAxis(double angle, const CVector3 &axis) const;

	// ���������
	CVector3 operator+(const CVector3 &rhs) const;
	CVector3 operator-(const CVector3 &rhs) const;
	CVector3 operator*(const float rhs) const;
	CVector3 operator/(const float rhs) const;

	bool operator==(const CVector3 &rhs) const;
	bool operator!=(const CVector3 &rhs) const;

	// ���Ա
	float x;
	float y;
	float z;
};

#endif			// CVector3_H