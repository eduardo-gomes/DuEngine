#include "glmath.hpp"

#include <cmath>
mat4f mat4f::operator*(const mat4f& right) {
	mat4f ret, &left = *this;

	ret.matrix[0][0] = right.matrix[0][0] * left.matrix[0][0] + right.matrix[0][1] * left.matrix[1][0] + right.matrix[0][2] * left.matrix[2][0] + right.matrix[0][3] * left.matrix[3][0];
	ret.matrix[0][1] = right.matrix[0][0] * left.matrix[0][1] + right.matrix[0][1] * left.matrix[1][1] + right.matrix[0][2] * left.matrix[2][1] + right.matrix[0][3] * left.matrix[3][1];
	ret.matrix[0][2] = right.matrix[0][0] * left.matrix[0][2] + right.matrix[0][1] * left.matrix[1][2] + right.matrix[0][2] * left.matrix[2][2] + right.matrix[0][3] * left.matrix[3][2];
	ret.matrix[0][3] = right.matrix[0][0] * left.matrix[0][3] + right.matrix[0][1] * left.matrix[1][3] + right.matrix[0][2] * left.matrix[2][3] + right.matrix[0][3] * left.matrix[3][3];

	ret.matrix[1][0] = right.matrix[1][0] * left.matrix[0][0] + right.matrix[1][1] * left.matrix[1][0] + right.matrix[1][2] * left.matrix[2][0] + right.matrix[1][3] * left.matrix[3][0];
	ret.matrix[1][1] = right.matrix[1][0] * left.matrix[0][1] + right.matrix[1][1] * left.matrix[1][1] + right.matrix[1][2] * left.matrix[2][1] + right.matrix[1][3] * left.matrix[3][1];
	ret.matrix[1][2] = right.matrix[1][0] * left.matrix[0][2] + right.matrix[1][1] * left.matrix[1][2] + right.matrix[1][2] * left.matrix[2][2] + right.matrix[1][3] * left.matrix[3][2];
	ret.matrix[1][3] = right.matrix[1][0] * left.matrix[0][3] + right.matrix[1][1] * left.matrix[1][3] + right.matrix[1][2] * left.matrix[2][3] + right.matrix[1][3] * left.matrix[3][3];

	ret.matrix[2][0] = right.matrix[2][0] * left.matrix[0][0] + right.matrix[2][1] * left.matrix[1][0] + right.matrix[2][2] * left.matrix[2][0] + right.matrix[2][3] * left.matrix[3][0];
	ret.matrix[2][1] = right.matrix[2][0] * left.matrix[0][1] + right.matrix[2][1] * left.matrix[1][1] + right.matrix[2][2] * left.matrix[2][1] + right.matrix[2][3] * left.matrix[3][1];
	ret.matrix[2][2] = right.matrix[2][0] * left.matrix[0][2] + right.matrix[2][1] * left.matrix[1][2] + right.matrix[2][2] * left.matrix[2][2] + right.matrix[2][3] * left.matrix[3][2];
	ret.matrix[2][3] = right.matrix[2][0] * left.matrix[0][3] + right.matrix[2][1] * left.matrix[1][3] + right.matrix[2][2] * left.matrix[2][3] + right.matrix[2][3] * left.matrix[3][3];

	ret.matrix[3][0] = right.matrix[3][0] * left.matrix[0][0] + right.matrix[3][1] * left.matrix[1][0] + right.matrix[3][2] * left.matrix[2][0] + right.matrix[3][3] * left.matrix[3][0];
	ret.matrix[3][1] = right.matrix[3][0] * left.matrix[0][1] + right.matrix[3][1] * left.matrix[1][1] + right.matrix[3][2] * left.matrix[2][1] + right.matrix[3][3] * left.matrix[3][1];
	ret.matrix[3][2] = right.matrix[3][0] * left.matrix[0][2] + right.matrix[3][1] * left.matrix[1][2] + right.matrix[3][2] * left.matrix[2][2] + right.matrix[3][3] * left.matrix[3][2];
	ret.matrix[3][3] = right.matrix[3][0] * left.matrix[0][3] + right.matrix[3][1] * left.matrix[1][3] + right.matrix[3][2] * left.matrix[2][3] + right.matrix[3][3] * left.matrix[3][3];
	return ret;
}

mat4f mat4f::GenPerspective(float fovy, float aspect, float zNear, float zFar) {
	float f = 1 / tanf(M_PI / 180 * fovy / 2);
	//float ymax = zNear * f, xmax = ymax * aspect, xlen = 2 * xmax, ylen = 2* ymax;
	mat4f mat;
	mat.matrix[0][0] = f / aspect;
	mat.matrix[0][1] = 0.0f;
	mat.matrix[0][2] = 0.0f;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = 0.0f;
	mat.matrix[1][1] = f;
	mat.matrix[1][2] = 0.0f;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = 0.0f;
	mat.matrix[2][1] = 0.0f;
	mat.matrix[2][2] = (zFar + zNear) / (zNear - zFar);
	mat.matrix[2][3] = -1.0f;

	mat.matrix[3][0] = 0.0f;
	mat.matrix[3][1] = 0.0;
	mat.matrix[3][2] = 2 * zFar * zNear / (zNear - zFar);
	mat.matrix[3][3] = 0.0f;
	return mat;
}
mat4f& mat4f::GenPerspective(mat4f& mat, float fovy, float aspect, float zNear, float zFar){
	float f = 1 / tanf(M_PI / 180 * fovy / 2);
	//float ymax = zNear * f, xmax = ymax * aspect, xlen = 2 * xmax, ylen = 2* ymax;
	mat.matrix[0][0] = f / aspect;
	mat.matrix[0][1] = 0.0f;
	mat.matrix[0][2] = 0.0f;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = 0.0f;
	mat.matrix[1][1] = f;
	mat.matrix[1][2] = 0.0f;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = 0.0f;
	mat.matrix[2][1] = 0.0f;
	mat.matrix[2][2] = (zFar + zNear) / (zNear - zFar);
	mat.matrix[2][3] = -1.0f;

	mat.matrix[3][0] = 0.0f;
	mat.matrix[3][1] = 0.0;
	mat.matrix[3][2] = 2 * zFar * zNear / (zNear - zFar);
	mat.matrix[3][3] = 0.0f;
	return mat;
}
mat4f mat4f::GenView(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
	float F0 = centerX - eyeX, F1 = centerY - eyeY, F2 = centerZ - eyeZ, Fmod = sqrt(pow(F0, 2) + pow(F1, 2) + pow(F2, 2));
	float f0 = F0 / Fmod, f1 = F1 / Fmod, f2 = F2 / Fmod;

	float UPmod = sqrt(pow(upX, 2) + pow(upY, 2) + pow(upZ, 2));
	float up0 = upX / UPmod, up1 = upY / UPmod, up2 = upZ / UPmod;

	float S0 = (f1 * up2) - (f2 * up1), S1 = (f2 * up0) - (f0 * up2), S2 = (f0 * up1) - (f1 * up0), Smod = sqrt(pow(S0, 2) + pow(S1, 2) + pow(S2, 2));
	float s0 = S0 / Smod, s1 = S1 / Smod, s2 = S2 / Smod;

	float u0 = (s1 * f2) - (s2 * f1), u1 = (s2 * f0) - (s0 * f2), u2 = (s0 * f1) - (s1 * f0);

	mat4f mat;
	mat.matrix[0][0] = S0;
	mat.matrix[0][1] = u0;
	mat.matrix[0][2] = -f0;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = S1;
	mat.matrix[1][1] = u1;
	mat.matrix[1][2] = -f1;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = S2;
	mat.matrix[2][1] = u2;
	mat.matrix[2][2] = -f2;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = -eyeX;
	mat.matrix[3][1] = -eyeY;
	mat.matrix[3][2] = -eyeZ;
	mat.matrix[3][3] = 1.0f;
	return mat;
}
mat4f& mat4f::GenView(mat4f& mat, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
	float F0 = centerX - eyeX, F1 = centerY - eyeY, F2 = centerZ - eyeZ, Fmod = sqrt(pow(F0, 2) + pow(F1, 2) + pow(F2, 2));
	float f0 = F0 / Fmod, f1 = F1 / Fmod, f2 = F2 / Fmod;

	float UPmod = sqrt(pow(upX, 2) + pow(upY, 2) + pow(upZ, 2));
	float up0 = upX / UPmod, up1 = upY / UPmod, up2 = upZ / UPmod;

	float S0 = (f1 * up2) - (f2 * up1), S1 = (f2 * up0) - (f0 * up2), S2 = (f0 * up1) - (f1 * up0), Smod = sqrt(pow(S0, 2) + pow(S1, 2) + pow(S2, 2));
	float s0 = S0 / Smod, s1 = S1 / Smod, s2 = S2 / Smod;

	float u0 = (s1 * f2) - (s2 * f1), u1 = (s2 * f0) - (s0 * f2), u2 = (s0 * f1) - (s1 * f0);
	mat.matrix[0][0] = S0;
	mat.matrix[0][1] = u0;
	mat.matrix[0][2] = -f0;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = S1;
	mat.matrix[1][1] = u1;
	mat.matrix[1][2] = -f1;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = S2;
	mat.matrix[2][1] = u2;
	mat.matrix[2][2] = -f2;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = -eyeX;
	mat.matrix[3][1] = -eyeY;
	mat.matrix[3][2] = -eyeZ;
	mat.matrix[3][3] = 1.0f;
	return mat;
}

mat4f mat4f::GenRotate(float angle, float X, float Y, float Z) {
	float c = cos(angle * M_PI / 180), s = sin(angle * M_PI / 180);
	float XYZmod = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	XYZmod = XYZmod != 0 ? XYZmod : 1;
	float x = X / XYZmod, y = Y / XYZmod, z = Z / XYZmod;
	mat4f mat;
	mat.matrix[0][0] = x * x * (1 - c) + c;
	mat.matrix[0][1] = x * y * (1 - c) + z * s;
	mat.matrix[0][2] = x * z * (1 - c) - y * s;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = y * x * (1 - c) - z * s;
	mat.matrix[1][1] = y * y * (1 - c) + c;
	mat.matrix[1][2] = y * z * (1 - c) + x * s;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = z * x * (1 - c) + y * s;
	mat.matrix[2][1] = z * y * (1 - c) - x * s;
	mat.matrix[2][2] = z * z * (1 - c) + c;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = 0.0f;
	mat.matrix[3][1] = 0.0f;
	mat.matrix[3][2] = 0.0f;
	mat.matrix[3][3] = 1.0f;
	return mat;
}
mat4f mat4f::GenRotate(float anglex, float angley, float anglez){
	float cx = cos(anglex * M_PI / 180), sx = sin(anglex * M_PI / 180);
	float cy = cos(angley * M_PI / 180), sy = sin(angley * M_PI / 180);
	float cz = cos(anglez * M_PI / 180), sz = sin(anglez * M_PI / 180);
	mat4f mat;
	mat.matrix[0][0] = cx * cy;
	mat.matrix[0][1] = sx * cy;
	mat.matrix[0][2] = -sy;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = cx * sy * sz - sx * cz;
	mat.matrix[1][1] = sx * sy * sz + cx * cz;
	mat.matrix[1][2] = cy * sz;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = cx * sy * cz + sx * sz;
	mat.matrix[2][1] = sx * sy * cz - cx * sz;
	mat.matrix[2][2] = cy * cz;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = 0.0f;
	mat.matrix[3][1] = 0.0f;
	mat.matrix[3][2] = 0.0f;
	mat.matrix[3][3] = 1.0f;
	return mat;
}
mat4f& mat4f::GenRotate(mat4f& mat,float anglex, float angley, float anglez) {
	float cx = cos(anglex * M_PI / 180), sx = sin(anglex * M_PI / 180);
	float cy = cos(angley * M_PI / 180), sy = sin(angley * M_PI / 180);
	float cz = cos(anglez * M_PI / 180), sz = sin(anglez * M_PI / 180);
	mat.matrix[0][0] = cx * cy;
	mat.matrix[0][1] = sx * cy;
	mat.matrix[0][2] = -sy;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = cx * sy * sz - sx * cz;
	mat.matrix[1][1] = sx * sy * sz + cx * cz;
	mat.matrix[1][2] = cy * sz;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = cx * sy * cz + sx * sz;
	mat.matrix[2][1] = sx * sy * cz - cx * sz;
	mat.matrix[2][2] = cy * cz;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = 0.0f;
	mat.matrix[3][1] = 0.0f;
	mat.matrix[3][2] = 0.0f;
	mat.matrix[3][3] = 1.0f;
	return mat;
}
mat4f mat4f::GenTranslate(float x, float y, float z) {
	mat4f mat;
	mat.matrix[0][0] = 1.0f;
	mat.matrix[0][1] = 0.0f;
	mat.matrix[0][2] = 0.0f;
	mat.matrix[0][3] = 0.0f;

	mat.matrix[1][0] = 0.0f;
	mat.matrix[1][1] = 1.0f;
	mat.matrix[1][2] = 0.0f;
	mat.matrix[1][3] = 0.0f;

	mat.matrix[2][0] = 0.0f;
	mat.matrix[2][1] = 0.0f;
	mat.matrix[2][2] = 1.0f;
	mat.matrix[2][3] = 0.0f;

	mat.matrix[3][0] = x;
	mat.matrix[3][1] = y;
	mat.matrix[3][2] = z;
	mat.matrix[3][3] = 1.0f;
	return mat;
}
mat4f& mat4f::Translate(mat4f& mat, float x, float y, float z) {
	mat.matrix[3][0] = x;
	mat.matrix[3][1] = y;
	mat.matrix[3][2] = z;
	mat.matrix[3][3] = 1.0f;
	return mat;
}

vec3f& mat4f::operator*(vec3f& right){
	vec3f ret;
	ret.v0 = this->matrix[0][0] * right.v0 + this->matrix[0][1] * right.v1 + this->matrix[0][2] * right.v2;
	ret.v1 = this->matrix[1][0] * right.v0 + this->matrix[1][1] * right.v1 + this->matrix[1][2] * right.v2;
	ret.v2 = this->matrix[2][0] * right.v0 + this->matrix[2][1] * right.v1 + this->matrix[2][2] * right.v2;
	right = ret;
	return right;
}