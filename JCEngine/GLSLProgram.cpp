#include "GLSLProgram.h"
#include "GameErrors.h"
#include "IOManager.h"

#include <fstream>

namespace JCEngine {
	GLSLProgram::GLSLProgram() : _programID(0), _vertexID(0), _fragmentID(0), _numAttributes(0) {

	}

	GLSLProgram::~GLSLProgram() {

	}

	void GLSLProgram::compileShaders(const std::string& vertexPath, const std::string& fragmentPath) {

		std::string vertexSource;
		std::string fragmentSource;

		if (!IOManager::readFileToString(vertexPath, vertexSource))
			fatalError("Could not read file: " + vertexPath);

		if (!IOManager::readFileToString(fragmentPath, fragmentSource))
			fatalError("Could not read file: " + fragmentPath);

		_programID = glCreateProgram();

		_vertexID = glCreateShader(GL_VERTEX_SHADER);
		_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		if (_vertexID == 0 || _fragmentID == 0) {
			fatalError("Shaders could not be created");
		}

		compileShader(vertexSource.c_str(), _vertexID, vertexPath);
		compileShader(fragmentSource.c_str(), _fragmentID, fragmentPath);
	}

	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource)
	{
		_programID = glCreateProgram();

		_vertexID = glCreateShader(GL_VERTEX_SHADER);
		_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		if (_vertexID == 0 || _fragmentID == 0) {
			fatalError("Shaders could not be created");
		}

		compileShader(vertexSource, _vertexID, "Vertex Shader");
		compileShader(fragmentSource, _fragmentID, "Fragment Source");
	}

	void GLSLProgram::compileShader(const char* source, GLuint shaderID, std::string name) {
		
		glShaderSource(shaderID, 1.0, &source, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(shaderID, 1024, &log_length, message);

			// Write the error to a log
			std::printf("%s\n", &message[0]);
			fatalError("Shader " + name + " failed to compile");
		}
	}

	void GLSLProgram::linkShaders() {
		// This step is unnecessary if you use the location specifier in your shader
		// e.g. layout (location = 0) in vec3 position;
		glBindAttribLocation(_programID, 0, "position"); // The index passed into glBindAttribLocation is
		glBindAttribLocation(_programID, 1, "texcoord"); // used by glEnableVertexAttribArray. "position"
		glBindAttribLocation(_programID, 2, "normal");   // "texcoord" "normal" and "color" are the names of the
		glBindAttribLocation(_programID, 3, "color");    // respective inputs in your fragment shader.

		glAttachShader(_programID, _vertexID);
		glAttachShader(_programID, _fragmentID);

		glLinkProgram(_programID);

		GLint program_linked;
		glGetProgramiv(_programID, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetProgramInfoLog(_programID, 1024, &log_length, message);
			// Write the error to a log
			std::printf("%s\n", &message[0]);
			fatalError("Shader linking failed");

			glDeleteProgram(_programID);
			glDeleteShader(_vertexID);
			glDeleteShader(_fragmentID);
		}

		glDetachShader(_programID, _vertexID);
		glDetachShader(_programID, _fragmentID);
		glDeleteShader(_vertexID);
		glDeleteShader(_fragmentID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}

		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}
	void GLSLProgram::dispose()
	{
		if (_programID)
			glDeleteProgram(_programID);
	}
}

