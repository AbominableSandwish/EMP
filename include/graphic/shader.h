#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <string>

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

		Shader() {

			vertexShaderSource = "";
			fragmentShaderSource = "";
			fragmentShader2Source  = "";
			vertexShader = 0;
			fragmentShader  = 0;
			shaderProgram = 0;
			VBO = 0; VAO = 0; EBO = 0;
		}

		void static CheckVertexCompile(unsigned int& vertexShader) {
			// check for shader compile errors
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::string msg = "[SHADER] VERTEX::COMPILATION_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}

		void static CheckFragmentCompile(unsigned int& fragmentShader) {
			// check for shader compile errorsg
			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::string msg = "[SHADER] FRAGMENT::COMPILATION_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}

		void static ChechShaderCompile(unsigned int& shaderProgram) {
			// check for shader compile errorsg
			int success;
			char infoLog[512];
			// check for linking errors
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				std::string msg = "[SHADER] PROGRAM::LINKING_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}

		void Init(std::string vertexShaderData, std::string fragmentShaderData) {
            // build and compile our shader program
            // ------------------------------------
            // vertex shader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            CheckVertexCompile(vertexShader);
            // fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            CheckFragmentCompile(fragmentShader);
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
            ChechShaderCompile(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader); ;
		}

		void Init(std::string vertexShaderData, std::string fragmentShaderData, float vertices[], unsigned int indices[]) {
			// build and compile our shader program
			// ------------------------------------
			// vertex shader
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);
			// check for shader compile errors
			CheckVertexCompile(vertexShader);
			// fragment shader
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);
			// check for shader compile errors
			CheckFragmentCompile(fragmentShader);
			// link shaders
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);
			// check for linking errors
			ChechShaderCompile(shaderProgram);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader); ;


			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture coord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
			// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
			glBindVertexArray(0);
		}

		void Init(std::string vertexShaderData, std::string fragmentShaderData, float vertices[]) {
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindVertexArray(VAO);
			// position attribute
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// normal attribut;
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			//SALUT MON pote
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// note that we update the lamp's position attribute's stride to reflect the updated buffer data
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		void UseProgram() {
			glUseProgram(this->shaderProgram);
		}
		
		void BindVertexArray() {
			glBindVertexArray(VAO);
		}

        void Draws() {
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

        void SetMat4(std::string name, glm::mat4 mat) {
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, name.c_str());
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat[0][0]);
        }		
	};
}