#include <cstdio>
#include <cstdlib>

#include <iostream>
#include "GL/glew.h"


namespace emp
{
    namespace Texture
    {
    	class Texture
    	{
            unsigned int  id;
    	};
    	
    	static void loadTexture(unsigned char* file, int& width, int& height, unsigned int &id, const char* path, int level)
    	{
            // load and create a texture 
			// -------------------------
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (file)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, file);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
    	}
    	
        static void loadCompressedTexture(const char* path, unsigned int level)
        {
            GLushort paddedWidth;
            GLushort paddedHeight;
            GLushort width;
            GLushort height;
            GLubyte textureHead[16];
            GLubyte* theTexture;
            FILE* theFile = fopen(path, "rb");
            if (theFile == NULL)
            {
                std::cout << ("Failure to load the texture");
                return;
            }
            fread(textureHead, 16, 1, theFile);
            paddedWidth = (textureHead[8] << 8) | textureHead[9];
            paddedHeight = (textureHead[10] << 8) | textureHead[11];
            width = (textureHead[12] << 8) | textureHead[13];
            height = (textureHead[14] << 8) | textureHead[15];
            theTexture = (GLubyte*)malloc(sizeof(GLubyte) * ((paddedWidth * paddedHeight) >> 1));
            fread(theTexture, (paddedWidth * paddedHeight) >> 1, 1, theFile);
            /* Load the texture. */
            glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, (paddedWidth * paddedHeight) >> 1, theTexture);
            /* Set the filtering mode. */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            free(theTexture);
            fclose(theFile);
        }
    }
}
