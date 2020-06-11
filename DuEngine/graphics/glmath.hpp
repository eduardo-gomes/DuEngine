#ifndef _GLMATH
#define _GLMATH
struct vec2f {
	float v0, v1;
};
struct vec3f {
	float v0, v1, v2;
};
struct vec4f {
	float v0, v1, v2, v3;
};
struct mat4f {
	float matrix[4][4];	 //colun row
	mat4f operator*(const mat4f&);
	static mat4f GenPerspective(float fovy, float aspect, float zNear, float zFar);
	static mat4f& GenPerspective(mat4f& mat, float fovy, float aspect, float zNear, float zFar);
	static mat4f GenView(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f& GenView(mat4f& mat, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f GenRotate(float angle, float x, float y, float z);
	static mat4f GenRotate(float anglex, float angley, float anglez);
	static mat4f& GenRotate(mat4f& mat, float anglex, float angley, float anglez);
	static mat4f GenTranslate(float x, float y, float z);
	static mat4f& Translate(mat4f&, float x, float y, float z);
	vec3f& operator*(vec3f&);
};
#endif