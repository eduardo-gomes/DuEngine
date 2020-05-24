#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform vec4 u_position;
uniform mat4 u_MVP;

mat4 TransformMatrix = mat4(1.0);  //TODO: Calc this on cpu

void main(){
	TransformMatrix[0][2] = u_position.x;  //TODO: Calc this on cpu
	TransformMatrix[1][2] = u_position.y;  //TODO: Calc this on cpu
	TransformMatrix[2][2] = u_position.z;  //TODO: Calc this on cpu
	gl_Position = u_MVP * TransformMatrix * position ;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_color;
uniform sampler2D u_Texture;

void main(){
	//color = u_color;
	
	vec4 texColor = texture(u_Texture, v_TexCoord);

	color = texColor;

	if(color.a<=0.0 ){
		discard;
	}
}