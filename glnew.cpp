#include <csignal>
#include <iostream>
#include "graphics.hpp"
struct vertex2d{
	float x, y, s, t;
};
#include <fstream>
#include <sstream>
#define ASSERT(X) if(!(X)) raise(SIGTRAP);
struct ShaderProgramSource{
	std::string VertexSource;
	std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string& filepath){
	std::ifstream stream(filepath);
	enum class ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type= ShaderType::NONE;
	std::string line;
	std::stringstream ss[2];
	while (getline(stream, line)){
		if(line.find("vertex") != std::string::npos){
			type = ShaderType::VERTEX;
		} else if (line.find("fragment") != std::string::npos) {
			type = ShaderType::FRAGMENT;
		}else{
			ss[(int) type] << line << '\n';
		}
	}
	return {ss[0].str(), ss[1].str()};
}
unsigned int buffer;
static unsigned int compileshader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca((unsigned)length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Cant compile shader " << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader){
	unsigned int program = glCreateProgram();
	unsigned int vs = compileshader(GL_VERTEX_SHADER, vertexshader);
	unsigned int fs = compileshader(GL_FRAGMENT_SHADER, fragmentshader);

	glAttachShader(program, vs);  // documentation
	glAttachShader(program, fs);  // documentation
	glLinkProgram(program);		  // documentation
	glValidateProgram(program);	  // documentation

	glDeleteShader(vs);	 // documentation
	glDeleteShader(fs);	 // documentation
	return program;
}
unsigned int shader;
void Inicializa(){
	//SDL_GL_SetSwapInterval(0);
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	vertex2d positions[4] = {
		-0.5,-0.5, 0, 0,
		 0.5,-0.5, 0, 0,
		 0.5, 0.5, 0, 0,
		-0.5, 0.5, 0, 0
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);  // documentation
	glBindVertexArray(vertexArrayID);	   // documentation

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(vertex2d), positions, GL_STATIC_DRAW);//each vertexarray has one so to use other has to switch
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex2d), 0);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	ShaderProgramSource source = ParseShader("basic.shader");
	//std::cout << "Vertex" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "Fragment" << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	shader = CreateShader(source.VertexSource, source.FragmentSource);
	gltry(glUseProgram(shader));

	//set color
	int location = glGetUniformLocation(shader, "u_color");
	ASSERT(location != -1);
	gltry(glUniform4f(location, 0.0f, 0.25f, 0.95f, 1.0f));
}
void render() {
	glEnable(GL_BLEND);// to use transparency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// to use transparency
	window::ClearErrors();
	gltry(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));//documentation
	
}
int main(){
	if(!window::init()){
		window::MainLoop();
	}
	glDeleteProgram(shader);
}