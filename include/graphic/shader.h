#include <GL/glew.h>

#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <string>
#include <core/log.h>


namespace emp
{
	class Shader {
	public:
		const char* vertexShaderSource;
		const char* fragmentShaderSource;
		const char* fragmentShader2Source;
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
		unsigned int VBO, VAO, EBO;
		std::string vertexCode;
		std::string fragmentCode;

		float shininess = 32.0f;

		Shader() {

			vertexShaderSource = "";	
			fragmentShaderSource = "";
			fragmentShader2Source  = "";
			vertexShader;
			fragmentShader ;
			shaderProgram;
		}

		bool static CheckVertexCompile(unsigned int& vertexShader) {
			// check for shader compile errors
			bool result = true;
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				result = false;
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::string msg = "[SHADER] VERTEX::COMPILATION_FAILED";
				LOG::Error(msg);
			}
			return result;
		}

		bool static CheckFragmentCompile(unsigned int& fragmentShader) {
			// check for shader compile errorsg
			bool result = true;
			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				result = false;
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::string msg = "[SHADER] FRAGMENT::COMPILATION_FAILED";
				LOG::Error(msg);
			}
			return result;
		}

		bool static ChechShaderCompile(unsigned int& shaderProgram) {
			// check for shader compile errorsg
			bool result = true;
			int success;
			char infoLog[512];
			// check for linking errors
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				result = false;
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				std::string msg = "[SHADER] PROGRAM : LINKING_FAILED";
				LOG::Error(msg);
			}
			return result;
		}

		void BindVertexArray(GLuint array) {
			glBindVertexArray(array);
		}

		

		bool Init(std::string vertexShaderData, std::string fragmentShaderData) {

			this->vertexCode = vertexShaderData;
			vertexShaderSource = vertexCode.c_str();

			this->fragmentCode = fragmentShaderData;
			fragmentShaderSource = fragmentCode.c_str();
			bool error = false;
            // build and compile our shader program
            // ------------------------------------
            // vertex shader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
			error = !CheckVertexCompile(vertexShader);
            // fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
			error = !CheckFragmentCompile(fragmentShader);
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
			error = !ChechShaderCompile(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader); ;
			return error;
		}

		void UseProgram() {
			glUseProgram(shaderProgram);
		}
		
		void BindVertexArray() {
			glBindVertexArray(0);
		}

        void Draws() {
        }

		void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
			  glDrawElements(mode, count, type, indices);
		}

		void DrawArrays(GLenum mode, GLint first, GLsizei count) {
			glDrawArrays(mode, first, count);
		}

		void SetInt(std::string name, int i) {
			unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform1i(viewPosLoc, i);
		}


		void SetFloat(std::string name, float f) {
			unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform1f(viewPosLoc, f);
		}

        void SetVec3(std::string name, glm::vec3 vec) {
            unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
            glUniform3f(viewPosLoc, vec.x, vec.y, vec.z);
        }

		void SetVec3(std::string name, float x, float y, float z) {
            unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
            glUniform3f(viewPosLoc, x, y, z);
        }

		void SetVec4(std::string name, glm::vec4 vec) {
            unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
            glUniform4f(viewPosLoc, vec.x, vec.y, vec.z, vec.w);
        }
		void SetVec4(std::string name, float x, float y, float z, float w)
        {
            unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
            glUniform4f(viewPosLoc, x, y, z, w);
        }	

		void SetMat4(std::string name, glm::mat4& mat) {
			unsigned int viewLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat[0][0]);
		}
	};
}