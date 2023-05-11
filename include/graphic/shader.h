#include <GL/glew.h>

#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <string>
#include <core/log.h>

namespace emp
{
	class Shader {
	public:
		GLuint programId = 0;
		GLuint vertexShader =0;
		GLuint fragmentShader = 0;
		GLuint shaderProgram = 0;
		GLuint VBO = 0, VAO = 0, EBO =0;
		GLuint IBO = 0;
		GLuint buffer = 0;
		const GLchar* vertexShaderSource;
		const GLchar* fragmentShaderSource;
		std::vector<float> vertices;

		unsigned int diffuse_map;
		unsigned int specular_map;
		

		float shininess = 32.0f;
		Shader() {
			// build and compile our shader program
			// ------------------------------------
			shaderProgram = glCreateProgram();
			vertexShader = glCreateShader(GL_VERTEX_SHADER); //Create a Vertex Shader Object
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Create a Fragment Shader Object
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
		void BindVertexArray() {
			glBindVertexArray(VAO);
		}
		void BindVertexArray(int n) {
			glBindVertexArray(n);
		}

		bool SetVertexShader(std::string vertexShaderData) {
			vertexShaderSource = vertexShaderData.c_str();
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);
			// check for shader compile errors
			return !CheckVertexCompile(vertexShader);
		}
		
		bool SetFragmentShader(std::string fragmentShaderData) {
			fragmentShaderSource = fragmentShaderData.c_str();
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);
			// check for shader compile errors
			return !CheckFragmentCompile(fragmentShader);
		}
		

		bool Init(std::string vertexShaderData, std::string fragmentShaderData) {
			bool error = false;
            // Set vertex shader
			error = SetVertexShader(vertexShaderData);
			
			// Set fragment shader
			error = SetFragmentShader(fragmentShaderData);
            // link shaders
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

		void GenVertexArrays() {
			glGenVertexArrays(1, &VAO);
		}

		void GenBuffers() {
			glGenBuffers(1, &VBO);
		}

		void SetVertexArrays(std::vector<float> v) {
			this->vertices = v;
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s). 
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
			glBindVertexArray(VAO);
			// position attribute 
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// normal attribut; 
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
			//buffer transform array
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
		}

		void GenerateTexture() {
			// at init time
			glm::vec4 color = glm::vec4(1, 1, 1, 1);

			glGenTextures(0, &diffuse_map);
			glBindTexture(GL_TEXTURE_2D, diffuse_map);

			// set texture wrap parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filter parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// set image data
			const int pixel_size = 156;
			unsigned char* data = new unsigned char[3 * pixel_size * pixel_size * sizeof(unsigned char)];
			for (unsigned int i = 0; i < pixel_size * pixel_size; i++)
			{
				data[i * 3] = (unsigned char)(color.x * 255.0f);
				data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
				data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixel_size, pixel_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			delete[] data;

			glGenTextures(1, &specular_map);
			glBindTexture(GL_TEXTURE_2D, specular_map);

			// set texture wrap parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filter parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			data = new unsigned char[3 * pixel_size * pixel_size * sizeof(unsigned char)];
			for (unsigned int i = 0; i < pixel_size * pixel_size; i++)
			{
				data[i * 3] = (unsigned char)(color.x * 255.0f);
				data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
				data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixel_size, pixel_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			delete[] data;

			UseProgram();

			SetInt("material.diffuse", 0);
			SetInt("material.specular", 1);
		}

		void BindTexture() {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse_map);
			// at init time.
			//glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
			glGenTextures(1, &specular_map);
			glBindTexture(GL_TEXTURE_2D, specular_map);
			// bind specular map
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specular_map);
		}


		void SetVertexArrays(float *p, const size_t size) {
			float* vertices;
		    vertices = new float[size]();
			for (int i =  0 ; i < size-1; i++)
			{
				(& vertices)[i] = (&p)[i];
				
			}
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s). 
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glBindVertexArray(VAO);
			// position attribute 
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// normal attribut; 
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
			//buffer transform array
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
		}

        void Draws() {
        }

		void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
			  glDrawElements(mode, count, type, indices);
		}

		void DrawArrays(GLenum mode, GLint first, GLsizei count) {
			glDrawArrays(mode, first, count);
		}


		void DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount) {
			glDrawArraysInstanced(mode, first, count, primcount);
		}

		void SetInt(std::string name, int i) {
			unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform1i(viewPosLoc, i);
		}


		void SetFloat(std::string name, float f) {
			unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform1f(viewPosLoc, f);
		}

		void SetVec2(std::string name, glm::vec2 vec) {
			unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform2f(viewPosLoc, vec.x, vec.y);
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

		void SetBuffetDataMat4(std::string name, glm::mat4& mats) {
			glBufferData(GL_ARRAY_BUFFER, 2048 * 16 * sizeof(float), &mats, GL_STATIC_DRAW);
			int transform_location = glGetAttribLocation(shaderProgram, "aTransform");
			BindVertexArray();
			for (unsigned int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(transform_location + i);
				glVertexAttribPointer(transform_location + i, 4, GL_FLOAT, GL_FALSE,
					16 * sizeof(float),
					(const GLvoid*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(transform_location + i, 1);
			}
		}

		void Free() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteBuffers(1, &buffer);
			glDeleteProgram(shaderProgram);
			 // optional: de-allocate all resources once they've outlived their purpose:
			// ------------------------------------------------------------------------
		}
	};
}