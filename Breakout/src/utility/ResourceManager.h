#pragma once

#include "Shader.h"
#include "Texture2D.h"

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <map>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace utility_opengl{

	class ResourceManager{

	private:
		// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager(){}
		//Loads and generates a shader from file
		static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
		// Loads a single texture from file
		static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);

	public:
		//Resource storage
		static std::map<std::string, Shader>     shaders;
		static std::map<std::string, Texture2D>  textures;
		//Loads (and generates)
		static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
		//Retrives a stored shader
		static Shader getShader(std::string name);
		//Loads (and generates) a texture from file
		static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);
		//Retrives a stored texture
		static Texture2D getTexture(std::string name);
		//Properly de-allocates all loaded resources
		static void clear();

	};

}