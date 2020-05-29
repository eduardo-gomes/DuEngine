#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 L_Color;
layout(location = 2) in vec2 L_texCoord;
layout(location = 3) in float L_TexID;

uniform mat4 u_MVP;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexID;

void main(){
	gl_Position = u_MVP * position;
	v_TexCoord = L_texCoord;
	v_Color = L_Color;
	v_TexID = L_TexID;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexID;

uniform sampler2D u_Texture[8];

void main(){
	int texID = int(v_TexID);
	switch(texID){
		case 0: color = v_Color; break;
		case 1: color = texture(u_Texture[0], v_TexCoord); break;
		case 2: color = texture(u_Texture[1], v_TexCoord); break;
		case 3: color = texture(u_Texture[2], v_TexCoord); break;
		case 4: color = texture(u_Texture[3], v_TexCoord); break;
		case 5: color = texture(u_Texture[4], v_TexCoord); break;
		case 6: color = texture(u_Texture[5], v_TexCoord); break;
		case 7: color = texture(u_Texture[6], v_TexCoord); break;
		case 8: color = texture(u_Texture[7], v_TexCoord); break;
	}
	if(color.a<=0.0 ){
		discard;
	}
}