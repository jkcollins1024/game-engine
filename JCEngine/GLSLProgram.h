#pragma once
#include <string>
#include <GL/glew.h>

namespace JCEngine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexPath, const std::string& fragmentPath);
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);
		void linkShaders();
		void addAttribute(const std::string& attributeName);

		GLuint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();
		void dispose();

	private:
		GLuint _programID;
		GLuint _vertexID;
		GLuint _fragmentID;
		int _numAttributes;

		void compileShader(const char* source, GLuint shaderID, std::string name);
		void loadFile(const std::string& filePath);
	};
}



