#include "Shader.h"

namespace utility_opengl{
	
	Shader& Shader::use(){

		glUseProgram(id);

		return *this;

	}

	void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource){
		
		GLuint sVertex, sFragment, gShader;

		// Vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSource, NULL);
		glCompileShader(sVertex);
		checkCompileErrors(sVertex, "VERTEX");

		// Fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSource, NULL);
		glCompileShader(sFragment);
		checkCompileErrors(sFragment, "FRAGMENT");

		// If geometry shader source code is given, also compile geometry shader
		if (geometrySource != nullptr){
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &geometrySource, NULL);
			glCompileShader(gShader);
			checkCompileErrors(gShader, "GEOMETRY");
		}

		// Shader Program
		id = glCreateProgram();
		glAttachShader(id, sVertex);
		glAttachShader(id, sFragment);
		if (geometrySource != nullptr)
			glAttachShader(id, gShader);
		glLinkProgram(id);
		checkCompileErrors(id, "PROGRAM");

		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		if (geometrySource != nullptr)
			glDeleteShader(gShader);

	}

	void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader){
		if (useShader) this->use();
		glUniform1f(glGetUniformLocation(this->id, name), value);
	}

	void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader){
		if (useShader) this->use();
		glUniform1i(glGetUniformLocation(this->id, name), value);
	}

	void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader){
		if (useShader) this->use();
		glUniform2f(glGetUniformLocation(this->id, name), x, y);
	}

	void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader){
		if (useShader) this->use();
		glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
	}

	void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader){
		if (useShader) this->use();
		glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
	}

	void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader){
		if (useShader) this->use();
		glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
	}

	void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader){
		if (useShader) this->use();
		glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
	}

	void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader){
		if (useShader) this->use();
		glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
	}

	void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader){
		if (useShader) this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::checkCompileErrors(GLuint object, std::string type){

		GLint success;
		GLchar infoLog[1024];

		if (type != "PROGRAM"){
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success){
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
			else{
				if(type == "VERTEX")
					std::cout << getShaderName(vFileName) <<" foi compilado com sucesso." << std::endl;
				else if(type == "FRAGMENT")
					std::cout << getShaderName(fFileName) <<" foi compilado com sucesso." << std::endl;
				else if(type == "GEOMETRY")
					std::cout << getShaderName(gFileName) <<" foi compilado com sucesso." << std::endl;
			}
		}
		else{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success){
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
			else
				std::cout << std::endl << "Programa foi linkado com sucesso." << std::endl << std::endl << std::endl;
		}
	}

	std::string getShaderName(const GLchar* filePath){
		std::string filePathString(filePath), outString;
		std::stringstream ss;
		for(int i = filePathString.size() - 1; !(filePathString[i] == '/' || filePathString[i] == '\\'); i--)
			ss << filePathString[i];
		outString = ss.str();
		ss.str("");
		for(std::string::reverse_iterator rit = outString.rbegin(); rit != outString.rend(); ++rit)
			ss << *rit;
		return ss.str();
	}
	
}
