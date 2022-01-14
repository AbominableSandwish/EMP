namespace emp
{
	//TextGraphic::TextGraphic(Entity& entity, string name) : GraphicComponent(entity, name)
	//{
	//}

	//TextGraphic::TextGraphic(Entity& entity, string text, string name) : GraphicComponent(entity, name)
	//{
	//    this->text = text;
	//    entity.AddComponent(this);
	//}

	//void TextGraphic::Init() {
	//    // OpenGL state
	//        // ------------
	//        //glEnable(GL_CULL_FACE);
	//    glEnable(GL_BLEND);
	//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//    const char* vShaderCode = "#version 330 core\n"
	//        "layout(location = 0) in vec4 vertex; \n"
	//        "out vec2 TexCoords;\n"

	//        "uniform mat4 projection;\n"

	//        "void main()\n"
	//        "{\n"
	//        "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	//        "TexCoords = vertex.zw;\n"
	//        "}\0";

	//    const char* fShaderCode = "#version 330 core\n"
	//        "in vec2 TexCoords;\n"
	//        "out vec4 color;\n"

	//        "uniform sampler2D text;\n"
	//        "uniform vec3 textColor;\n"

	//        "void main()\n"
	//        "{\n"
	//        "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	//        "color = vec4(textColor, 1.0) * sampled;\n"
	//        "}\0";

	//    // 2. compile shaders
	//    unsigned int vertex, fragment;
	//    // vertex shader
	//    vertex = glCreateShader(GL_VERTEX_SHADER);
	//    glShaderSource(vertex, 1, &vShaderCode, NULL);
	//    glCompileShader(vertex);
	//    // utility function for checking shader compilation/linking errors.
	//    // ------------------------------------------------------------------------
	//    GLint success;
	//    GLchar infoLog[1024];
	//    if ("VERTEX" != "PROGRAM")
	//    {
	//        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	//        if (!success)
	//        {
	//            glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
	//            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	//        }
	//    }
	//    else
	//    {
	//        glGetProgramiv(vertex, GL_LINK_STATUS, &success);
	//        if (!success)
	//        {
	//            glGetProgramInfoLog(vertex, 1024, NULL, infoLog);
	//            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	//        }
	//    }
	//    // fragment Shader
	//    fragment = glCreateShader(GL_FRAGMENT_SHADER);
	//    glShaderSource(fragment, 1, &fShaderCode, NULL);
	//    glCompileShader(fragment);
	//    // utility function for checking shader compilation/linking errors.
	//    // ------------------------------------------------------------------------
	//    success;
	//    infoLog[1024];
	//    if ("FRAGMENT" != "PROGRAM")
	//    {
	//        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	//        if (!success)
	//        {
	//            glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
	//            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	//        }
	//    }
	//    else
	//    {
	//        glGetProgramiv(fragment, GL_LINK_STATUS, &success);
	//        if (!success)
	//        {
	//            glGetProgramInfoLog(fragment, 1024, NULL, infoLog);
	//            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	//        }
	//    }
	//    // shader Program
	//    this->shaderProgram = glCreateProgram();
	//    glAttachShader(shaderProgram, vertex);
	//    glAttachShader(shaderProgram, fragment);
	//    glLinkProgram(shaderProgram);
	//    success;
	//    infoLog[1024];
	//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	//    if (!success)
	//    {
	//        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
	//        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	//    }
	//    // delete the shaders as they're linked into our program now and no longer necessery
	//    glDeleteShader(vertex);
	//    glDeleteShader(fragment);

	//    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1920), 0.0f, static_cast<float>(1080 - 30 - 40));
	//    glUseProgram(shaderProgram);
	//    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//    // FreeType
	//    // --------
	//    FT_Library ft;
	//    // All functions return a value different than 0 whenever an error occurred
	//    if (FT_Init_FreeType(&ft))
	//    {
	//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//        return;
	//    }

	//    // find path to font
	//    std::string font_name = "./data/fonts/Inconsolata.ttf";
	//    if (font_name.empty())
	//    {
	//        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
	//        return;
	//    }

	//    // load font as face
	//    FT_Face face;
	//    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
	//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//        return;
	//    }
	//    else {
	//        // set size to load glyphs as
	//        FT_Set_Pixel_Sizes(face, 0, 48);

	//        // disable byte-alignment restriction
	//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//        // load first 128 characters of ASCII set
	//        for (unsigned char c = 0; c < 128; c++)
	//        {
	//            // Load character glyph 
	//            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	//            {
	//                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//                continue;
	//            }
	//            // generate texture
	//            unsigned int texture;
	//            glGenTextures(1, &texture);
	//            glBindTexture(GL_TEXTURE_2D, texture);
	//            glTexImage2D(
	//                GL_TEXTURE_2D,
	//                0,
	//                GL_RED,
	//                face->glyph->bitmap.width,
	//                face->glyph->bitmap.rows,
	//                0,
	//                GL_RED,
	//                GL_UNSIGNED_BYTE,
	//                face->glyph->bitmap.buffer
	//            );
	//            // set texture options
	//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//            // now store character for later use
	//            Character character = {
	//                texture,
	//                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//                static_cast<unsigned int>(face->glyph->advance.x)
	//            };
	//            Characters.insert(std::pair<char, Character>(c, character));
	//        }
	//        glBindTexture(GL_TEXTURE_2D, 0);
	//    }
	//    // destroy FreeType once we're finished
	//    FT_Done_Face(face);
	//    FT_Done_FreeType(ft);


	//    // configure VAO/VBO for texture quads
	//    // -----------------------------------
	//    glGenVertexArrays(1, &VAO);
	//    glGenBuffers(1, &VBO);
	//    glBindVertexArray(VAO);
	//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	//    glEnableVertexAttribArray(0);
	//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//    glBindBuffer(GL_ARRAY_BUFFER, 0);
	//    glBindVertexArray(0);
	//}

	//void TextGraphic::RenderText(unsigned& shader, std::string text, float x, float y, float scale, glm::vec3 color)
	//{
	//    // activate corresponding render state	
	//    glUseProgram(shader);
	//    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
	//    glActiveTexture(GL_TEXTURE0);
	//    glBindVertexArray(VAO);

	//    // iterate through all characters
	//    std::string::const_iterator c;
	//    for (c = text.begin(); c != text.end(); c++)
	//    {
	//        Character ch = Characters[*c];

	//        float xpos = x + ch.Bearing.x * scale;
	//        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

	//        float w = ch.Size.x * scale;
	//        float h = ch.Size.y * scale;
	//        // update VBO for each character
	//        float vertices[6][4] = {
	//            { xpos,     ypos + h,   0.0f, 0.0f },
	//            { xpos,     ypos,       0.0f, 1.0f },
	//            { xpos + w, ypos,       1.0f, 1.0f },

	//            { xpos,     ypos + h,   0.0f, 0.0f },
	//            { xpos + w, ypos,       1.0f, 1.0f },
	//            { xpos + w, ypos + h,   1.0f, 0.0f }
	//        };
	//        // render glyph texture over quad
	//        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	//        // update content of VBO memory
	//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

	//        glBindBuffer(GL_ARRAY_BUFFER, 0);
	//        // render quad
	//        glDrawArrays(GL_TRIANGLES, 0, 6);
	//        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	//        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	//    }
	//    glBindVertexArray(0);
	//    glBindTexture(GL_TEXTURE_2D, 0);
	//}

	//void TextGraphic::Draw()
	//{
	//    emp::Transform transform;
	//    // create transformations
	//    for (emp::Component* component : entity->components)
	//    {
	//        if (component->name == "Transform") {
	//            transform = static_cast<emp::Transform&>(*component);
	//        }
	//    }
	//	
	//    RenderText(shaderProgram, text, transform.x * 100, transform.y * 100, 1.0f, glm::vec3(0.8, 0.0f, 0.9f));
	//}
}