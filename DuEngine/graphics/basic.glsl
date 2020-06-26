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

uniform sampler2D u_Texture[32];

void main(){
	int texID = int(round(v_TexID));
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
		case 9: color = texture(u_Texture[8], v_TexCoord); break;
		case 10: color = texture(u_Texture[9], v_TexCoord); break;
		case 11: color = texture(u_Texture[10], v_TexCoord); break;
		case 12: color = texture(u_Texture[11], v_TexCoord); break;
		case 13: color = texture(u_Texture[12], v_TexCoord); break;
		case 14: color = texture(u_Texture[13], v_TexCoord); break;
		case 15: color = texture(u_Texture[14], v_TexCoord); break;
		case 16: color = texture(u_Texture[15], v_TexCoord); break;
		case 17: color = texture(u_Texture[16], v_TexCoord); break;
		case 18: color = texture(u_Texture[17], v_TexCoord); break;
		case 19: color = texture(u_Texture[18], v_TexCoord); break;
		case 20: color = texture(u_Texture[19], v_TexCoord); break;
		case 21: color = texture(u_Texture[20], v_TexCoord); break;
		case 22: color = texture(u_Texture[21], v_TexCoord); break;
		case 23: color = texture(u_Texture[22], v_TexCoord); break;
		case 24: color = texture(u_Texture[23], v_TexCoord); break;
		case 25: color = texture(u_Texture[24], v_TexCoord); break;
		case 26: color = texture(u_Texture[25], v_TexCoord); break;
		case 27: color = texture(u_Texture[26], v_TexCoord); break;
		case 28: color = texture(u_Texture[27], v_TexCoord); break;
		case 29: color = texture(u_Texture[28], v_TexCoord); break;
		case 30: color = texture(u_Texture[29], v_TexCoord); break;
		case 31: color = texture(u_Texture[30], v_TexCoord); break;
		case 32: color = texture(u_Texture[31], v_TexCoord); break;
	}
	if(color.a<=0.0 ){
		discard;
	}
}