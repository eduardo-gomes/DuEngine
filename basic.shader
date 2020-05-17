#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//layout(location = 0) out vec4 gl_position;
void main(){
	gl_Position = position;
};

#shader fragment
#version 330 core

//layout(location = 1) in vec4 incolor;
layout(location = 0) out vec4 color;
void main(){
	color = vec4(0.0, 1.0, 1.0, 1.0);
}